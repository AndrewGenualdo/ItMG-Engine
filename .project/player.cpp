//
// Created by cobble on 12/27/2024.
//
#include "player.h"
#include "client.h"

int Player::loadCount = 0;
Player Player::players[MAX_PLAYERS]{};
Texture2d Player::texture = Texture2d();
Animation Player::anims[10];
Texture2d Player::blank = Texture2d();
Window *Player::window = nullptr;
Level Player::levels[10];
int Player::currentLevel = 0;

Player::Player() {
    texture = Texture2d();
}

Player::Player(Window *w, const std::string &tex, float scale) {
    if (Player::window == nullptr) Player::window = w;
    this->scale = scale;
    if (texture.getPath().empty()) {
        texture = Texture2d(tex + "still/0.png");
        anims[TEX_LEFT] = Animation(tex + "left/", 3, 0.1f, true);
        anims[TEX_STILL] = Animation(tex + "still/", 1, 1.0f, false);
        anims[TEX_RIGHT] = Animation(tex + "right/", 3, 0.1f, true);
        anims[TEX_MOUSE_GRABBED] = Animation(tex + "mouse_drag/", 8, 0.05f, true);
        anims[TEX_SIT] = Animation(tex + "sit/", 4, 0.02f, false);
        anims[TEX_SLIDE_LEFT] = Animation(tex + "slide_left/", 3, 0.05f, false);
        anims[TEX_SLIDE_RIGHT] = Animation(tex + "slide_right/", 3, 0.05f, false);
        anims[TEX_WALL_SLIDE_LEFT] = Animation(tex + "wall_slide_left/", 1, 1.0f, false);
        anims[TEX_WALL_SLIDE_RIGHT] = Animation(tex + "wall_slide_right/", 1, 1.0f, false);
        anims[TEX_ROCKET_CHARGE] = Animation(tex + "rocket_charge/", 4, maxRocketChargeDuration / 4.0f, false);
    }
    if(blank.getPath().empty()) {
        blank = Texture2d("assets/textures/ui/blank.png");
    }
    for(int i=0;i<KEY_COUNT;i++) {
        keys[i] = false;
        wasKeys[i] = false;
    }
    hurtbox = Hitbox(pos.x, pos.y, 42 * scale, 90 * scale);

    groundAcceleration *= scale * 1.5f;
    groundFriction *= scale;
    airAcceleration *= scale;
    airResistance *= scale;
    gravity *= scale;
    jumpVel *= scale;
    startSpeed *= scale;
    wallJumpHorizVel *= scale;
    wallJumpVertVel *= scale;

    loadCount++;
}

void Player::update(float deltaTime, Level &level) {
    auto time = window->getTime();
    bool mousePressed = !wasMouseDown && keys[MOUSE_LEFT];
    bool mouseReleased = wasMouseDown && !keys[MOUSE_LEFT];
    wasMouseDown = keys[MOUSE_LEFT];
    animStart = false;

    bool readyToDash = abs(velocity.x) < startSpeed;

    float floorHeight = groundHeight;
    platStandingOn = -1;

    //move left
    if (keys[KEY_LEFT]) {
        //if left and right are pressed, dont move
        if (!keys[KEY_RIGHT]) {
            //initial dash speed
            if (grounded) {
                if (readyToDash) velocity.x = -startSpeed;
                velocity.x -= groundAcceleration * deltaTime;
            } else {
                velocity.x -= airAcceleration * deltaTime;
            }
            facing = -1;
        } else {
            facing = 0;
        }
        //move right
    } else if (keys[KEY_RIGHT]) {
        //initial dash speed
        if (grounded) {
            if (readyToDash) velocity.x = startSpeed;
            velocity.x += groundAcceleration * deltaTime;
        } else {
            velocity.x += airAcceleration * deltaTime;
        }
        facing = 1;
    } else {
        facing = 0;
    }

    /*//drag little dude around
    if (keys[MOUSE_LEFT]) {
        pos.x = window->gX(window->mousePos.x);
        pos.y = window->gY(window->getHeight() - window->mousePos.y);
        velocity.x = 0;
        velocity.y = 0;
    }*/
    if(mousePressed) {
        rocketStart = time;
        animStart = true;
    }
    if(mouseReleased) {
        float chargeTime = time - rocketStart;
        if(chargeTime > maxRocketChargeDuration) chargeTime = maxRocketChargeDuration;
        float t = chargeTime / maxRocketChargeDuration;
        float rocketSpeed = (t * (maxRocketSpeed - minRocketSpeed) + minRocketSpeed);
        vec2 launchVel = normalize(pos - vec2(mousePos.x, mousePos.y)) * rocketSpeed;
        velocity += launchVel;
    }

    grounded = false;
    wallSlide = false;

    float newX = pos.x + velocity.x * deltaTime;
    float newY = pos.y + velocity.y * deltaTime;
    int platCounter = 0;
    for (auto &plat: level.platforms) {
        if (!keys[KEY_DOWN] || plat.solid) {
            if (hurtbox.x + hurtbox.w >= plat.x && hurtbox.x <= plat.x + plat.w) {
                if (newY <= plat.y + plat.h && pos.y >= plat.y + plat.h - 1) { //above -> below
                    floorHeight = plat.y + plat.h;
                    grounded = true;
                    platStandingOn = platCounter;
                    //break;
                } else if(plat.solid && newY + hurtbox.h >= plat.y && pos.y + hurtbox.h <= plat.y + 1) { //below -> above
                    velocity.y = 0;
                    pos.y = plat.y - hurtbox.h;
                    //break;
                }
            }
            if(plat.solid && hurtbox.y + hurtbox.h > plat.y && hurtbox.y <= plat.y + plat.h) {
                if(newX <= plat.x + plat.w && pos.x >= plat.x + plat.w - 1) { //right -> left
                    velocity.x = 0;
                    pos.x = plat.x + plat.w;
                    wallSlide = true;
                    lastWallSlide = time;
                    //break;
                } else if(newX + hurtbox.w >= plat.x && pos.x + hurtbox.w <= plat.x + 1) { //left -> right
                    velocity.x = 0;
                    pos.x = plat.x - hurtbox.w ;
                    wallSlide = true;
                    lastWallSlide = time;
                    //break;
                }
            }
            platCounter++;
        }

    }

    if (!grounded) grounded = pos.y <= groundHeight + 1;


    bool wasSitting = sitting;
    sitting = false;

    //crouching
    if (grounded) {
        if (keys[KEY_DOWN]) {
            sitting = true;
            if (!wasSitting) animStart = true;
        }
    }

    //wall jump
    if(wallSlide && facing != 0) {
        lastWallSlideDir = facing;
    }
    if(glfwGetTime() - lastWallSlide <= wasWallSlideTime) {
        if(keys[KEY_JUMP] && !wasKeys[KEY_JUMP]) {
            velocity += vec2(static_cast<float>(-lastWallSlideDir) * wallJumpHorizVel, wallJumpVertVel);
            lastWallSlide = 0.0f;
        }
    }


    if (!grounded) {
        //gravity
        velocity.y -= gravity * deltaTime;
        //wall slide reduction
        if(wallSlide && facing != 0) {
            velocity.y *= wallSlideFriction;
        }

        //air resistance
        if (velocity.x > 0) {
            velocity.x = glm::max(0.0f, velocity.x - airResistance * deltaTime);
        } else if (velocity.x < 0) {
            velocity.x = glm::min(0.0f, velocity.x + airResistance * deltaTime);
        }
    } else {
        //on ground adjustments
        pos.y = floorHeight;
        velocity.y = 0;

        //ground friction
        float groundFric = groundFriction * (sitting ? 2.5f : 1.0f);
        if (velocity.x > 0) {
            velocity.x = glm::max(0.0f, velocity.x - groundFric * deltaTime);
        } else if (velocity.x < 0) {
            velocity.x = glm::min(0.0f, velocity.x + groundFric * deltaTime);
        }

        //jump
        if (keys[KEY_JUMP]) {
            velocity.y = jumpVel;
        }
    }



    //don't leave the screen
    /*if (pos.x < 0) {
        pos.x = 0;
        velocity.x = 0;
    } else if (pos.x - hurtbox.w > Window::GAME_WIDTH) {
        pos.x = Window::GAME_WIDTH - hurtbox.w;
        velocity.x = 0;
    }*/

    //apply velocity
    pos.x += velocity.x * deltaTime;
    pos.y += velocity.y * deltaTime;

    //set hurtbox stuff
    hurtbox.setPos(pos.x + 3, pos.y - 1);



    //update Client for server
    if (id == Client::id) {
        Client::setPos(pos.x, pos.y);
        Client::setVel(velocity.x, velocity.y);
        Client::setMousePos(mousePos.x, mousePos.y);
    }

    //update wasKeys
    for(int i=0;i<KEY_COUNT;i++) {
        wasKeys[i] = keys[i];
    }

}

void Player::bind() const {
    if (keys[MOUSE_LEFT]) {
        anims[TEX_ROCKET_CHARGE].bindSingle(window->getTime(), animStart);
        //anims[TEX_MOUSE_GRABBED].bindLoop(window->getTime());
    } else if (sitting && velocity.x > startSpeed) {
        anims[TEX_SLIDE_RIGHT].bindSingle(window->getTime(), animStart);
    } else if (sitting && velocity.x < -startSpeed) {
        anims[TEX_SLIDE_LEFT].bindSingle(window->getTime(), animStart);
    } else if(wallSlide && facing > 0) {
        anims[TEX_WALL_SLIDE_LEFT].bindLoop(window->getTime());
    } else if(wallSlide && facing < 0) {
        anims[TEX_WALL_SLIDE_RIGHT].bindLoop(window->getTime());
    } else if (facing > 0) {
        anims[TEX_RIGHT].bindLoop(window->getTime());
    } else if (facing < 0) {
        anims[TEX_LEFT].bindLoop(window->getTime());
    } else if (sitting) {
        anims[TEX_SIT].bindSingle(window->getTime(), animStart);
    } else {
        anims[TEX_STILL].bindLoop(window->getTime());
    }
}

void Player::draw() const {
    float nx = pos.x;
    float ny = pos.y;
    if(wallSlide) {
        if(facing > 0) {
            nx -= 9.0f * scale; // 3/2 of a guy pixel
        } else if(facing < 0) {
            nx += 3.0f * scale; // 1/2 of a guy pixel;
        }
    }
    Texture2d::setColor(playerColors[id]);
    bind();

    if(keys[MOUSE_LEFT]) {
        /*blank.bind();
        float chargeTime = window->getTime() - rocketStart;
        if(chargeTime > maxRocketChargeDuration) chargeTime = maxRocketChargeDuration;
        float t = chargeTime / maxRocketChargeDuration;
        Texture2d::setColor(vec4(1, 0, 0, 1));
        blank.draw(pos.x, pos.y - 5, hurtbox.w, 5.0f);
        Texture2d::setColor(vec4(0, 1, 0, 1));
        blank.draw(pos.x, pos.y - 5, t * hurtbox.w, 5.0f);*/
        //float mx = window->gX(window->mousePos.x);
        //float my = window->gY(window->getHeight() - window->mousePos.y);
        //vec2 m = vec2(mx, my);
        float angle = radians(360.0f) - atan(mousePos.y - pos.y, mousePos.x - pos.x) - radians(90.0f);

        texture.draw(nx, ny + 41 * scale, 90 * scale, 90 * scale, angle, false);
    } else {
        texture.draw(nx, ny + 41 * scale, 90 * scale, 90 * scale, false);
    }


    if(Client::debug) hurtbox.draw(vec4(1, 0, 0, 0.2f));
}



