//
// Created by Vlad on 04.09.2020.
//

#include "Player.h"

void Player::init(const std::string &filename) {
    // Orientation
    auto origin = cocos2d::Director::getInstance()->getVisibleOrigin();
    auto size = cocos2d::Director::getInstance()->getVisibleSize();

    sprite = cocos2d::Sprite::create(filename);
    sprite->setTag(TAG);
    sprite->setScale(SCALE);
    cocos2d::PhysicsBody *body = cocos2d::PhysicsBody::createBox(sprite->getContentSize(),
            cocos2d::PhysicsMaterial(0.0f, 0.0f, 1.0f));
    body->setRotationEnable(false);
    body->setDynamic(true);
    body->setLinearDamping(1.0f);
    body->setContactTestBitmask(0xFFFFFFFF);
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
        case PlayerState::MOVE_FORWARD:
            setMoveForwardState();
            break;
        case PlayerState::MOVE_LEFT:
            setMoveLeftState();
            break;
        case PlayerState::MOVE_RIGHT:
            setMoveRightState();
            break;
    }
}

void Player::setAttackState() {
    if (currentState != PlayerState::ATTACK) {
        // Change state
        currentState = PlayerState::ATTACK;

        // Run animation
        auto animation = animations[static_cast<int>(PlayerState::ATTACK)];
        auto animate = cocos2d::Animate::create(animation);
        auto callbackAnimate = cocos2d::CallFunc::create([this]() {
            currentState = PlayerState::NONE;
            setState(PlayerState::IDLE);
        });
        auto seq = cocos2d::Sequence::create(animate, callbackAnimate, nullptr);
        sprite->runAction(seq);
    }
}

void Player::setIdleState() {
    if (currentState != PlayerState::IDLE &&
            currentState != PlayerState::ATTACK) {
        // Change state
        currentState = PlayerState::IDLE;

        // Run animation
        sprite->stopAllActions(); // TODO: stop only needed actions (RUN, JUMP ...)
        auto animation = animations[static_cast<int>(PlayerState::IDLE)];
        auto animate = cocos2d::Animate::create(animation);
        sprite->runAction(cocos2d::RepeatForever::create(animate));
    }
}

void Player::setMoveForwardState() {
    if (currentState != PlayerState::MOVE_FORWARD) {
        currentState = PlayerState::MOVE_FORWARD;

        // ...
    }
}

void Player::setMoveBackwardState() {
    if (currentState != PlayerState::MOVE_BACKWARD) {
        currentState = PlayerState::MOVE_BACKWARD;
        
        // ...
    }
}

void Player::setMoveRightState() {
    if (currentState != PlayerState::MOVE_RIGHT) {
        currentState = PlayerState::MOVE_RIGHT;

        // ...
    }
}

void Player::setMoveLeftState() {
    if (currentState != PlayerState::MOVE_LEFT) {
        currentState = PlayerState::MOVE_LEFT;

        // ..
    }
}

void Player::moveRight(float t, Line *line) {
    if (isNotMoving()) {
        // Set state
        setState(PlayerState::MOVE_RIGHT);
        // Change current line
        currentLineIndex++;
        // Change position
        auto move = cocos2d::MoveTo::create(1, {line->getPosition().x, sprite->getPositionY()});
        move->setTag(static_cast<int>(PlayerState::MOVE_RIGHT));

        sprite->runAction(move);
    }
}

void Player::moveLeft(float t, Line *line) {
    if (isNotMoving()) {
        // Set state
        setState(PlayerState::MOVE_LEFT);
        // Change current line
        currentLineIndex--;
        // Change position
        auto move = cocos2d::MoveTo::create(1, {line->getPosition().x, sprite->getPositionY()});
        move->setTag(static_cast<int>(PlayerState::MOVE_LEFT));

        sprite->runAction(move);
    }
}

void Player::moveForward(float t) {
    if (isNotMoving()) {
        // Change state
        setState(PlayerState::MOVE_FORWARD);
        // Move
        auto size = cocos2d::Director::getInstance()->getVisibleSize();
        auto origin = cocos2d::Director::getInstance()->getVisibleOrigin();

        auto position = sprite->getPosition();
        if (position.y < origin.y + size.height) {
            auto move = cocos2d::MoveBy::create(t, {0, SPEED});
            move->setTag(static_cast<int>(PlayerState::MOVE_FORWARD));

            sprite->runAction(move);
        }
    }
}

void Player::moveBackward(float t) {
    if (isNotMoving()) {
        // Change state
        setState(PlayerState::MOVE_FORWARD);
        // Move
        auto size = cocos2d::Director::getInstance()->getVisibleSize();
        auto origin = cocos2d::Director::getInstance()->getVisibleOrigin();

        auto position = sprite->getPosition();
        if (position.y > origin.y) {
            auto move = cocos2d::MoveBy::create(t, {0, -SPEED});
            move->setTag(static_cast<int>(PlayerState::MOVE_BACKWARD));

            sprite->runAction(move);
        }
    }
}

void Player::attack(float t) {
    // Set state
    // setState(PlayerState::ATTACK);

    // TODO: actual attack
}

void Player::update(float t) {
    bool isMoveRight = sprite->getNumberOfRunningActionsByTag(static_cast<int>(PlayerState::MOVE_RIGHT)) > 0;
    bool isMoveLeft = sprite->getNumberOfRunningActionsByTag(static_cast<int>(PlayerState::MOVE_LEFT)) > 0;
    bool isMoveForward = sprite->getNumberOfRunningActionsByTag(static_cast<int>(PlayerState::MOVE_FORWARD)) > 0;
    bool isMoveBackward = sprite->getNumberOfRunningActionsByTag(static_cast<int>(PlayerState::MOVE_BACKWARD)) > 0;

    if (!isMoveForward && !isMoveLeft && !isMoveRight && !isMoveBackward) {
        setState(PlayerState::IDLE);
    }
}

bool Player::isNotMoving() {
    return currentState != PlayerState::MOVE_LEFT
        && currentState != PlayerState::MOVE_RIGHT
        && currentState != PlayerState::MOVE_FORWARD
        && currentState != PlayerState::MOVE_BACKWARD;
}
