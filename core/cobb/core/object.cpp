//
// Created by cobble on 10/12/2024.
//

#include "object.h"

Object::Object() {
    exists = false;
}

Object::Object(vec3 pos, vec3 rot, vec3 sca) {
    exists = true;
    pos = pos;
    rot = rot;
    _scale = sca;
}

mat4 Object::scale(const float x, const float y, const float z)
{
    mat4 m = mat4(1);
    m[0][0] = x;
    m[1][1] = y;
    m[2][2] = z;
    return m;
}

mat4 Object::rotate(const float yaw, const float pitch, const float roll)
{
    const mat4 x = mat4(
            cos(yaw), 0, sin(yaw), 0,
            0, 1, 0, 0,
            -sin(yaw), 0, cos(yaw), 0,
            0, 0, 0, 1
    );
    const mat4 y = mat4(
            1, 0, 0, 0,
            0, cos(pitch), -sin(pitch), 0,
            0, sin(pitch), cos(pitch), 0,
            0, 0, 0, 1
    );
    const mat4 z = mat4(
            cos(roll), -sin(roll), 0, 0,
            sin(roll), cos(roll), 0, 0,
            0, 0, 1, 0,
            0, 0, 0, 1
    );
    return y * x * z;
}

mat4 Object::translate(const float x, const float y, const float z)
{
    mat4 m = mat4(1);
    m[3][0] = x;
    m[3][1] = y;
    m[3][2] = z;
    return m;
}
