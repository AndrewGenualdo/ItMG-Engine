//
// Created by cobble on 10/12/2024.
//


#ifndef EWRENDER_OBJECT_HPP
#define EWRENDER_OBJECT_HPP

#pragma once

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"


using namespace glm;

class Object
{
public:
    vec3 pos = vec3(0, 0, 0);
    vec3 rot = vec3(0, 0, 0);
    vec3 _scale = vec3(1, 1, 1);
    bool exists = false;

    Object();
    Object(vec3 pos, vec3 rot, vec3 sca);

    static mat4 scale(const float x, const float y, const float z);
    static mat4 rotate(const float yaw, const float pitch, const float roll);
    static mat4 translate(const float x, const float y, const float z);
};


#endif //EWRENDER_OBJECT_HPP
