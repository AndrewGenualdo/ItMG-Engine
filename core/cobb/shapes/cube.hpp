//
// Created by cobble on 10/12/2024.
//

#ifndef EWRENDER_CUBE_HPP
#define EWRENDER_CUBE_HPP

#pragma once

#include "iostream"
#include <string>
#include "../../ew/external/glad.h"
#include "../../ew/external/stb_image.h"
#include "../core/shader.h"

namespace cobb {
    static unsigned int CUBEMAP_VAO = -1, CUBEMAP_VBO = -1;


const static float VERTICES_CUBE[] = {
 //PX  PY  PZ   NX    NY     NZ    U  V
  -1, -1, -1,  0.0f,  0.0f, -1.0f, 0, 0,
   1, -1, -1,  0.0f,  0.0f, -1.0f, 1, 0,
   1,  1, -1,  0.0f,  0.0f, -1.0f, 1, 1,
   1,  1, -1,  0.0f,  0.0f, -1.0f, 1, 1,
  -1,  1, -1,  0.0f,  0.0f, -1.0f, 0, 1,
  -1, -1, -1,  0.0f,  0.0f, -1.0f, 0, 0,

  -1, -1,  1,  0.0f,  0.0f, 1.0f, 0, 0,
   1, -1,  1,  0.0f,  0.0f, 1.0f, 1, 0,
   1,  1,  1,  0.0f,  0.0f, 1.0f, 1, 1,
   1,  1,  1,  0.0f,  0.0f, 1.0f, 1, 1,
  -1,  1,  1,  0.0f,  0.0f, 1.0f, 0, 1,
  -1, -1,  1,  0.0f,  0.0f, 1.0f, 0, 0,

  -1,  1,  1, -1.0f,  0.0f,  0.0f, 1, 0,
  -1,  1, -1, -1.0f,  0.0f,  0.0f, 1, 1,
  -1, -1, -1, -1.0f,  0.0f,  0.0f, 0, 1,
  -1, -1, -1, -1.0f,  0.0f,  0.0f, 0, 1,
  -1, -1,  1, -1.0f,  0.0f,  0.0f, 0, 0,
  -1,  1,  1, -1.0f,  0.0f,  0.0f, 1, 0,

   1,  1,  1,  1.0f,  0.0f,  0.0f, 1, 0,
   1,  1, -1,  1.0f,  0.0f,  0.0f, 1, 1,
   1, -1, -1,  1.0f,  0.0f,  0.0f, 0, 1,
   1, -1, -1,  1.0f,  0.0f,  0.0f, 0, 1,
   1, -1,  1,  1.0f,  0.0f,  0.0f, 0, 0,
   1,  1,  1,  1.0f,  0.0f,  0.0f, 1, 0,

  -1, -1, -1,  0.0f, -1.0f,  0.0f, 0, 1,
   1, -1, -1,  0.0f, -1.0f,  0.0f, 1, 1,
   1, -1,  1,  0.0f, -1.0f,  0.0f, 1, 0,
   1, -1,  1,  0.0f, -1.0f,  0.0f, 1, 0,
  -1, -1,  1,  0.0f, -1.0f,  0.0f, 0, 0,
  -1, -1, -1,  0.0f, -1.0f,  0.0f, 0, 1,

  -1,  1, -1,  0.0f,  1.0f,  0.0f, 0, 1,
   1,  1, -1,  0.0f,  1.0f,  0.0f, 1, 1,
   1,  1,  1,  0.0f,  1.0f,  0.0f, 1, 0,
   1,  1,  1,  0.0f,  1.0f,  0.0f, 1, 0,
  -1,  1,  1,  0.0f,  1.0f,  0.0f, 0, 0,
  -1,  1, -1,  0.0f,  1.0f,  0.0f, 0, 1
};

    class Cube
    {
        unsigned int m_id;

        int m_width, m_height;
        int m_filterMode;
        const char *m_path;

    public:
        //unsigned int VBO, EBO;
        float vertices[36];
        Cube(const std::string &path);
        Cube(const std::string &path, int filterMode);
        ~Cube();
        void bind();
        void draw();

        unsigned int getId() const;

        static unsigned int* getVAO();
        static unsigned int* getVBO();
        //static unsigned int* getVAO();

        void load();
    };
}



#endif //EWRENDER_CUBE_HPP
