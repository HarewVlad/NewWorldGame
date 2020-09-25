//
// Created by Vlad on 04.09.2020.
//

#ifndef PROJ_ANDROID_PLAYER_H
#define PROJ_ANDROID_PLAYER_H

#include "cocos2d.h"
#include "Level.h"

enum class PlayerState {
    NONE,
    IDLE,
    MOVE_FORWARD,
    MOVE_BACKWARD,
    MOVE_RIGHT,
    MOVE_LEFT,
    ATTACK,
};

// TODO: inheritance from Character
struct Player : public cocos2d::Node {
    void init(const std::string &filename);
    void show(float d = 0);
    void hide(float d = 0);
    void addAnimation(PlayerState state, cocos2d::Animation *animation);
    inline PlayerState getState() {
        return currentState;
    }
    inline cocos2d::Sprite *getSprite() {
        return sprite;
    }
    inline cocos2d::Vec2 getPosition() {
        return sprite->getPosition();
    }
    inline void setSpawnData(const PlayerSpawnData &playerSpawnData) {
        sprite->setPosition(playerSpawnData.position);
        currentLineIndex = playerSpawnData.lineIndex;
    }
    inline int getCurrentLineIndex() const {
        return currentLineIndex;
    }
    inline int getTag() const {
        return TAG;
    }
    void setState(PlayerState state);
    void moveRight(float t, Line *line);
    void moveLeft(float t, Line *line);
    void moveForward(float t);
    void moveBackward(float t);
    void attack(float t);
    void update(float t);
private:
    void setAttackState();
    void setIdleState();
    void setMoveForwardState();
    void setMoveBackwardState();
    void setMoveRightState();
    void setMoveLeftState();
    bool isNotMoving();
private:
    cocos2d::Sprite *sprite;
    std::unordered_map<int, cocos2d::Animation *> animations;
    Level *currentLevel;
    int currentLineIndex;
    PlayerState currentState;

    const float SCALE = 1.0f;
    const float SPEED = 1.0f;
    const int TAG = 0x33;
};


#endif //PROJ_ANDROID_PLAYER_H
