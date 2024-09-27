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
    float xnew = rotatedPoint.x * c - rotatedPoint.y * s;
    float ynew = rotatedPoint.x * s + rotatedPoint.y * c;
    return vec2(xnew, ynew) + origin;

}

void main() {

    vec2 point = vec2(aPos.x, aPos.y);
    point+=vec2((sin(time * 6.0f) / 3.0f), sin(time * 3.0f) / 3.0f);
    gl_Position = vec4(rotatePoint(vec2(-cos(time)/3.0f,-sin(time)/3.0f), time, point), aPos.z, 1.0f);
    color = aColor;
    TexCoord = aTexCoord;
}