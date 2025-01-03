//
// Created by cobble on 12/28/2024.
//

#ifndef ENGINE_2_MENU_H
#define ENGINE_2_MENU_H

#include "cobb/core/scene.h"

using namespace glm;
using namespace cobb;

class MenuScene : Scene {
    void load() override;
    void draw() override;
    void keyPress(int key, int action) override;
    void mouseMove(float x, float y) override;
};



#endif //ENGINE_2_MENU_H
