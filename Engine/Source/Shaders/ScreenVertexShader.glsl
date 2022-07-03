#version 410 core
layout(location = 0) in vec2 POS;
layout(location = 1) in vec2 UVS;

out vec2 v_TexCoords;

void main()
{
    v_TexCoords = UVS;
    gl_Position = vec4(POS.xy, 0.0, 1.0);
}
