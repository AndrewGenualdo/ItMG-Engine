//
// Created by cobble on 9/25/2024.
//

#ifndef EWRENDER_TEXTURE2D_HPP
#define EWRENDER_TEXTURE2D_HPP

#include "iostream"
#include <string>
#include "../../ew/external/glad.h"
#include "../../ew/external/stb_image.h"
#include "../core/shader.h"
#include "../core/object.h"
#include "../core/window.h"

using namespace glm;

namespace cobb {
    static unsigned int VAO = -1, VBO = -1, EBO = -1;
    const unsigned int indices[] = {  // note that we start from 0!
            0, 1, 3,   // first triangle
            1, 2, 3    // second triangle
    };


    static Shader *texture2dShader;


    class Texture2d
    {
    private:
        unsigned int m_id;

        int m_width, m_height;
        int m_filterMode, m_wrapMode;
        const char *m_path;

    public:

        static mat4 orthoProj;
        static Window *window;

        //unsigned int VBO, EBO;
        float vertices[36];
        Texture2d();
        explicit Texture2d(const std::string &path);
        Texture2d(const std::string &path, float positions[8]);
        Texture2d(const std::string &path, int filterMode, int wrapMode, float positions[8]);
        ~Texture2d();
        void bind() const;


        void loadVertices(const float positions[]);

        [[nodiscard]] float getWidth() const;
        [[nodiscard]] float getHeight() const;
        [[nodiscard]] std::string getPath() const;

        [[nodiscard]] unsigned int getId() const;

        static unsigned int* getVAO();
        static unsigned int* getEBO();
        static unsigned int* getVBO();

        void load();

        void draw(float x, float y, float scale, bool shouldBind = true) const;
        void draw(float x, float y, float width, float height, bool shouldBind = true) const;
        void draw(float x, float y, float width, float height, float rotation, bool shouldBind = true) const;
        void drawRaw(float x, float y, float width, float height, float rotation, bool shouldBind = true) const;
        void drawRaw(float x, float y, float width, float height, bool shouldBind = true) const;

        static void loadShader();
        static void setOrtho(mat4 ortho);
        static void setColor(vec4 color);



    };
}



#endif //EWRENDER_TEXTURE2D_HPP
