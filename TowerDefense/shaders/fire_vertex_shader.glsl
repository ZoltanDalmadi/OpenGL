#version 400

layout (location = 0) in vec3 VertexInitVel; // Particle initial velocity
layout (location = 1) in float StartTime;    // Particle "birth" time

out float Transp;  // Transparency of the particle

uniform float Time;  // Animation time
uniform vec3 Gravity = vec3(0.0,0.0,0.0);  // world coords
uniform float ParticleLifetime;  // Max particle lifetime

uniform mat4 MVP;
uniform mat4 model;

void main()
{
    // Assume the initial position is (0,0,0).
	vec4 pos = vec4(0.0f);
    //pos = vec4(VertexInitVel,1.0);
	//pos = model*pos;
	vec4 pos2;
    Transp = 0.0;

    // Particle dosen't exist until the start time
    if( Time > StartTime ) {
        float t = Time - StartTime;

        if( t < ParticleLifetime ) {
            pos = vec4((VertexInitVel) * t + Gravity * t * t, 1.0);
			pos = model * pos;
            Transp = 1.0 - t / ParticleLifetime;
        }
    }

    // Draw at the current position
    gl_Position = MVP * pos;
}
