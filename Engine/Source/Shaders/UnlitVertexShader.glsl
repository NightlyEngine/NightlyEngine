#version 410 core
layout (location = 0) in vec3 POS;

uniform mat4 u_Model;
uniform mat4 u_ProjectionView;

void main()
{
    vec4 vertexPos = u_ProjectionView * u_Model * vec4(POS, 1.0);
    gl_Position = vertexPos;
}
