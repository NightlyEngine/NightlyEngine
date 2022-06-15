#version 450

layout(location = 0) in vec3 v_Color;
layout(location = 0) out vec4 o_Color;

void main()
{
    vec3 gammaCorrected = pow(v_Color, vec3(1.0f / 2.2f));
    o_Color = vec4(gammaCorrected, 1.0);
}
