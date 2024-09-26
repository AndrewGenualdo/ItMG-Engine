#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoord;

out vec3 color;
out vec2 TexCoord;

uniform sampler2D tex;
uniform float time;

void main() {
    gl_Position = vec4(aPos.x + ((cos(3 * time + aPos.x + aPos.y) + 1) / 2) - 0.5f, aPos.y + ((sin(3 * time + aPos.x + aPos.y) + 1) / 2) - 0.5f, aPos.z, 1.0f);
    color = aColor;
    TexCoord = aTexCoord;
}