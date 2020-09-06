//
// Created by Vlad on 04.09.2020.
//

#include "Player.h"
void Player::init(const std::string &filename, const cocos2d::Vec2 &position) {
    // Orientation
    auto origin = cocos2d::Director::getInstance()->getVisibleOrigin();
    auto size = cocos2d::Director::getInstance()->getVisibleSize();

    sprite = cocos2d::Sprite::create(filename);
    sprite->setScale(PLAYER_SCALE);
    sprite->setPosition(position);
    cocos2d::PhysicsBody *body = cocos2d::PhysicsBody::createBox(sprite->getContentSize(),
            cocos2d::PhysicsMaterial(0.0f, 0.0f, 1.0f));
    body->setRotationEnable(false);
    body->setDynamic(true);
    body->setLinearDamping(1.0f);
    sprite->setPhysicsBody(body);

    // Add components to node
    this->addChild(sprite);
}

void Player::show(float d) {
    auto showAction = cocos2d::FadeIn::create(d);
    sprite->runAction(showAction);
}

void Player::hide(float d) {
    auto hideAction = cocos2d::FadeOut::create(d);
    sprite->runAction(hideAction);
}
// NOTE: rofling with VIKA in discord, check this becouse it can be wrong
void Player::addAnimation(PlayerState state, cocos2d::Animation *animation) {
    animations.insert(std::pair<int, cocos2d::Animation *>(static_cast<int>(state), animation));
}

void Player::setState(PlayerState state) {
    switch (state) {
        case PlayerState::NONE:
            break;
        case PlayerState::IDLE:
            setIdleState();
            break;
        case PlayerState::ATTACK:
            setAttackState();
            break;
        case PlayerState::RUN:
            setRunState();
            break;
    }
}
void Player::setAttackState() {
    if (currentPlayerState != PlayerState::ATTACK) {
        currentPlayerState = PlayerState::ATTACK;
    }
}
void Player::setIdleState() {
    if (currentPlayerState != PlayerState::IDLE) {
        // Change state
        currentPlayerState = PlayerState::IDLE;

        // Run animation
        sprite->stopAllActions(); // TODO: stop only needed actions (RUN, JUMP ...)
        auto animation = animations[static_cast<int>(PlayerState::IDLE)];
        auto animate = cocos2d::Animate::create(animation);
        sprite->runAction(cocos2d::RepeatForever::create(animate));
    }
}
void Player::setRunState() {
    if (currentPlayerState != PlayerState::RUN) {
        // Change state
        currentPlayerState = PlayerState::RUN;

        // Run animation
        sprite->stopAllActions(); // TODO: stop only needed actions (RUN, JUMP ...)
        auto animation = animations[static_cast<int>(PlayerState::RUN)];
        auto animate = cocos2d::Animate::create(animation);
        sprite->runAction(cocos2d::RepeatForever::create(animate));
    }
}

void Player::move(float t, cocos2d::Vec2 &position) {
    auto body = sprite->getPhysicsBody();
    body->applyForce(position * PLAYER_SPEED);

    setState(PlayerState::RUN);
}