#version 330 core

in vec4 pos;
in vec2 TexCoord;

out vec4 FragColor;

uniform bool gameOver;
uniform bool lock;
uniform sampler2D tex;

void main() {

    FragColor = texture(tex, TexCoord);
    if(lock) {
        FragColor*=vec4(0.5f, 0.5f, 0.5f, 1.0f);
    }
    if(gameOver) {
        FragColor*=vec4(1.0f, 0.5f, 0.5f, 1.0f);
    }

}