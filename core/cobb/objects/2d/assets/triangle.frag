#version 330 core

uniform vec4 color;

in vec3 pos;

out vec4 FragColor;

void main() {
    float heightColor = (pos.y + 0.25f) * 2.0f;
    //FragColor = vec4(heightColor * 0.8f, 1.0f - heightColor, 0, 1);
    //FragColor = vec4(0, heightColor * 0.5f, max(heightColor, 0.5f), 1);
    FragColor = color;
}