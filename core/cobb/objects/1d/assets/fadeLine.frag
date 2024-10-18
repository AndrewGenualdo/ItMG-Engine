#version 330 core

uniform vec4 color;
uniform vec4 color2;

in float dist;

out vec4 FragColor;

void main() {
    FragColor = mix(color, color2, dist);
}