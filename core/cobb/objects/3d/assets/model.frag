#version 330 core

uniform vec4 color;

in vec3 pos;

out vec4 FragColor;

void main() {


    /*float fade = 0.05f;
    vec3 c = vec3((pos.x + 1.0f) * 0.5f, (pos.y + 1.0f) * 0.5f, (pos.z + 1.0f) * 0.5f);
    if(c.x > fade && c.x < 1.0f - fade && c.y > fade && c.y < 1.0f - fade) {
        FragColor = vec4(color.rgb, 0.5f);
    } else if(c.y > fade && c.y < 1.0f - fade && c.z > fade && c.z < 1.0f - fade) {
        FragColor = vec4(color.rgb, 0.5f);
    } else if(c.z > fade && c.z < 1.0f - fade && c.x > fade && c.x < 1.0f - fade) {
        FragColor = vec4(color.rgb, 0.5f);
    } else {
        FragColor = color;
    }*/
    FragColor = color;

}