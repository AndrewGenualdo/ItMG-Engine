//
// Created by cobble on 12/30/2024.
//

#ifndef ENGINE_2_HITBOX_H
#define ENGINE_2_HITBOX_H

#include "cobb/misc/texture2d.h"

using namespace cobb;

class Hitbox {
public:
    static Texture2d texture;

    float x, y, w, h;

    Hitbox() = default;

    Hitbox(float x, float y, float w, float h);

    bool isColliding(Hitbox &hitbox) const;

    void setPos(float newX, float newY);

    void draw(vec4 color) const;
};

#endif //ENGINE_2_HITBOX_H
