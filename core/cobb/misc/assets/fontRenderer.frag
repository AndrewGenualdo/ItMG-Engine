#version 330 core

uniform sampler2D tex;
uniform vec4 color;
uniform int index;

in vec2 TexCoords;

out vec4 FragColor;

const float ROWS = 9.0f;
const float COLUMNS = 9.0f;

void main() {
    float column = mod(index, COLUMNS); //x
    float row = COLUMNS - (index / int(COLUMNS)) - 1.0f; //y
    FragColor = texture(tex, vec2(TexCoords.x / COLUMNS + (column / COLUMNS), TexCoords.y / ROWS + (row / ROWS))) * color;
}