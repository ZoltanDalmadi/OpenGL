#version 430 core

in vec3 normal;
in vec3 fragPos;
in vec2 texCoords;

uniform vec3 camPos;

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

  // Assemble output vector
  return (ambient + diffuse + specular) * light.energy;
}

void main()
{
  vec3 norm = normalize(normal);
  vec3 camDir = normalize(camPos - fragPos);

  vec3 result = calcSpotLight(spotLight, norm, fragPos, camDir);

  gl_FragColor = vec4(result, 1.0f);
}
