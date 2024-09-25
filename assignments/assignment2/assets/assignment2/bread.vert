#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoord;

out vec3 color;
out vec2 TexCoord;

uniform sampler2D tex;
uniform float time;

void main() {
    gl_Position = vec4(aPos.x, aPos.y + abs(sin(time * 3.0f)) / 3.0f, aPos.z, 1.0f);
    color = aColor;
    TexCoord = aTexCoord;
}