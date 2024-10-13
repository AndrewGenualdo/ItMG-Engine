//
// Created by cobble on 10/12/2024.
//


#ifndef EWRENDER_OBJECT_HPP
#define EWRENDER_OBJECT_HPP

#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


using namespace glm;

class Object
{
public:
    vec3 position = vec3(0, 0, 0);
    vec3 rotation = vec3(0, 0, 0);
    vec3 scale = vec3(1, 1, 1);
    bool exists = false;

    Object();
    Object(vec3 pos, vec3 rot, vec3 sca);
};


#endif //EWRENDER_OBJECT_HPP
