#version 330 core

uniform sampler2D tex;
uniform vec4 color;

in vec2 TexCoords;

out vec4 FragColor;



void main()
{
    FragColor = texture(tex, TexCoords) * color;
}