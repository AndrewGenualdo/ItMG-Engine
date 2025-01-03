//
// Created by cobble on 12/27/2024.
//

#ifndef ENGINE_2_ANIMATION_H
#define ENGINE_2_ANIMATION_H

#include "texture2d.h"

using namespace cobb;
using namespace glm;

class Animation {
    Texture2d *frames;
    float delay;
    int frameCount;
    bool loop;
    float startTime;

public:
    Animation() = default;
    Animation(const std::string& folder, int frames, float delay, bool loop) {
        frameCount = frames;
        this->frames = new Texture2d[frameCount];
        for(int i=0;i<frameCount;i++) {
            this->frames[i] = Texture2d(folder+std::to_string(i)+".png");
        }
        this->delay = delay;
        this->loop = loop;
        startTime = 0.0f;
    }

    void bindSingle(float time, bool start) {
        if(start) startTime = time;
        float t = time - startTime;
        float totalLength = static_cast<float>(frameCount) * delay;
        int frame = (int) (static_cast<float>(frameCount) * t / totalLength);
        if(frame >= frameCount) frame = frameCount - 1;
        frames[frame].bind();
    }

    void bindLoop(float time) {
        float totalLength = static_cast<float>(frameCount) * delay;
        int cycle = (int) (time / totalLength);
        float t = time - totalLength * static_cast<float>(cycle);
        int frame = (int) (static_cast<float>(frameCount) * t / totalLength);
        frames[frame].bind();
    }

    void draw(float time, float x, float y, float width, float height, bool shouldBind = true) {
        if(shouldBind) bindLoop(time);
        if(frameCount > 0) frames[0].draw(x, y, width, height, false);
    }


};


#endif //ENGINE_2_ANIMATION_H
