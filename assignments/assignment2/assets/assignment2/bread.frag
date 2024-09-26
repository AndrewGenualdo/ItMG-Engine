#version 330 core

in vec3 color;
in vec2 TexCoord;

out vec4 FragColor;

uniform sampler2D tex;

void main() {

    vec4 t = texture(tex, TexCoord);
    FragColor = t.a == 0 ? t : vec4(t.rgb, 0.7f);
}