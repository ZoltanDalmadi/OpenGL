#version 430 core

in vec3 normal;
in vec3 fragPos;
in vec2 texCoords;

uniform vec3 camPos;

struct Material
{
  sampler2D diffuse;
  sampler2D specular;
  float shininess;
};
uniform Material material;

struct PointLight
{
  vec3 position;

  float constant;
  float linear;
  float quadratic;

  vec3 ambient;
  vec3 diffuse;
  vec3 specular;

  float energy;
};
uniform PointLight pointLight;

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
uniform SpotLight spotLight[4];

vec3 calcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 camDir)
{
  // Calculate light direction
  vec3 lightDir = normalize(light.position - fragPos);

  // Calculate diffuse intensity
  float diff = max(dot(normal, lightDir), 0.0);

  // Calculate reflection vector and specular intensity
  vec3 reflectDir = reflect(-lightDir, normal);
  float spec = pow(max(dot(camDir, reflectDir), 0.0), material.shininess);

  // Calculate attenuation
  float dist = length(light.position - fragPos);
  float attenuation = 1.0f / (light.constant + light.linear * dist + light.quadratic * (dist * dist));

  // Calculate colors
  vec3 ambient = light.ambient * vec3(texture(material.diffuse, texCoords));
  vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, texCoords));
  vec3 specular = light.specular * spec * vec3(texture(material.specular, texCoords));

  // Apply attenuation
  ambient *= attenuation;
  diffuse *= attenuation;
  specular *= attenuation;

  // Assemble output vector
  return (ambient + diffuse + specular) * light.energy;
}

vec3 calcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 camDir)
{
  // Calculate light direction
  vec3 lightDir = normalize(light.position - fragPos);

  // Calculate diffuse intensity
  float diff = max(dot(normal, lightDir), 0.0);

  // Calculate reflection vector and specular intensity
  vec3 reflectDir = reflect(-lightDir, normal);
  float spec = pow(max(dot(camDir, reflectDir), 0.0), material.shininess);

  // Calculate attenuation
  float dist = length(light.position - fragPos);
  float attenuation = 1.0f / (light.constant + light.linear * dist + light.quadratic * (dist * dist));

  // Spotlight intensity
  float theta = dot(lightDir, normalize(-light.direction));
  float epsilon = light.cutOff - light.outerCutOff;
  float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);

  // Calculate colors
  vec3 ambient = light.ambient * vec3(texture(material.diffuse, texCoords));
  vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, texCoords));
  vec3 specular = light.specular * spec * vec3(texture(material.specular, texCoords));

  // Apply attenuation
  ambient *= attenuation * intensity;
  diffuse *= attenuation * intensity;
  specular *= attenuation * intensity;

  // Assemble output vector
  return (ambient + diffuse + specular) * light.energy;
}

void main()
{
  vec3 norm = normalize(normal);
  vec3 camDir = normalize(camPos - fragPos);

  vec3 result;

  result += calcPointLight(pointLight, norm, fragPos, camDir);

  for(int i = 0; i < 4; i++)
    result += calcSpotLight(spotLight[i], norm, fragPos, camDir);

  gl_FragColor = vec4(result, 1.0f);
}
