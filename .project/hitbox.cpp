//
// Created by cobble on 12/30/2024.
//

#include "hitbox.h"

Texture2d Hitbox::texture = Texture2d();

Hitbox::Hitbox(float x, float y, float w, float h) {
    {
        if(texture.getPath().empty()) texture = Texture2d("assets/textures/blank.png");
        this->x = x;
        this->y = y;
        this->w = w;
        this->h = h;
    }
}

bool Hitbox::isColliding(Hitbox &hitbox) const  {
    return x + w >= hitbox.x && hitbox.x + hitbox.w >= x && y + h >= hitbox.y && hitbox.y + hitbox.h >= y;
}

void Hitbox::setPos(float newX, float newY) {
    this->x = newX;
    this->y = newY;
}

void Hitbox::draw(vec4 color) const {
    Texture2d::setColor(color);
    texture.draw(x, y, w, h);
}