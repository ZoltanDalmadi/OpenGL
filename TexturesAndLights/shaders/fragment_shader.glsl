#version 430 core

in vec3 normalVec;
in vec3 fragPos;
in vec2 texCoord;

uniform vec3 camPos;

struct Material
{
  sampler2D diffuse1;
  sampler2D diffuse2;
  vec3 specular;
  float shininess;
};
uniform Material material;

struct DirectionalLight
{
  vec3 direction;
  vec3 ambient;
  vec3 diffuse;
  vec3 specular;

  float energy;
};
uniform DirectionalLight dirLight;

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
uniform SpotLight spotLight;

vec3 calcDirLight(DirectionalLight light, vec3 normal, vec3 camDir);
vec3 calcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 camDir);
vec3 calcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 camDir);

void main()
{
  vec3 norm = normalize(normalVec);
  vec3 camDir = normalize(camPos - fragPos);
  /* vec3 result = calcDirLight(dirLight, norm, camDir); */
  vec3 result = calcPointLight(pointLight, norm, fragPos, camDir);
  /* vec3 result = calcSpotLight(spotLight, norm, fragPos, camDir); */
  gl_FragColor = vec4(result, 1.0f);
}

vec3 calcDirLight(DirectionalLight light, vec3 normal, vec3 camDir)
{
  vec3 lightDir = normalize(-light.direction);

  float diff = max(dot(normal, lightDir), 0.0);

  vec3 reflectDir = reflect(-lightDir, normal);
  float spec = pow(max(dot(camDir, reflectDir), 0.0), material.shininess);

  vec4 mixture = mix(texture(material.diffuse1, texCoord), texture(material.diffuse2, texCoord), 0.2);
  /* vec3 ambient  = light.ambient  * vec3(texture(material.diffuse, texCoord)); */
  vec3 ambient  = light.ambient  * vec3(mixture);
  /* vec3 diffuse  = light.diffuse  * diff * vec3(texture(material.diffuse, texCoord)); */
  vec3 diffuse  = light.diffuse  * diff * vec3(mixture);
  vec3 specular = light.specular * spec * material.specular;
  return light.energy * ambient + (diffuse + specular);
}

vec3 calcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 camDir)
{
  vec3 lightDir = normalize(light.position - fragPos);

  float diff = max(dot(normal, lightDir), 0.0);

  vec3 reflectDir = reflect(-lightDir, normal);
  float spec = pow(max(dot(camDir, reflectDir), 0.0), material.shininess);
  // Attenuation
  float dist = length(light.position - fragPos);
  float attenuation = 1.0f / (light.constant + light.linear * dist + light.quadratic * (dist * dist));

  vec4 mixture = mix(texture(material.diffuse1, texCoord), texture(material.diffuse2, texCoord), 0.2);
  /* vec3 ambient = light.ambient * vec3(texture(material.diffuse, texCoord)); */
  vec3 ambient = light.ambient * vec3(mixture);
  /* vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, texCoord)); */
  vec3 diffuse = light.diffuse * diff * vec3(mixture);
  vec3 specular = light.specular * spec * material.specular;
  ambient *= attenuation;
  diffuse *= attenuation;
  specular *= attenuation;
  return (ambient + diffuse + specular) * light.energy;
}

vec3 calcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 camDir)
{
  vec3 lightDir = normalize(light.position - fragPos);

  float diff = max(dot(normal, lightDir), 0.0);

  vec3 reflectDir = reflect(-lightDir, normal);
  float spec = pow(max(dot(camDir, reflectDir), 0.0), material.shininess);
  // Attenuation
  float dist = length(light.position - fragPos);
  float attenuation = 1.0f / (light.constant + light.linear * dist + light.quadratic * (dist * dist));
  // Spotlight intensity
  float theta = dot(lightDir, normalize(-light.direction));
  float epsilon = light.cutOff - light.outerCutOff;
  float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);

  vec4 mixture = mix(texture(material.diffuse1, texCoord), texture(material.diffuse2, texCoord), 0.2);
  /* vec3 ambient = light.ambient * vec3(texture(material.diffuse, texCoord)); */
  vec3 ambient = light.ambient * vec3(mixture);
  /* vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, texCoord)); */
  vec3 diffuse = light.diffuse * diff * vec3(mixture);
  vec3 specular = light.specular * spec * material.specular;
  ambient *= attenuation * intensity;
  diffuse *= attenuation * intensity;
  specular *= attenuation * intensity;
  return (ambient + diffuse + specular) * light.energy;
}
