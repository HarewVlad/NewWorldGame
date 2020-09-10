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
    body->setVelocityLimit(PLAYER_WALK_SPEED);
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
        case PlayerState::JUMP:
            setJumpState();
            break;
        case PlayerState::FALL:
            setFallState();
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
    if (currentPlayerState != PlayerState::RUN &&
        currentPlayerState != PlayerState::ATTACK) {
        // Change state
        currentPlayerState = PlayerState::RUN;

        // Run animation
        sprite->stopAllActions(); // TODO: stop only needed actions (RUN, JUMP ...)
        auto animation = animations[static_cast<int>(PlayerState::RUN)];
        auto animate = cocos2d::Animate::create(animation);
        sprite->runAction(cocos2d::RepeatForever::create(animate));
    }
}

void Player::setJumpState() {
    if (currentPlayerState != PlayerState::JUMP &&
        currentPlayerState != PlayerState::ATTACK) {
        // Change state
        currentPlayerState = PlayerState::JUMP;

        // Run animation
        sprite->stopAllActions();
        auto animation = animations[static_cast<int>(PlayerState::JUMP)];
        auto animate = cocos2d::Animate::create(animation);
        sprite->runAction(cocos2d::RepeatForever::create(animate));
    }
}

void Player::setFallState() {
    if (currentPlayerState != PlayerState::FALL &&
        currentPlayerState != PlayerState::ATTACK) {
        // Change state
        currentPlayerState = PlayerState::FALL;

        // Run animation
        sprite->stopAllActions();
        auto animation = animations[static_cast<int>(PlayerState::FALL)];
        auto animate = cocos2d::Animate::create(animation);
        sprite->runAction(cocos2d::RepeatForever::create(animate));
    }
}

void Player::move(float t, cocos2d::Vec2 &position) {
    // Flip
    sprite->setFlippedX(position.x < 0);

    auto body = sprite->getPhysicsBody();
    auto bodyVelocity = body->getVelocity();
    // Set state // TODO: possible move this part to player->update(t);
    if (bodyVelocity.y > 0) {
        setState(PlayerState::JUMP);
    } else if (bodyVelocity.y < 0){
        setState(PlayerState::FALL);
    } else {
        setState(PlayerState::RUN);
    }

    // Change position
    if (bodyVelocity.y == 0 && position.x < 0.3f && position.y >= 0.5f) { // Jump UP
        body->applyForce(cocos2d::Vec2(0, PLAYER_JUMP_SPEED));
    } else if (bodyVelocity.y == 0 && position.x >= 0.5f && position.y >= 0.5f) { // Jump N-E
        body->applyForce(cocos2d::Vec2(PLAYER_WALK_SPEED, PLAYER_JUMP_SPEED));
    } else if (bodyVelocity.y == 0 && position.x <= -0.5f && position.y >= 0.5f) { // Jump N-W
        body->applyForce(cocos2d::Vec2(-PLAYER_WALK_SPEED, PLAYER_JUMP_SPEED));
    } else if (position.x > 0) { // Move right
        body->applyForce(cocos2d::Vec2(PLAYER_WALK_SPEED, 0));
    } else { // Move left
        body->applyForce(cocos2d::Vec2(-PLAYER_WALK_SPEED, 0));
    }
}

void Player::attack(float t) {
    // Set state
    setState(PlayerState::ATTACK);

    // TODO: actual attack
}