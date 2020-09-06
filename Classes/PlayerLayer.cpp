//
// Created by Vlad on 06.09.2020.
//

#include "PlayerLayer.h"

void PlayerLayer::init(Player *player) {
    this->addChild(player);

    auto follow = cocos2d::Follow::create(player);
    this->runAction(follow);
}