#version 330 core
layout (location = 0) in vec3 aPos;

out vec3 color;

uniform float time;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec4 pos;

void main() {


    pos = vec4(aPos, 1.0f);
    //Scale then rotate then translate
    //Matrix multiplication goes backwards tho so T.ranslate R.otate S.cale (TRS)
    //Model Matrix (TRS)
    //Model Space -> World Space
    //mat4 M = translate(0, 0, 0) * rotate(0, 0, 0) * scale(1, 1, 1);
    mat4 M = model;
    gl_Position = M * pos;
}