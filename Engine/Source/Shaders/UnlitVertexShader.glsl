#version 410 core
layout (location = 0) in vec3 POS;

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProjection;

void main()
{
    vec4 vertexPos = uProjection * uView * uModel * vec4(POS, 1.0);
    gl_Position = vertexPos;
}
