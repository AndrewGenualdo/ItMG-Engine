#version 330 core

in vec4 pos;
in vec2 TexCoord;
in vec3 block;

out vec4 FragColor;

uniform bool lock;
uniform sampler2D tex;
uniform bool hover;

void main() {

    vec4 color = texture(tex, TexCoord);

    float mult = (block.y + 5.0f) / 10.0f;
    color *= vec4(mult, mult, mult, 1.0f);

    if(hover) {
        float borderWidth = 0.01f;
        if(TexCoord.x < borderWidth) color = mix(color, vec4(1), smoothstep(0, 1, 1.0f - (TexCoord.x / borderWidth)));
        if(TexCoord.y < borderWidth) color = mix(color, vec4(1), smoothstep(0, 1, 1.0f - (TexCoord.y / borderWidth)));
        if(TexCoord.x > 1.0f - borderWidth) color = mix(color, vec4(1), smoothstep(0, 1, (TexCoord.x - (1.0f - borderWidth)) * (1.0f / borderWidth)));
        if(TexCoord.y > 1.0f - borderWidth) color = mix(color, vec4(1), smoothstep(0, 1, (TexCoord.y - (1.0f - borderWidth)) * (1.0f / borderWidth)));
    }


    if(lock) color*=vec4(0.5f, 0.5f, 0.5f, 1.0f);

    FragColor = color;
}