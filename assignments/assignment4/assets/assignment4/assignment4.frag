#version 330 core

in vec4 pos;

out vec4 FragColor;


void main() {



    FragColor = vec4(pos.x, pos.y, 1.0f, 1.0f);
}