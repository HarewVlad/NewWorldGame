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
    MOVE,
    ATTACK,
};

// TODO: inheritance from Character
struct Player : public cocos2d::Node {
    void init(const std::string &filename);
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
    inline void setSpawnData(const PlayerSpawnData &playerSpawnData) {
        sprite->setPosition(playerSpawnData.position);
        currentLineIndex = playerSpawnData.lineIndex;
    }
    inline int getCurrentLine() const {
        return currentLineIndex;
    }
    void setState(PlayerState state);
    void moveToLine(float t, Line *line);
    void moveForward(float t, float value);
    void attack(float t);
    void update(float t);
private:
    void setAttackState();
    void setIdleState();
    void setRunState();
private:
    cocos2d::Sprite *sprite;
    std::unordered_map<int, cocos2d::Animation *> animations;
    Level *currentLevel;
    int currentLineIndex;
    PlayerState currentPlayerState;

    const float PLAYER_SCALE = 2.0f;
    const float PLAYER_SPEED = 100.0f;
};


#endif //PROJ_ANDROID_PLAYER_H
