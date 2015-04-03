#version 430 core

in vec3 normal;
in vec3 fragPos;
in vec2 texCoords;

uniform sampler2D diffuse;
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

vec3 calcPointLight(PointLight light, vec3 normal, vec3 fragPos)
{
  vec3 lightDir = normalize(light.position - fragPos);

  float diff = max(dot(normal, lightDir), 0.0);

  float dist = length(light.position - fragPos);
  float attenuation = 1.0f / (light.constant + light.linear * dist + light.quadratic * (dist * dist));

  vec3 ambient = light.ambient * vec3(texture(diffuse, texCoords));
  vec3 diffuse = light.diffuse * diff * vec3(texture(diffuse, texCoords));
  ambient *= attenuation;
  diffuse *= attenuation;

  return (ambient + diffuse) * light.energy;
}

void main()
{
  vec3 norm = normalize(normal);
  gl_FragColor = vec4(calcPointLight(pointLight, norm, fragPos), 1.0f);
}
