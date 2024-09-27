#version 330 core

in vec3 color;
in vec3 screenPos;

out vec4 FragColor;

uniform float time;
uniform sampler2D tex;
uniform vec2 dims;

const float PI = 3.1415926535; //I mean, change this if you want but I wouldn't
const float BACKGROUND_FREQUENCY = 1.0f; //How fast the background moves? sorta? idk
const float FOV = 3.0f; //how zoomed in each cube feels? kinda?
const int CUBE_COUNT = 10; //how many cubes there are
const float CUBE_BRIGHTNESS = 19.0f; //how many times brighter than the background are the cubes
const float CUBE_LINE_WIDTH = 1.0f; //the thickness of each line (CUBE_LINE_WIDTH * 2 = pixels wide the line will be)
const float CUBE_MOVE_SPEED = 0.25f; //how fast each cube moves
const float CUBE_HORIZONTAL_SPEED = 0.07f; //if you want more or less horizontal movement
const float CUBE_VERTICAL_SPEED = 0.05f; //if you want more or less vertical movement
const float CUBE_SCALE = 100.0f; //how big each cube is (in pixels)
const float CUBE_SEPARATION = 0.05f; //how separated each cube is (any non-0 value will have them separate no matter what, this controls how fast they separate)

//translate 3d points into 2d point
vec2 toScreen(vec3 inp) {
    float fX = inp.x + dims.x / 2;
    float fY = inp.y + dims.y / 2;

    float fZXP = inp.x / (dims.x / 2);
    float fZYP = inp.y / (dims.y / 2);

    float fZ = inp.z / 4;
    float fZX = fZ * fZXP * FOV;
    float fZY = fZ * fZYP * FOV;
    float x = fX + fZX;
    float y = fY + fZY;
    return vec2(x,y);
}

vec4 drawCircle(vec4 inputColor, vec2 pos, vec2 center, float radius, vec4 color) {
    //this just checks if the distance to the point is less than the thickness, if it is, its part of the circle.
    if(length(pos - center) < radius) {
        return color;
    } else {
        return inputColor;
    }
}

vec4 drawLine(vec4 inputColor, vec2 pos, vec2 p1, vec2 p2, float thickness, vec4 color) {
    vec4 outColor = inputColor;

    //I honestly forget how this works (wrote it last year for a matrices vectors and 3d math assignment) but it gets the distance to the nearest point on the defined line
    //tried to comment it as much as I could
    vec2 p3 = pos; //this is the point we are checking against the line
    vec2 l12 = p2 - p1;
    vec2 l13 = p3 - p1;
    float d = dot(l12, l13) / length(l12); //gets the projection of l13 onto l12, results in a scalar
    vec2 p4 = p1 + normalize(l12) * d; //this gets the closest point on the line to the input point (p3)
    vec2 l14 = p4 - p1;
    vec2 l24 = p4 - p2;
    vec2 l34 = p4 - p3;

    // if that distance is less than the thickness in every direction,
    // AND makes sure its not before the start of the line
    // AND makes sure its not beyond the end of the line
    // if true: it is part of the line (change the color)
    if(length(l34) < thickness && length(l14) <= length(l12) && length(l24) <= length(l12)) {
        outColor = color;
    }

    //the ends of the lines were squared off, so I draw a circle there with the same thickness to make it round. The issue is more visible with higher thickness lines
    //The reason there is only one of these (instead of one for both ends of the line) is because if you set the vertices of the cube just right, it will still draw one on every vertex.
    outColor = drawCircle(outColor, pos, p2, thickness, color);


    return outColor;
}

vec4 drawCube(vec4 inpColor, vec2 pos, float cubeLength, float lineWidth, vec3 cubePosition, vec4 cubeColor) {

    float len = (cubeLength / 2);

    //cube vertices
    vec3 p1 = cubePosition + vec3(-len, -len, -len); //back  top    left
    vec3 p2 = cubePosition + vec3(-len, -len,  len); //front top    left
    vec3 p3 = cubePosition + vec3( len, -len, -len); //back  top    right
    vec3 p4 = cubePosition + vec3( len, -len,  len); //front top    right
    vec3 p5 = cubePosition + vec3(-len,  len, -len); //back  bottom left
    vec3 p6 = cubePosition + vec3(-len,  len,  len); //front bottom left
    vec3 p7 = cubePosition + vec3( len,  len, -len); //back  bottom right
    vec3 p8 = cubePosition + vec3( len,  len,  len); //front bottom right

    vec4 color = inpColor;

    //draw all the wireframe lines
    //top square
    color = drawLine(color, pos, toScreen(p1), toScreen(p2), lineWidth, cubeColor);
    color = drawLine(color, pos, toScreen(p1), toScreen(p3), lineWidth, cubeColor);
    color = drawLine(color, pos, toScreen(p3), toScreen(p4), lineWidth, cubeColor);
    color = drawLine(color, pos, toScreen(p2), toScreen(p4), lineWidth, cubeColor);

    //supporting pillars
    color = drawLine(color, pos, toScreen(p5), toScreen(p1), lineWidth, cubeColor);
    color = drawLine(color, pos, toScreen(p2), toScreen(p6), lineWidth, cubeColor);
    color = drawLine(color, pos, toScreen(p3), toScreen(p7), lineWidth, cubeColor);
    color = drawLine(color, pos, toScreen(p4), toScreen(p8), lineWidth, cubeColor);

    //base
    color = drawLine(color, pos, toScreen(p6), toScreen(p5), lineWidth, cubeColor);
    color = drawLine(color, pos, toScreen(p5), toScreen(p7), lineWidth, cubeColor);
    color = drawLine(color, pos, toScreen(p7), toScreen(p8), lineWidth, cubeColor);
    color = drawLine(color, pos, toScreen(p6), toScreen(p8), lineWidth, cubeColor);
    return color;
}

void main() {

    //my weird background color formula
    vec4 bgColor = vec4(
    (sin(BACKGROUND_FREQUENCY*1.0*(time-screenPos.x+screenPos.y))+1)*(cos(BACKGROUND_FREQUENCY*1.2*(time+1*PI))+1)*0.025f+0.02f,
    (sin(BACKGROUND_FREQUENCY*1.1*(time+screenPos.x+screenPos.y))+1)*(cos(BACKGROUND_FREQUENCY*1.1*(time+2*PI))+1)*0.025f+0.02f,
    (sin(BACKGROUND_FREQUENCY*1.2*(time+screenPos.x-screenPos.y))+1)*(cos(BACKGROUND_FREQUENCY*1.0*(time+3*PI))+1)*0.025f+0.02f,
    1.0f);

    //the color of everything we will draw (same color as background, just brighter, or darker I guess if CUBE_BRIGHTNESS < 1.0)
    vec4 cubeColor = bgColor * CUBE_BRIGHTNESS;

    //Translates the -1.0 to 1.0 to 0 to 1920/1080, prevents stretching
    vec2 pos = (screenPos.xy + 1.0f) * 0.5f * dims;

    vec4 color = bgColor;

    for(int i=0;i<CUBE_COUNT;i++) {
        vec3 cubePos = vec3(
        sin((1.0 + i * CUBE_HORIZONTAL_SPEED)*(time + (i * 2)) * CUBE_MOVE_SPEED) * (dims.x / 2 - CUBE_SCALE * 1.5),
        cos((1.0 + CUBE_SEPARATION + i * CUBE_VERTICAL_SPEED)*(time + (i * 2)) * CUBE_MOVE_SPEED) * (dims.y / 2 - CUBE_SCALE * 1.5),
        0.0f);

        color = drawCube(color, pos, CUBE_SCALE, CUBE_LINE_WIDTH, cubePos, cubeColor);
    }

    FragColor = color;
}