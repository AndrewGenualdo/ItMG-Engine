#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

out vec2 TexCoord;
out vec3 Normal;

uniform mat4 viewProj;
uniform mat4 model;

void main() {
    TexCoord = aTexCoord;
    Normal = aNormal;
    gl_Position = viewProj * model * vec4(aPos, 1.0f);
}