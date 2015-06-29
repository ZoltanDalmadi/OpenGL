#version 330 core
layout (lines_adjacency) in;
layout (line_strip, max_vertices = 100) out;

uniform mat4 MVP;

vec3 evaluateBezierPosition( vec3 ControlPoints[4],  float t )
{
    vec3 p;
    float OneMinusT = 1.0 - t;
    float b0 = OneMinusT*OneMinusT*OneMinusT;
    float b1 = 3.0*t*OneMinusT*OneMinusT;
    float b2 = 3.0*t*t*OneMinusT;
    float b3 = t*t*t;
    return b0*ControlPoints[0] + b1*ControlPoints[1] + b2*ControlPoints[2] + b3*ControlPoints[3];
}

void main() {    

	vec3 cPoints[4];

    cPoints[0] = vec3(gl_in[0].gl_Position); 
    cPoints[1] = vec3(gl_in[1].gl_Position); 
    cPoints[2] = vec3(gl_in[2].gl_Position); 
    cPoints[3] = vec3(gl_in[3].gl_Position);

    float OneOverDetail = 1.0 / float(100.0 - 1.0);
    for( int i = 0; i < 100; i++ )
    {
        float t = i * OneOverDetail;
        vec3 p = evaluateBezierPosition( cPoints, t );
        gl_Position = MVP * vec4( p, 1.0 );
        EmitVertex();
    }

	EndPrimitive();	
}  