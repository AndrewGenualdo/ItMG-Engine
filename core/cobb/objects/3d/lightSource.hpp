//
// Created by andrew.genualdo on 10/29/2024.
//

#ifndef LIGHTSOURCE_H
#define LIGHTSOURCE_H
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

#include "camera.hpp"
#include "cube.hpp"

using namespace std;
using namespace glm;

namespace cobb {

    static Shader *lightShader;


    class LightSource {

    public:
        vec3 pos;
        vec4 color;

        LightSource(vec3 pos, vec4 color);

        void load();
        void draw(Camera &camera);

        static void loadShader();
        static void bind();
    };
}

#endif //LIGHTSOURCE_H
