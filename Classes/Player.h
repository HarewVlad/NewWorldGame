//
// Created by Vlad on 04.09.2020.
//

#ifndef PROJ_ANDROID_PLAYER_H
#define PROJ_ANDROID_PLAYER_H

#include "cocos2d.h"

enum class PlayerState {
    NONE,
    IDLE,
    RUN,
    ATTACK
};

const float PLAYER_SPEED = 0.5f;

struct Player : public cocos2d::Node {
    void init(const std::string &filename, const cocos2d::Vec2 &position);
    void show(float d = 0);
    void hide(float d = 0);
    void addAnimation(PlayerState state, cocos2d::Animation *animation);
    inline PlayerState getState() {
        return currentPlayerState;
    }
    void setState(PlayerState state);
    void setAttackState();
    void setIdleState();
    void setRunState();
    void move(float t, cocos2d::Vec2 &position);
private:
    cocos2d::Sprite *sprite;
    std::unordered_map<int, cocos2d::Animation *> animations;
    PlayerState currentPlayerState;

    const int PLAYER_SCALE = 2.0f;
};


#endif //PROJ_ANDROID_PLAYER_H
