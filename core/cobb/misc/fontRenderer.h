//
// Created by cobble on 12/30/2024.
//

#ifndef ENGINE_2_FONTRENDERER_H
#define ENGINE_2_FONTRENDERER_H

#include "texture2d.h"

using namespace cobb;

static Shader *fontShader;

class FontRenderer {
    Texture2d font;
    float maxWidth{};
    float height{};
    char cs[81]{};
    float ws[81]{};
    vec4 color{};
public:
    float characterSpacing{};
    vec4 colors[10]{};
    FontRenderer() = default;
    explicit FontRenderer(const std::string& fontTexture);

    void setColor(vec4 color);
    void draw(const std::string& text, float x, float y, float scale);
    static void loadShader();
    float getWidth(const std::string& text);
    [[nodiscard]] float getHeight() const;
    int getIndex(char c);

};


#endif //ENGINE_2_FONTRENDERER_H
