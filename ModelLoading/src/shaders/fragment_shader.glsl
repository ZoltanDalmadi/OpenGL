#version 430 core

in vec3 normal;
in vec3 fragPosition;

uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPosition;
uniform vec3 cameraPosition;

void main()
{
  float ambientAmount = 0.1f;
  float diffuseAmount = 1.0f;
  float specularAmount = 0.5f;

  vec3 ambient = ambientAmount * lightColor;

  vec3 norm = normalize(normal);
  vec3 lightDirection = normalize(vec3(lightPosition - fragPosition));

  float diff = max(dot(norm, lightDirection), 0.0f);
  vec3 diffuse = diffuseAmount * diff * lightColor;

  vec3 cameraDirection = normalize(cameraPosition - fragPosition);
  vec3 reflectDirection = reflect(-lightDirection, norm);

  float spec = pow(max(dot(cameraDirection, reflectDirection), 0.0), 32);
  vec3 specular = specularAmount * spec * lightColor;

  vec3 result = (ambient + diffuse + specular) * objectColor;

  gl_FragColor = vec4(result, 1.0f);
}
