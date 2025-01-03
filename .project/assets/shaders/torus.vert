#version 330

layout (location = 0) in vec3 aPosition;


uniform mat4 modelMatrix;
uniform mat4 view;
uniform mat4 projection;

uniform vec3 offset;
uniform vec3 scale;

void main()
{
    vec3 scaledPosition = (aPosition * offset);
    gl_Position = view * modelMatrix * vec4(scaledPosition, 1.0f);

}