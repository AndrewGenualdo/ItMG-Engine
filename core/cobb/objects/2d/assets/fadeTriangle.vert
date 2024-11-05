#version 330 core
layout (location = 0) in vec3 aPos;

uniform mat4 model;
uniform mat4 viewProj;
uniform vec4 color;
uniform vec4 color2;
uniform vec4 color3;

out vec4 vertexColor;

void main() {
    vertexColor = vec4(1);
    if(aPos == vec3(0.0f,0.0f,0.0f)) {
        vertexColor = color;
    } else if(aPos == vec3(1.0f,0.0f,0.0f)) {
        vertexColor = color2;
    } else if(aPos == vec3(0.0f, 1.0f, 0.0f)){
        vertexColor = color3;
    }
    gl_Position = viewProj * model * vec4(aPos, 1);
}