//
// Created by andrew.genualdo on 9/17/2024.
//

#ifndef WINDOW_H
#define WINDOW_H

#include <string>
#include "../ew/external/glad.h"
#include <GLFW/glfw3.h>

using namespace std;

namespace cobb {
    class Window {
    public:
        static constexpr int SCREEN_WIDTH = 1920;
        static constexpr int SCREEN_HEIGHT = 1080;

        GLFWwindow* window;

        explicit Window(const string& title);
        void update();
    private:
        string _title;
        int _uptime;
        int _frames;
    };
}




#endif //WINDOW_H
