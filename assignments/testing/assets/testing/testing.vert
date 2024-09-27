#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoord;

out vec3 color;
out vec3 screenPos;

uniform float time;
uniform sampler2D tex;
uniform vec2 dims;

void main() {
    screenPos = aPos;
    gl_Position = vec4(aPos, 1.0f);
    color = aColor;
}