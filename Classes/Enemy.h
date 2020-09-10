//
// Created by Vlad on 10.09.2020.
//

#ifndef PROJ_ANDROID_ENEMY_H
#define PROJ_ANDROID_ENEMY_H

#include "cocos2d.h"

// TODO: inheritance from Character
struct Enemy : public cocos2d::Node {
    void init(const std::string &filename, const cocos2d::Vec2 &position);
private:
    cocos2d::Sprite *sprite;

    const float ENEMY_SCALE = 2.0f;
    const float ENEMY_WALK_SPEED = 200.0f;
    const float ENEMY_JUMP_SPEED = 2000.0f;
};


#endif //PROJ_ANDROID_ENEMY_H
