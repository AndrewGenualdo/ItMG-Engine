//
// Created by Zeben on 11/22/2024.
//

#include "textRenderer.h"
/*#include "../core/shader.h"
#include "../core/window.h"


void TextRenderer::loadText(std::string fontPath)
{
    if (FT_Init_FreeType(&ft))
    {
        std::cout << "ERROR::FREETYPE: Failed tp initialize freetype" << std::endl;
    }

    //find path to font
    if (fontPath.empty())
    {
        std::cout << "ERROR::FREETYPE: Failed to load font name" << std::endl;
    }

    //load font as face
    if (FT_New_Face(ft, fontPath.c_str(), 0, &face))
    {
        std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
    }
    else
    {
        //set size to load glyphs as
        FT_Set_Pixel_Sizes(face, 0, 48);

        //disable byte-alignment restriction
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

        //load first 128 characters of ASCII set
        for (unsigned char c = 0; c < 128; c++)
        {
            //load character glyph
            if (FT_Load_Char(face, c, FT_LOAD_RENDER))
            {
                std::cout << "ERROR::FREETYPE: Failed to load glyph" << std::endl;
                continue;
            }

            //generate texture
            unsigned int glyphTexture;
            glGenTextures(1, &glyphTexture);
            glBindTexture(GL_TEXTURE_2D, glyphTexture);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, face->glyph->bitmap.width, face->glyph->bitmap.rows, 0, GL_RED, GL_UNSIGNED_BYTE, face->glyph->bitmap.buffer);

            //set texture options
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            //store character for later use
            Character character =
                    {
                    glyphTexture, ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows), ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top), static_cast<unsigned int>(face->glyph->advance.x)
                    };
            characterMap.insert(std::pair<char, Character>(c, character));
        }
        glBindVertexArray(0);
        glBindTexture(GL_TEXTURE_2D, 0);
    }
}

void TextRenderer::RenderText(Shader &shader, std::string text, float x, float y, float scale, vec3 color)
{
    shader.use();
    glUniform3f(glGetUniformLocation(shader.ID, "textColor"), color.x, color.y, color.z);
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(textVAO);

    //iterate through all characters
    std::string::const_iterator character;
    for (character = text.begin(); character != text.end(); character++)
    {
        Character ch = characterMap[*character];

        float xPos = x + ch.Bearing.x * scale;
        float yPos = (w->getHeight() - y) - (ch.Size.y - ch.Bearing.y) * scale;//- (ch.Size.y - ch.Bearing.y) * scale;

        float width = ch.Size.x * scale;
        float height = ch.Size.y * scale;
        float vertices[6][4] =
                {
                xPos, yPos + height, 0.0f, 0.0f,
                xPos, yPos, 0.0f, 1.0f,
                xPos + width, yPos, 1.0f, 1.0f,

                xPos, yPos + height, 0.0f, 0.0f,
                xPos + width, yPos, 1.0f, 1.0f,
                xPos + width, yPos + height, 1.0f, 0.0f
                };

        //render glyph texture over quad
        glBindTexture(GL_TEXTURE_2D, ch.textureID);
        //update content of VBO memory
        glBindBuffer(GL_ARRAY_BUFFER, textVBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); //need to use subdata instead of data
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        //render quad
        glDrawArrays(GL_TRIANGLES, 0, 6);

        //advance cursors for next quad
        x += (ch.Advance >> 6) * scale; //bitshift by six to get value in pixels (2^6 = 64 (divide amount by 1/64th pixels by 64 to get the amount of pixels)
    }
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}

float TextRenderer::getWidth(std::string text, float scale)
{
    float totalWidth = 0.0f;
    //iterate through all characters
    std::string::const_iterator character;
    for (character = text.begin(); character != text.end(); character++) {
        Character ch = characterMap[*character];
        totalWidth+= ch.Size.x;
    }
    return totalWidth * scale;
}

float TextRenderer::getHeight(float scale)
{
    std::string text = "ABCDEFGHIJKLMNOPQRSTUVWXYZ ";
    float maxHeight = 0.0f;
    std::string::const_iterator character;
    for (character = text.begin(); character != text.end(); character++) {
        Character ch = characterMap[*character];
        if(ch.Size.y > maxHeight) {
            maxHeight = ch.Size.y;
        }
    }
    return maxHeight * scale;
}*/
