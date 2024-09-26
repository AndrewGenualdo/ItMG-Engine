#version 330 core

in vec3 color;
in vec2 TexCoord;

out vec4 FragColor;

uniform sampler2D tex;
uniform sampler2D tex2;
uniform float time;

void main() {
    vec4 t1 = texture(tex, TexCoord);
    vec4 t2 = texture(tex2, TexCoord);
    FragColor = vec4(t1.rgb * t2.rgb * color, 1.0f);
    //FragColor = t2;
    //FragColor = mix(texture(tex, TexCoord).rgb, texture(tex2, TexCoord).rgb, 0.2);
    //FragColor = texture(tex, TexCoord);
}