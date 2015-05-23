#version 430 core

in vec3 normal;
in vec3 fragPos;
in vec2 texCoords;
in vec4 fragPosinLightSpace;
in vec4 projTexCoords;

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

subroutine void renderPassType();
subroutine uniform renderPassType renderPass;

float shadowCalculation()
{
  // perform perspective divide
  vec3 projCoords = fragPosinLightSpace.xyz / fragPosinLightSpace.w;

  // Transform to [0, 1] range
  projCoords = projCoords * 0.5 + 0.5;

  // Get closest depth value from light's perspective
  float closestDepth = texture(shadowMap, projCoords.xy).r;

  // Get depth of current fragment from light's perspective
  float currentDepth = projCoords.z;

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

  shadow /= 16.0;

  // Keep the shadow at 0.0 when outside the far_plane region of the light's frustum.
  if(projCoords.z > 1.0)
      shadow = 0.0;

  return shadow * 1.5;
}

vec3 calcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 camDir)
{
  // Calculate light direction
  vec3 lightDir = normalize(light.position - fragPos);

  // Calculate diffuse intensity
  float diff = max(dot(normal, lightDir), 0.0);

  // Calculate reflection vector and specular intensity
  vec3 halfwayDir = normalize(lightDir + camDir);
  float spec = pow(max(dot(normal, halfwayDir), 0.0), material.shininess * 4.0f);

  // Calculate attenuation
  float dist = length(light.position - fragPos);
  float attenuation = 1.0f /
    (light.constant + light.linear * dist + light.quadratic * (dist * dist));

  // Spotlight intensity
  float theta = dot(lightDir, normalize(-light.direction));
  float epsilon = light.cutOff - light.outerCutOff;
  float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);

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

  // Calculate shadow
  float shadow = shadowCalculation();
  //shadow = min(shadow, 0.75);

  // Assemble output vector
  return (ambient + (1.0 - shadow) * (diffuse + specular)) * light.energy;
}

subroutine (renderPassType)
void normalRender()
{
  vec3 norm = normalize(normal);
  vec3 camDir = normalize(camPos - fragPos);

  vec3 result = calcSpotLight(spotLight, norm, fragPos, camDir);

  vec4 projTexColor = vec4(0.0);

  if(projTexCoords.z > 0.0)
    projTexColor = textureProj(projectTex, projTexCoords);

  gl_FragColor = vec4(result, 1.0) + projTexColor * 0.2;
}

subroutine (renderPassType)
void recordDepth()
{
}

void main()
{
  renderPass();
}
