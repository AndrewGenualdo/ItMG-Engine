//
// Created by andrew.genualdo on 9/17/2024.
//


#ifndef WINDOW_H
#define WINDOW_H

#pragma once

#include <string>
#include "../../ew/external/glad.h"
#include "GLFW/glfw3.h"
#include "glm/vec2.hpp"
#include <iostream>

namespace cobb {


    class Window {
    public:
        static constexpr float SCREEN_WIDTH = 1280;//1920;
        static constexpr float SCREEN_HEIGHT = 720;//1080;

        static constexpr float GAME_WIDTH = 1920;
        static constexpr float GAME_HEIGHT = 1080;

        GLFWwindow *window;

        glm::vec2 mousePos;

        Window();

        explicit Window(const std::string &title);

        float update();

        [[nodiscard]] float getTime() const;

        [[nodiscard]] float getWidth() const;

        [[nodiscard]] float getHeight() const;

        /*
        * Converts screen x coordinates into game x coordinates
        */
        [[nodiscard]] float gX(float screenX) const;

        /*
         * Converts screen y coordinates into game y coordinates
        */
        [[nodiscard]] float gY(float screenY) const;

        /*
        * Converts game x coordinates into screen x coordinates
        */
        [[nodiscard]] float sX(float x) const;

        /*
        * Converts game y coordinates into screen y coordinates
        */
        [[nodiscard]] float sY(float y) const;

        void getGameDimensions();

        void setWidth(int width);

        void setHeight(int height);

        void setTime(double time);

        static void setVsync(bool vsync);

    private:
        std::string _title;
        int _uptime;
        int _frames;
        float _time;
        int _width; //screen width
        int _height; //screen height
        double _timeOffset;
    public:
        float gx, gy, gw, gh; //game x, game y, game width, game height
    };


}


#endif //WINDOW_H
