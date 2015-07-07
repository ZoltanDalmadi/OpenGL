#version 430
layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

in vec3 normalToGeom[];
in vec3 fragPosToGeom[];

out vec3 normal;
out vec3 fragPos;

uniform float explosionProgress;

vec4 explode(vec4 position, vec3 normal)
{
  float magnitude = 2.0f;
  vec3 direction = normal * explosionProgress * magnitude;
  return position + vec4(direction, 1.0f);
}

vec3 getFaceNormal()
{
  vec3 a = vec3(gl_in[0].gl_Position) - vec3(gl_in[1].gl_Position);
  vec3 b = vec3(gl_in[2].gl_Position) - vec3(gl_in[1].gl_Position);
  return normalize(cross(a, b));
}

void main()
{
  normal = normalToGeom[0];
  fragPos = fragPosToGeom[0];
  vec3 faceNormal = getFaceNormal();

  gl_Position = explode(gl_in[0].gl_Position, faceNormal);
  EmitVertex();
  gl_Position = explode(gl_in[1].gl_Position, faceNormal);
  EmitVertex();
  gl_Position = explode(gl_in[2].gl_Position, faceNormal);
  EmitVertex();

  EndPrimitive();
}
