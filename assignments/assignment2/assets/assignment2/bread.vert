#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoord;

out vec3 color;
out vec2 TexCoord;

uniform sampler2D tex;
uniform float time;

vec2 rotatePoint(vec2 origin, float angleInRad, vec2 point) {

    vec2 rotatedPoint = point - origin;
    float s = sin(angleInRad);
    float c = cos(angleInRad);
    return vec2(rotatedPoint.x * c - rotatedPoint.y * s, rotatedPoint.x * s + rotatedPoint.y * c) + origin;

}

void main() {

    vec2 point = vec2(aPos.x, aPos.y);
    point+=vec2((sin(time * 5.8f) / 2.9f), sin(time * 3.2f) / 2.8f);
    gl_Position = vec4(rotatePoint(vec2(-cos(time * 2.7f)/3.1f,-sin(time * 4.1f)/3.0f), (sin(time / 1.1f) + 1) * abs(cos(time / 1.2f)) * 10.0f, point), aPos.z, 1.0f);
    color = aColor;
    TexCoord = aTexCoord;
}