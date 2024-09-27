#version 330 core

in vec3 color;
in vec2 TexCoord;

out vec4 FragColor;

uniform sampler2D tex;
uniform sampler2D tex2;
uniform float time;

void main() {
    vec4 t1 = texture(tex, TexCoord * 10.0f);
    vec4 t2 = texture(tex2, TexCoord * 10.0f);
    FragColor = vec4(mix(t1.rgb, t2.rgb, (sin(time) + 1) * 0.25f + 0.25f) * color, 1.0f);
    //FragColor = vec4(t1.rgb * t2.rgb * color, 1.0f);

}