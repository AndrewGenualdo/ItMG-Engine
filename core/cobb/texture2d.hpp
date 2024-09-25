//
// Created by cobble on 9/25/2024.
//

#ifndef EWRENDER_TEXTURE2D_HPP
#define EWRENDER_TEXTURE2D_HPP

#include "iostream"
#include <string>
#include "../ew/external/glad.h"
#include "../ew/external/stb_image.h"
#include "shader.hpp"

using namespace std;

namespace cobb {
    static unsigned int textureCount = 0;
    static unsigned int selectedTexture = 4294967295;
    /*static const float vertices[] = {
            // positions (xyz)      color (rgba)              texture coords (xy) ((0,0) is bottom left)
            0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 1.0f, 1.0f,   1.0f, 1.0f,// top right
            0.5f, -0.5f, 0.0f,   1.0f, 1.0f, 1.0f, 1.0f,   1.0f, 0.0f,// bottom right
            -0.5f, -0.5f, 0.0f,   1.0f, 1.0f, 1.0f, 1.0f,   0.0f, 0.0f,// bottom left
            -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 1.0f, 1.0f,   0.0f, 1.0f,// top left
    };*/
    const unsigned int indices[] = {  // note that we start from 0!
            0, 1, 3,   // first triangle
            1, 2, 3    // second triangle
    };


    class Texture2d
    {
    private:
        unsigned int m_id;
        unsigned int VBO, VAO, EBO;
        int m_width, m_height;
        int m_filterMode, m_wrapMode;
        const char *m_path;
        Shader* m_shader;

        void loadVertices(float positions[]);


    public:
        float vertices[36];
        Texture2d(const string &path, Shader* shader, float positions[8]);
        Texture2d(const string &path, Shader* shader, int filterMode, int wrapMode, float positions[8]);
        ~Texture2d();
        void draw() const;

        Shader* getShader();
        void load();

        void setShaderBool(const string &name, bool value);
        void setShaderInt(const string &name, int value);
        void setShaderFloat(const string &name, float value);
    };
}



#endif //EWRENDER_TEXTURE2D_HPP
