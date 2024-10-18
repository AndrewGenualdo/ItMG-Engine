#version 330 core

uniform vec4 color;
uniform vec4 color2;
uniform vec4 color3;

in vec3 pos;

out vec4 FragColor;

void main() {
    FragColor = mix(mix(color, color2, pos.x), color3, pos.z);
}