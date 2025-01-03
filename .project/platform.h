//
// Created by cobble on 12/27/2024.
//

#ifndef ENGINE_2_PLATFORM_H
#define ENGINE_2_PLATFORM_H

#include "hitbox.h"

class Platform : public Hitbox {
public:
    bool solid;

    Platform(float x, float y, float width, float height, bool solid = false) : Hitbox() {
        this->x = x;
        this->y = y;
        this->w = width;
        this->h = height;
        this->solid = solid;
    }

    std::string to_string() {
        return std::to_string(x) + " " + std::to_string(y) + " " + std::to_string(w) + " " + std::to_string(h) + " " + (solid ? "T" : "F");
    }
};

#endif //ENGINE_2_PLATFORM_H
