#version 330 core
layout (location = 0) in vec3 aPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

out vec3 color;

uniform vec3 lightColor;

void main() {
    color = lightColor;
    gl_Position = proj * view * model * vec4(aPos, 1.0f);
}