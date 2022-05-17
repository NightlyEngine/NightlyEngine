#version 410 core
out vec4 OutColor;

in vec2 TexCoords;

uniform sampler2D renderTexture;

void main()
{
    vec4 color = texture(renderTexture, TexCoords);
    OutColor = color;
}
