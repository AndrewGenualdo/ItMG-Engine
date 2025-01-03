//
// Created by cobble on 12/30/2024.
//

#include <bits/stdc++.h>
#include "fontRenderer.h"

FontRenderer::FontRenderer(const std::string &fontTexture) {
    font = Texture2d(fontTexture);
    characterSpacing = 1.0f;
    maxWidth = 7.0f;
    height = 7.0f;
    color = vec4(1);
    for(auto & c : colors) c = vec4(1);

    //characters
    for (char i = 0; i < 26; i++) cs[i] = (char) ('A' + i);
    for (int i = 0; i < 10; i++) cs[i + 26] = (char) ('0' + i);
    cs[36] = '.';
    cs[37] = ',';
    cs[38] = '<';
    cs[39] = '>';
    cs[40] = '/';
    cs[41] = '?';
    cs[42] = ':';
    cs[43] = ';';
    cs[44] = '\'';
    cs[45] = '"';
    cs[46] = '\\';
    cs[47] = '|';
    cs[48] = '!';
    cs[49] = '@';
    cs[50] = '#';
    cs[51] = '$';
    cs[52] = '%';
    cs[53] = '^';
    cs[54] = '&';
    cs[55] = '*';
    cs[56] = '(';
    cs[57] = ')';
    cs[58] = '[';
    cs[59] = ']';
    cs[60] = '{';
    cs[61] = '}';
    cs[62] = '-';
    cs[63] = '_';
    cs[64] = '=';
    cs[65] = '+';
    cs[66] = '`';
    cs[67] = '~';
    cs[68] = ' ';


    //widths
    for (float &w: ws) w = 7; //default of 7
    ws[0] = 6; //A
    ws[1] = 6; //B
    ws[2] = 5; //C
    ws[4] = 6; //E
    ws[5] = 6; //F
    ws[7] = 6; //H
    ws[8] = 6; //I
    ws[11] = 5; //L
    ws[15] = 6; //P
    ws[17] = 6; //R
    ws[18] = 6; //S
    ws[19] = 6; //T
    ws[20] = 6; //U
    ws[24] = 6; //Y
    ws[26] = 6; //0
    ws[27] = 6; //1
    ws[28] = 6; //2
    ws[29] = 5; //3
    ws[31] = 6; //5
    ws[32] = 6; //6
    ws[33] = 6; //7
    ws[34] = 6; //8
    ws[35] = 6; //9
    ws[36] = 1; //.
    ws[37] = 2; //,
    ws[38] = 3; //<
    ws[39] = 3; //>
    ws[40] = 3; ///
    ws[41] = 6; //?
    ws[42] = 2; //:
    ws[43] = 2; //;
    ws[44] = 1; //'
    ws[45] = 3; //"
    ws[46] = 3; //\
    no clue why this line break has to be here but its kinda funny
    ws[47] = 1; //|
    ws[48] = 2; //!
    ws[51] = 5; //$
    ws[52] = 5; //%
    ws[53] = 5; //^
    ws[54] = 6; //&
    ws[55] = 3; //*
    ws[56] = 4; //(
    ws[57] = 4; //)
    ws[58] = 4; //[
    ws[59] = 4; //]
    ws[60] = 3; //{
    ws[61] = 3; //}
    ws[62] = 6; //-
    ws[63] = 6; //_
    ws[64] = 6; //=
    ws[65] = 6; //+
    ws[66] = 2; //`
    ws[67] = 6; //~
    ws[68] = 6; //space

    loadShader();
}


void FontRenderer::draw(const std::string &text, float x, float y, float scale) {
    glDisable(GL_DEPTH_TEST);
    font.bind();
    fontShader->use();
    glBindVertexArray(*Texture2d::getVAO());
    fontShader->setMat4("proj", Texture2d::orthoProj);
    fontShader->setVec4("color", color);
    float windowScale = Texture2d::window->gw / Window::GAME_WIDTH;

    float xOffset = 0;
    float yOffset = 0;
    bool skip = false;
    for (int i = 0; i < text.length(); i++) {
        if(skip || text[i] == '\0') {
            skip = false;
            continue;
        } else if (text[i] == '\n') {
            yOffset -= (height + characterSpacing * 2.5f) * scale;
            xOffset = 0;
            continue;
        } else if(text[i] == '%') {
            if(i != text.length() - 1) { //make sure its not the last character
                int c = std::stoi(&text[i+1]);
                fontShader->setVec4("color", colors[c]);
                skip = true;
                continue;
            }
        }
        int index = getIndex(text[i]);
        if (index == -1) index = 69; //unknown character
        fontShader->setInt("index", index);
        float width = ws[index];
        float nx = Texture2d::window->sX(x + xOffset + maxWidth * scale * 0.23f) + (Texture2d::window->sX(0) * ((x + xOffset + maxWidth * scale * 0.23f) / Window::GAME_WIDTH));
        float ny = Texture2d::window->sY(y + yOffset + height * scale * 0.43f) + (Texture2d::window->sY(0) * ((y + yOffset + height * scale * 0.43f) / Window::GAME_HEIGHT));
        float nw = maxWidth * windowScale * 0.5f * scale;
        float nh = height * windowScale * 0.5f * scale;
        fontShader->setMat4("model", Object::translate(nx + nw / maxWidth * 0.5f + nw * 0.5f, ny + nh / getHeight(), 0) * Object::scale(nw, nh, 1)); //this line might be fucked
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
        xOffset += (width + characterSpacing) * scale;
    }
    glEnable(GL_DEPTH_TEST);
}

void FontRenderer::loadShader() {
    if (fontShader == nullptr) {
        fontShader = new Shader("assets/fontRenderer");
        fontShader->use();
        fontShader->setVec4("color", vec4(1));
        glBindVertexArray(*Texture2d::getVAO());
        std::cout << "Loaded font shader!" << std::endl;
    }
}

float FontRenderer::getWidth(const std::string &text) {
    std::string s = text;
    std::transform(s.begin(), s.end(), s.begin(), ::toupper);
    float width = 0;
    for (int i = 0; i < s.length(); i++) {
        int index = getIndex(s[i]);
        if (index == -1) width += 7;
        else width += ws[index] + (i == s.length() - 1 ? 0 : characterSpacing);
    }
    return width;
}

int FontRenderer::getIndex(char c) {
    char C = toupper(c);
    for (int i = 0; i < 81; i++) if (cs[i] == C) return i;
    return -1;
}

float FontRenderer::getHeight() const {
    return height;
}

void FontRenderer::setColor(vec4 c) {
    color = c;
}
