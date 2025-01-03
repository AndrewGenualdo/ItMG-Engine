#version 330 core
//layout (location = 0) in vec4 vertex; // <vec2 position, vec2 texCoords>
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec4 aColor;
layout (location = 2) in vec2 aTexCoord;

out vec2 TexCoords;

uniform mat4 model;
uniform mat4 proj;

void main()
{
    TexCoords = aTexCoord;
    gl_Position = proj * model * vec4(aPos, 1.0);
}