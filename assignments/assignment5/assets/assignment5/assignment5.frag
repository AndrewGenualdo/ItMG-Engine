#version 330 core

struct Light {
    vec3 pos;
    vec4 color;
};

uniform Light light;
uniform bool lock;
uniform sampler2D tex;

in vec3 FragPos;
in vec2 TexCoord;
in vec3 Normals;


out vec4 FragColor;




void main() {



    //lighting variables
    float ambientStrength = 0.1f;
    vec4 ambient = ambientStrength * light.color;
    vec4 result = texture(tex, TexCoord) + ambient;
    FragColor = result;//vec4(abs(normalize(Normals)), 1.0f);
    if(lock) {
        FragColor*=vec4(0.5f, 0.5f, 0.5f, 1.0f);
    }

}