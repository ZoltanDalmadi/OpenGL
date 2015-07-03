#version 430 core
layout (points) in;
layout (triangle_strip, max_vertices = 4) out;

uniform mat4 MVP;

void main() {    
  		
	gl_Position = MVP * (gl_in[0].gl_Position + vec4(-0.05, 0.01, -0.1, 0.0));
    EmitVertex();
	gl_Position = MVP * (gl_in[0].gl_Position + vec4(-0.05, 0.01, 0.1, 0.0));
    EmitVertex();
	gl_Position = MVP * (gl_in[0].gl_Position + vec4(0.05, 0.01, -0.1, 0.0));
    EmitVertex();
	gl_Position = MVP * (gl_in[0].gl_Position + vec4(0.05, 0.01, 0.1, 0.0));
    EmitVertex();


	EndPrimitive();	
} 


