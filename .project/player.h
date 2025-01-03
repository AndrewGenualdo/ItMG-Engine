//
// Created by cobble on 12/27/2024.
//

#ifndef ENGINE_2_PLAYER_H
#define ENGINE_2_PLAYER_H

#pragma once

#include "../core/cobb/misc/animation.h"
#include "level.h"
#include "cobb/core/window.h"
#include "hitbox.h"

const int TEX_LEFT = 0;
const int TEX_STILL = 1;
const int TEX_RIGHT = 2;
const int TEX_MOUSE_GRABBED = 3;
const int TEX_SIT = 4;
const int TEX_SLIDE_LEFT = 5;
const int TEX_SLIDE_RIGHT = 6;
const int TEX_WALL_SLIDE_LEFT = 7;
const int TEX_WALL_SLIDE_RIGHT = 8;
const int TEX_ROCKET_CHARGE = 9;


const int KEY_COUNT = 5;
const int MOUSE_LEFT = 0;
const int KEY_LEFT = 1;
const int KEY_JUMP = 2;
const int KEY_DOWN = 3;
const int KEY_RIGHT = 4;

using namespace cobb;
using namespace glm;

class Player {
private:
    bool wasKeys[KEY_COUNT]{};

public:
    static constexpr int MAX_PLAYERS = 8;
    static constexpr vec4 playerColors[Player::MAX_PLAYERS + 1] = {
            vec4(0, 1, 1, 1),
            vec4(1, 0, 0, 1),
            vec4(1, 1, 0, 1),
            vec4(0, 1, 0, 1),
            vec4(1, 0, 1, 1),
            vec4(0, 0, 1, 1),
            vec4(1, 1, 1, 1),
            vec4(0, 0, 0, 1),
            vec4(0.58823f, 0.29411f, 0, 1)
    };
    static Player players[MAX_PLAYERS];
    static Texture2d texture;
    static Animation anims[10];
    static Texture2d blank;
    static Window *window;
    static int loadCount;

    static constexpr int MAX_LEVELS = 10;
    static Level levels[MAX_LEVELS];
    static int currentLevel;


    bool keys[KEY_COUNT]{};
    bool connected = false;
    vec2 pos = vec2(0);
    vec2 velocity = vec2(0);
    vec2 mousePos = vec2(0);
    int facing = 0;
    int id = -1;
    Hitbox hurtbox = Hitbox();
    int platStandingOn = -1;

    float scale{};

    explicit Player(Window *w, const std::string& tex, float scale);

    Player();

    //player stats
    float groundAcceleration = 6000.0f; //how much they accelerate horizontally on the ground
    float groundFriction = 4000.0f; //howm much their x velocity slows down on the ground
    float airAcceleration = 3000.0f; //how much they accelerate horizontally in the air
    float airResistance = 10.0f; //how much their x velocity slows down in the air
    float gravity = 10000.0f; //how fast they accellerate as they fall
    float jumpVel = 2000.0f; //how high they jump
    float startSpeed = 1000.0f; //initial grouned dash speed
    float wallSlideFriction = 0.75f; //how much their y velocity is multiplied by when wall sliding
    float wasWallSlideTime = 0.15f; //the amount of time after leaving a wall slide, the player can still wall jump
    float wallJumpHorizVel = 1000.0f;
    float wallJumpVertVel = 3000.0f;
    float minRocketSpeed = 500.0f;
    float maxRocketSpeed = 2000.0f;
    float maxRocketChargeDuration = 0.5f;

    //dont touch section
    float lastWallSlide = 0.0f; //dont touch
    int lastWallSlideDir = 0; //dont touch
    bool sitting = false; //dont touch
    bool animStart = false; //dont touch
    bool grounded = false; //dont touch
    bool wallSlide = false; //dont touch
    float groundHeight = 0.0f; //self explanatory
    bool wasMouseDown = false;
    float rocketStart = 0.0f;

    void update(float deltaTime, Level &level);

    void bind() const;
    void draw() const;

};

#endif //ENGINE_2_PLAYER_H
