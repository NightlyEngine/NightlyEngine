#version 410 core
layout (location = 0) in vec3 POS;

void main()
{
    gl_Position = vec4(POS.xyz, 1.0);
}
