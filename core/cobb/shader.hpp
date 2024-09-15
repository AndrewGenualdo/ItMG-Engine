//
// Created by andrew.genualdo on 9/10/2024.
//
#ifndef SHADER_H
#define SHADER_H

#include <string>

#include "../ew/external/glad.h"

using namespace std;

namespace cobb {
    class Shader {
    public:

        unsigned int ID; //the shader ID

        Shader(std::string vertexPath, std::string fragmentPath);

        //"activate" the shader to whatever you draw until a different shader is activated? I think
        void use();


        //set uniforms for inside the shaders
        void setBool(const std::string &name, bool value) const;
        void setInt(const std::string &name, int value) const;
        void setFloat(const std::string &name, float value) const;

    };
}

#endif