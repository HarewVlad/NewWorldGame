//
// Created by Vlad on 04.09.2020.
//

#include "Player.h"

void Player::init(const std::string &filename) {
    // Orientation
    auto origin = cocos2d::Director::getInstance()->getVisibleOrigin();
    auto size = cocos2d::Director::getInstance()->getVisibleSize();

    sprite = cocos2d::Sprite::create(filename);
    sprite->setScale(PLAYER_SCALE);
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
        case PlayerState::MOVE:
            setRunState();
            break;
    }
}

void Player::setAttackState() {
    if (currentPlayerState != PlayerState::ATTACK) {
        // Change state
        currentPlayerState = PlayerState::ATTACK;

        // Run animation
        auto animation = animations[static_cast<int>(PlayerState::ATTACK)];
        auto animate = cocos2d::Animate::create(animation);
        auto callbackAnimate = cocos2d::CallFunc::create([this]() {
            currentPlayerState = PlayerState::NONE;
            setState(PlayerState::IDLE);
        });
        auto seq = cocos2d::Sequence::create(animate, callbackAnimate, nullptr);
        sprite->runAction(seq);
    }
}

void Player::setIdleState() {
    if (currentPlayerState != PlayerState::IDLE &&
        currentPlayerState != PlayerState::ATTACK) {
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
    if (currentPlayerState != PlayerState::MOVE &&
        currentPlayerState != PlayerState::ATTACK) {
        // Change state
        currentPlayerState = PlayerState::MOVE;

        // Run animation
        sprite->stopAllActions(); // TODO: stop only needed actions (RUN, JUMP ...)
        auto animation = animations[static_cast<int>(PlayerState::MOVE)];
        auto animate = cocos2d::Animate::create(animation);
        sprite->runAction(cocos2d::RepeatForever::create(animate));
    }
}

void Player::moveToLine(float t, Line *line) {
    auto body = sprite->getPhysicsBody();

    body->setVelocity({PLAYER_SPEED, 0});
}

void Player::moveForward(float t, float value) {
    auto body = sprite->getPhysicsBody();

    body->setVelocity({0, PLAYER_SPEED * value});
}

void Player::attack(float t) {
    // Set state
    setState(PlayerState::ATTACK);

    // TODO: actual attack
}

void Player::update(float t) {
    auto body = sprite->getPhysicsBody();
    auto bodyVelocity = body->getVelocity();

    if (bodyVelocity.isZero()) {
        setState(PlayerState::IDLE);
    }
}