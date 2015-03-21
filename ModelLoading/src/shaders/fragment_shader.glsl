#version 430 core

in vec3 normal;
in vec3 fragPosition;

uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPosition;

void main()
{
  float ambientAmount = 0.1f;
  vec3 ambient = ambientAmount * lightColor;

  vec3 norm = normalize(normal);
  vec3 lightDirection = normalize(vec3(lightPosition - fragPosition));

  float diff = max(dot(norm, lightDirection), 0.0f);
  vec3 diffuse = diff * lightColor;

  vec3 result = (ambient + diffuse) * objectColor;

  gl_FragColor = vec4(result, 1.0f);
}
