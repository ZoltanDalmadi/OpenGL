#version 430 core

uniform int Detail;
uniform vec3 ControlPoints[];
uniform mat4 modelViewProjectionMatrix;

vec3 evaluateBezierPosition( vec3 v[4], float t )
{
    vec3 p;
    float OneMinusT = 1.0 - t;
    float b0 = OneMinusT*OneMinusT*OneMinusT;
    float b1 = 3.0*t*OneMinusT*OneMinusT;
    float b2 = 3.0*t*t*OneMinusT;
    float b3 = t*t*t;
    return b0*v[0] + b1*v[1] + b2*v[2] + b3*v[3];
}

void main()
{
    float OneOverDetail = 1.0 / float(g_Detail-1.0);
    for( int i=0; i<g_Detail; i++ )
    {
        float t = i * OneOverDetail;
        vec3 p = evaluateBezierPosition( ControlPoints, t );
        gl_Position = modelViewProjectionMatrix * vec4( p.xyz, 1.0 );
        EmitVertex();
    }
 
    EndPrimitive();
}
