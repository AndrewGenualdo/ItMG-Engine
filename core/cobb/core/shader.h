//
// Created by andrew.genualdo on 9/10/2024.
//
#ifndef SHADER_H
#define SHADER_H

#include <string>
#include "glm/fwd.hpp"

#include "../../ew/external/glad.h"

namespace cobb {
    class Shader {
    public:

        unsigned int ID; //the shader ID

        Shader();
        Shader(std::string shaderPath);
        Shader(std::string vertexPath, std::string fragmentPath);

        //"activate" the shader to whatever you draw until a different shader is activated? I think
        void use();


        //set uniforms for inside the shaders
        void setBool(const std::string &name, bool value) const;
        void setInt(const std::string &name, int value) const;
        void setFloat(const std::string &name, float value) const;
        void setVec2(const std::string &name, const glm::vec2 &value) const;
        void setVec3(const std::string &name, const glm::vec3 &value) const;
        void setVec4(const std::string &name, const glm::vec4 &value) const;
        void setMat3(const std::string &name, const glm::mat3 &value) const;
        void setMat4(const std::string &name, const glm::mat4 &value) const;

    private:
        void load(std::string vertexPath, std::string fragmentPath);

    };
}

#endif