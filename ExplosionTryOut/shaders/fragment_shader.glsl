#version 400

in float Transp;
uniform sampler2D ParticleTex;
uniform vec3 camPos;

layout ( location = 0 ) out vec4 FragColor;

void main()
{
	vec3 camDir = normalize(camPos - fragPos);
    FragColor = texture(ParticleTex, gl_PointCoord);
    FragColor.a *= Transp;
}
