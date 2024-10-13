//
// Created by cobble on 10/12/2024.
//

#include "object.hpp"

Object::Object() {
    exists = false;
}

Object::Object(vec3 pos, vec3 rot, vec3 sca) {
    exists = true;
    position = pos;
    rotation = rot;
    scale = sca;
}
