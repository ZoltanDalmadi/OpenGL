#version 430 core

uniform int Detail;
in vec3 ControlPoints[];
uniform mat4 modelViewProjectionMatrix;

vec3 evaluateBezierPosition( float t )
{
    vec3 p;
    float OneMinusT = 1.0 - t;
    float b0 = OneMinusT*OneMinusT*OneMinusT;
    float b1 = 3.0*t*OneMinusT*OneMinusT;
    float b2 = 3.0*t*t*OneMinusT;
    float b3 = t*t*t;
    return b0*ControlPoints[0] + b1*ControlPoints[1] + b2*ControlPoints[2] + b3*ControlPoints[3];
}

void main()
{
    float OneOverDetail = 1.0 / float(Detail-1.0);
    for( int i=0; i<Detail; i++ )
    {
        float t = i * OneOverDetail;
        vec3 p = evaluateBezierPosition( t );
        gl_Position = modelViewProjectionMatrix * vec4( p.xyz, 1.0 );
        EmitVertex();
    }
 
    EndPrimitive();
}
