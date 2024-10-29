#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

uniform mat3 transposeInverseModel;
uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

out vec3 FragPos;
out vec2 TexCoord;
out vec3 Normal;

void main() {

    TexCoord = aTexCoord;
    Normal = transposeInverseModel * aNormal;
    FragPos = vec3(model * vec4(aPos, 1.0f));

    //Scale then rotate then translate
    //Matrix multiplication goes backwards tho so T.ranslate R.otate S.cale (TRS)
    //Model Matrix (TRS)
    //Model Space -> World Space
    //mat4 M = translate(0, 0, 0) * rotate(0, 0, 0) * scale(1, 1, 1);
    gl_Position = proj * view * model * vec4(aPos, 1.0f);
}