//
// Created by Vlad on 06.09.2020.
//

#ifndef PROJ_ANDROID_PLAYERLAYER_H
#define PROJ_ANDROID_PLAYERLAYER_H

#include "cocos2d.h"
#include "Player.h"

struct PlayerLayer : public cocos2d::Layer {
    void init(Player *player);
private:
    Player *player;
};


#endif //PROJ_ANDROID_PLAYERLAYER_H
