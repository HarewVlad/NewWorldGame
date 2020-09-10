//
// Created by Vlad on 10.09.2020.
//

#include "Enemy.h"

void Enemy::init(const std::string &filename, const cocos2d::Vec2 &position) {
    // Orientation
    auto origin = cocos2d::Director::getInstance()->getVisibleOrigin();
    auto size = cocos2d::Director::getInstance()->getVisibleSize();

    sprite = cocos2d::Sprite::create(filename);
    sprite->setScale(ENEMY_SCALE);
    sprite->setPosition(position);
    cocos2d::PhysicsBody *body = cocos2d::PhysicsBody::createBox(sprite->getContentSize(),
                                                                 cocos2d::PhysicsMaterial(0.0f, 0.0f, 1.0f));
    body->setRotationEnable(false);
    body->setDynamic(true);
    body->setLinearDamping(1.0f);
    body->setVelocityLimit(ENEMY_WALK_SPEED);
    sprite->setPhysicsBody(body);

    // Add components to node
    this->addChild(sprite);
}