#version 430
layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

in vec2 texCoords[];

out vec2 TexCoords; 
//out float Time;

uniform float time;

vec4 explode(vec4 position, vec3 normal)
{
    float magnitude = 2.0f;
    vec3 direction = normal * time * magnitude; 
    //return position + vec4(vec3(0.0f,0.0f,0.0f), 0.0f);
	return position + vec4(direction, 0.0f);
}

vec3 GetNormal()
{
    vec3 a = vec3(gl_in[0].gl_Position) - vec3(gl_in[1].gl_Position);
    vec3 b = vec3(gl_in[2].gl_Position) - vec3(gl_in[1].gl_Position);
    return normalize(cross(a, b));
}

void main() {    
	//Time = time;
    vec3 normal = GetNormal();

    gl_Position = explode(gl_in[0].gl_Position, normal);
    TexCoords = texCoords[0];
    EmitVertex();
    gl_Position = explode(gl_in[1].gl_Position, normal);
    TexCoords = texCoords[1];
    EmitVertex();
    gl_Position = explode(gl_in[2].gl_Position, normal);
    TexCoords = texCoords[2];
    EmitVertex();
    EndPrimitive();
}