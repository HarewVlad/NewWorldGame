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
    ATTACK,
    JUMP,
    FALL
};

struct Player : public cocos2d::Node {
    void init(const std::string &filename, const cocos2d::Vec2 &position);
    void show(float d = 0);
    void hide(float d = 0);
    void addAnimation(PlayerState state, cocos2d::Animation *animation);
    inline PlayerState getState() {
        return currentPlayerState;
    }
    inline cocos2d::Sprite *getSprite() {
        return sprite;
    }
    inline cocos2d::Vec2 getPosition() {
        return sprite->getPosition();
    }
    void setState(PlayerState state);
    void move(float t, cocos2d::Vec2 &position);
    void attack(float t);
private:
    void setAttackState();
    void setIdleState();
    void setRunState();
    void setJumpState();
    void setFallState();
private:
    cocos2d::Sprite *sprite;
    std::unordered_map<int, cocos2d::Animation *> animations;
    PlayerState currentPlayerState;

    const float PLAYER_SCALE = 2.0f;
    const float PLAYER_WALK_SPEED = 200.0f;
    const float PLAYER_JUMP_SPEED = 2000.0f;
};


#endif //PROJ_ANDROID_PLAYER_H
