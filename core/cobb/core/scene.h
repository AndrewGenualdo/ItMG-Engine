//
// Created by cobble on 12/28/2024.
//

#ifndef ENGINE_2_SCENE_H
#define ENGINE_2_SCENE_H

#pragma once

#include "window.h"

using namespace cobb;
using namespace glm;

class Scene {
public:
    virtual void load();
    virtual void draw();
    virtual void keyPress(int key, int action);
    virtual void mouseMove(float x, float y);
};


#endif //ENGINE_2_SCENE_H
