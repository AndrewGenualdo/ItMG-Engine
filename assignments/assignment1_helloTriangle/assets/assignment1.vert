#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec4 aColor;

uniform float time;

out vec3 pos;
out vec4 color;

vec2 rotatePoint(vec2 origin, float angleInRad, vec2 point) {

    vec2 rotatedPoint = point - origin;
    float s = sin(angleInRad);
    float c = cos(angleInRad);
    float xnew = rotatedPoint.x * c - rotatedPoint.y * s;
    float ynew = rotatedPoint.x * s + rotatedPoint.y * c;
    return vec2(xnew, ynew) + origin;

}

void main() {
    pos = aPos;
    color = aColor;

    vec2 centerOfTriangle = vec2(0.0f, -0.167f);
    float adjustedTime = time * 2.0f * 3.141592653589f / 3.0f;
    float cosAdjTime = cos(adjustedTime);
    float sinAdjTime = sin(adjustedTime);
    float scale = ((sin(time * 2.5f + aPos.x + aPos.y) + 1) / 2.0f) / 2.0f + 0.2f;
    vec2 rotatedPoint = rotatePoint(centerOfTriangle, adjustedTime, vec2(aPos.x * scale, aPos.y * scale));
    gl_Position = vec4(rotatedPoint.x, rotatedPoint.y, aPos.z, 1.0f);
}