#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;

uniform mat4 model;

uniform mat4 viewProj;

out vec4 pos;
out vec2 TexCoord;
out vec3 block;

void main() {

    TexCoord = aTexCoord;
    pos = vec4(aPos, 1.0f);
    //Scale then rotate then translate
    //Matrix multiplication goes backwards tho so T.ranslate R.otate S.cale (TRS)
    //Model Matrix (TRS)
    //Model Space -> World Space
    //mat4 M = translate(0, 0, 0) * rotate(0, 0, 0) * scale(1, 1, 1);
    block = vec3(model[3][0], model[3][1], model[3][2]);
    gl_Position = viewProj * model * pos;
}