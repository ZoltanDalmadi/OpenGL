#version 430 core

in vec3 normal;
in vec3 fragPos;
in vec2 texCoords;
//in float Time;

out vec4 fragColor;

uniform vec3 camPos;

struct Material
{
  vec3 diffuse;
  //sampler2D diffuseTex;
  //float diffMix;
  vec3 specular;
  //sampler2D specularTex;
  //float specMix;
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
  vec3 ambient = light.ambient * material.diffuse;
  vec3 diffuse = light.diffuse * diff * material.diffuse;
  vec3 specular = light.specular * spec * material.specular;

  // Apply attenuation
  // ambient *= attenuation;
  diffuse *= attenuation;
  specular *= attenuation;

  // Assemble output vector
  return (ambient + diffuse + specular) * light.energy;
}

void main()
{
  vec3 norm = normalize(normal);
  vec3 camDir = normalize(camPos - fragPos);

  fragColor = vec4(calcPointLight(pointLight, norm, fragPos, camDir),
			 1.0f);
}
