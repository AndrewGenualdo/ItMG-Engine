#version 330 core

struct Light {
    vec3 pos;
    vec3 color;
};

uniform Light light;

in vec2 TexCoord;
in vec3 Normal;

out vec4 FragColor;

uniform sampler2D tex;

void main() {
    FragColor = texture(tex, TexCoord);
    //FragColor = vec4(Normal, 1.0f);
}