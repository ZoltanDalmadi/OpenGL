#version 430 core

in vec3 normal;
in vec3 fragPosition;

uniform vec3 cameraPosition;

struct Material
{
  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
  float shininess;
};
uniform Material material;

struct Light
{
  vec3 position;
  vec3 color;
};
uniform Light light;

void main()
{
  vec3 ambient = material.ambient * light.color;

  vec3 norm = normalize(normal);
  vec3 lightDirection = normalize(light.position - fragPosition);

  float diff = max(dot(norm, lightDirection), 0.0f);
  vec3 diffuse = light.color * (diff * material.diffuse);

  vec3 cameraDirection = normalize(cameraPosition - fragPosition);
  vec3 reflectDirection = reflect(-lightDirection, norm);

  float spec = pow(max(dot(cameraDirection, reflectDirection), 0.0), material.shininess);
  vec3 specular = light.color * (spec * material.specular);

  vec3 result = ambient + diffuse + specular;

  gl_FragColor = vec4(result, 1.0f);
}
