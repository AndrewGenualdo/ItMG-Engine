//
// Created by andrew.genualdo on 9/10/2024.
//
#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <glm/fwd.hpp>

#include "../ew/external/glad.h"

using namespace std;

namespace cobb {
    class Shader {
    public:

        unsigned int ID; //the shader ID

        Shader(string shaderPath);
        Shader(string vertexPath, string fragmentPath);

        //"activate" the shader to whatever you draw until a different shader is activated? I think
        void use();


        //set uniforms for inside the shaders
        void setBool(const string &name, bool value) const;
        void setInt(const string &name, int value) const;
        void setFloat(const string &name, float value) const;
        void setVec2(const string &name, const glm::vec2 &value) const;
        void setMat4(const string &name, const glm::mat4 &value) const;

    private:
        void load(string vertexPath, string fragmentPath);

    };
}

#endif