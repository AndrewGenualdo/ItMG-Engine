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

    private:
        void load(string vertexPath, string fragmentPath);

    public:

        unsigned int ID; //the shader ID

        Shader(string basePath);
        Shader(string vertexPath, std::string fragmentPath);

        //"activate" the shader to whatever you draw until a different shader is activated? I think
        void use();


        //set uniforms for inside the shaders
        void setBool(const string &name, bool value) const;
        void setInt(const string &name, int value) const;
        void setFloat(const string &name, float value) const;
        void setVec2(const string &name, float x, float y) const;

    };
}

#endif