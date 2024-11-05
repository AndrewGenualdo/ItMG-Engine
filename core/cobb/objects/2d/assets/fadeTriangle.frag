#version 330 core

uniform vec4 color;
uniform vec4 color2;
uniform vec4 color3;

in vec4 vertexColor;

out vec4 FragColor;

void main() {
    FragColor = vertexColor;
}