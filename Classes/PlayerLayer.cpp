//
// Created by Vlad on 06.09.2020.
//

#include "PlayerLayer.h"

void PlayerLayer::init(Player *player, cocos2d::Node *node) {
    this->addChild(node);
    this->addChild(player->getSprite());

    auto follow = cocos2d::Follow::create(player->getSprite());
    this->runAction(follow);
}