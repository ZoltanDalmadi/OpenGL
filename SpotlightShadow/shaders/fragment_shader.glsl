#version 430 core

in vec3 normal;
in vec3 fragPos;
in vec2 texCoords;
in vec4 fragPosinLightSpace;
in vec4 projTexCoords;

out vec4 fragColor;

uniform vec3 camPos;
uniform sampler2D shadowMap;
uniform sampler2D projectTex;

struct Material
{
  vec3 diffuse;
  sampler2D diffuseTex;
  float diffMix;
  vec3 specular;
  sampler2D specularTex;
  float specMix;
  float shininess;
};
uniform Material material;

struct SpotLight
{
  vec3 position;
  vec3 direction;
  float cutOff;
  float outerCutOff;

  float constant;
  float linear;
  float quadratic;

  vec3 ambient;
  vec3 diffuse;
  vec3 specular;

  float energy;
};
uniform SpotLight spotLight;

uniform bool rect;

subroutine void renderPassType();
subroutine uniform renderPassType renderPass;

float shadowCalculation(out float closestDepth, out float currentDepth)
{
  // perform perspective divide
  vec3 projCoords = fragPosinLightSpace.xyz / fragPosinLightSpace.w;

  // Transform to [0, 1] range
  projCoords = projCoords * 0.5 + 0.5;

  // Get closest depth value from light's perspective
  closestDepth = texture(shadowMap, projCoords.xy).r;

  // Get depth of current fragment from light's perspective
  currentDepth = projCoords.z;

  // Calculate bias (based on depth map resolution and slope)
  vec3 normal_l = normalize(normal);
  vec3 lightDir = normalize(spotLight.position - fragPos);
  float bias = max(0.05 * (1.0 - dot(normal_l, lightDir)), 0.005);

  // PCF
  float shadow = 0.0;
  vec2 texelSize = 1.0 / textureSize(shadowMap, 0);

  for(int x = -1; x <= 1; ++x) {
    for(int y = -1; y <= 1; ++y) {
      float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r;
      shadow += currentDepth - bias > pcfDepth  ? 1.0 : 0.0;
    }
  }

  shadow /= 9.0;

  // Keep the shadow at 0.0 when outside the far_plane region of the light's frustum.
  if(projCoords.z > 1.0)
      shadow = 0.0;

  return shadow;
}

void calcRectangleSpotlight(SpotLight light, out float xProj, out float yProj, out float tgA, out float tgB)
{
  tgA = tan(light.outerCutOff);
  tgB = tan(light.cutOff);
  vec3 up = vec3(0.0f, 1.0f, 0.0f);
  vec3 xAxis = normalize(cross(up, light.direction));

  // Calculate light direction
  vec3 lightDir = normalize(light.position - fragPos);
  vec3 k = cross(lightDir, light.direction);
  vec3 m = normalize(cross(light.direction, k));

  float gamma = acos(dot(lightDir, light.direction));
  float l = tan(gamma);
  vec3 mm = m * l;

  xProj = dot(mm, xAxis);

  if (xProj < 0)
    xProj = cos(radians(180) - acos(xProj));

  yProj = dot(mm, up);
}

vec3 calcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 camDir,
  float shadow, out float diff, out float intensity)
{
  // Calculate light direction
  vec3 lightDir = normalize(light.position - fragPos);

  // Calculate diffuse intensity
  diff = max(dot(normal, lightDir), 0.0);

  // Calculate reflection vector and specular intensity
  vec3 halfwayDir = normalize(lightDir + camDir);
  float spec = pow(max(dot(normal, halfwayDir), 0.0), material.shininess * 4.0f);

  // Calculate attenuation
  float dist = length(light.position - fragPos);
  float attenuation = 1.0f /
    (light.constant + light.linear * dist + light.quadratic * (dist * dist));

  // Spotlight intensity
  if (rect)
  {
    if(projTexCoords.z > 0.0)
    {
      float xProj, yProj, tgA, tgB;
      float radius = 0.02;
      calcRectangleSpotlight(light, xProj, yProj, tgA, tgB);
      intensity = min(clamp((tgA - abs(xProj)) / radius, 0.0, 1.0),
                      clamp((tgB - abs(yProj)) / radius, 0.0, 1.0));
    }
  }
  else
  {
    float theta = dot(lightDir, normalize(-light.direction));
    float epsilon = cos(light.cutOff) - cos(light.outerCutOff);
    intensity = clamp((theta - cos(light.outerCutOff)) / epsilon, 0.0, 1.0);
  }

  // Calculate colors
  vec3 diffuseMix = mix(material.diffuse,
    vec3(texture(material.diffuseTex, texCoords)), material.diffMix);
  vec3 specularMix = mix(material.specular,
    vec3(texture(material.specularTex, texCoords)), material.specMix);

  vec3 ambient = light.ambient * diffuseMix;
  vec3 diffuse = light.diffuse * diff * diffuseMix;
  vec3 specular = light.specular * spec * specularMix;

  // Apply attenuation
  ambient *= attenuation;
  diffuse *= attenuation * intensity;
  specular *= attenuation * intensity;

  // Assemble output vector
  return (ambient + (1.0 - shadow) * (diffuse + specular)) * light.energy;
}

layout(index = 1)
subroutine (renderPassType)
void recordDepth()
{
}

layout(index = 2)
subroutine (renderPassType)
void normalRender()
{
  vec3 norm = normalize(normal);
  vec3 camDir = normalize(camPos - fragPos);

  float closestDepth, currentDepth, diff, intensity;
  float shadow = shadowCalculation(closestDepth, currentDepth);

  vec4 result = vec4(calcSpotLight(spotLight, norm, fragPos, camDir, shadow, diff, intensity), 1.0);

  if(projTexCoords.z > 0.0 && currentDepth < closestDepth)
    result += textureProj(projectTex, projTexCoords) * diff * intensity * 0.5;

  fragColor = result;
}

void main()
{
  renderPass();
}
