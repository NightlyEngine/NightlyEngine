#version 410 core
out vec4 f_Color;

in vec2 v_TexCoords;

uniform sampler2D u_RenderTexture;

void main()
{
    vec4 color = texture(u_RenderTexture, v_TexCoords);
    f_Color = color;
}
