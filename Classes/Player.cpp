//
// Created by Vlad on 04.09.2020.
//

#include "Player.h"

#define IS_OFFSET_ENABLED

void Player::init(const std::string &filename) {
  // Orientation
  auto origin = cocos2d::Director::getInstance()->getVisibleOrigin();
  auto size = cocos2d::Director::getInstance()->getVisibleSize();

  sprite = cocos2d::Sprite::create(filename);
  sprite->setScale(SCALE);
  sprite->setTag(TAG);
#ifdef IS_OFFSET_ENABLED
  cocos2d::PhysicsBody *body = cocos2d::PhysicsBody::createBox(
    getExpandZone().size, cocos2d::PhysicsMaterial(0.0f, 0.0f, 1.0f), { sprite->getContentSize().width, sprite->getContentSize().height / 2.5f });
#else
  cocos2d::PhysicsBody *body = cocos2d::PhysicsBody::createBox(
    getExpandZone().size, cocos2d::PhysicsMaterial(0.0f, 0.0f, 1.0f));
#endif
  body->setRotationEnable(false);
  body->setDynamic(false);
  body->setContactTestBitmask(0xFFFFFFFF);
  sprite->setPhysicsBody(body);

  this->scheduleUpdate();

  // Add components to node
  this->addChild(sprite);
}

void Player::addAnimation(PlayerState state, cocos2d::Animation *animation) {
  animations.insert(
      std::pair<int, cocos2d::Animation *>(static_cast<int>(state), animation));
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
    sprite->stopAllActions();  // TODO: stop only needed actions (RUN, JUMP ...)
    auto animation = animations[static_cast<int>(PlayerState::IDLE)];
    auto animate = cocos2d::Animate::create(animation);
    sprite->runAction(cocos2d::RepeatForever::create(animate));
  }
}

void Player::setMoveRightState() {
  if (currentState != PlayerState::MOVE_RIGHT) {
    currentState = PlayerState::MOVE_RIGHT;

    // Run animation
    sprite->stopAllActions();  // TODO: stop only needed actions (RUN, JUMP ...)
    auto animation = animations[static_cast<int>(PlayerState::MOVE_RIGHT)];
    auto animate = cocos2d::Animate::create(animation);
    sprite->runAction(cocos2d::RepeatForever::create(animate));
  }
}

void Player::setMoveLeftState() {
  if (currentState != PlayerState::MOVE_LEFT) {
    currentState = PlayerState::MOVE_LEFT;

    // Run animation
    sprite->stopAllActions();  // TODO: stop only needed actions (RUN, JUMP ...)
    auto animation = animations[static_cast<int>(PlayerState::MOVE_LEFT)];
    auto animate = cocos2d::Animate::create(animation);
    sprite->runAction(cocos2d::RepeatForever::create(animate));
  }
}

void Player::moveRight(Line *line) {
  if (isNotMoving()) {
    // Set state
    setState(PlayerState::MOVE_RIGHT);
    // Change current line
    currentLineIndex++;
    // Change position
    auto move = cocos2d::MoveTo::create(
        2, {line->getPositionX(), this->getPositionY()});
    move->setTag(static_cast<int>(PlayerState::MOVE_RIGHT));

    this->runAction(move);
  }
}

void Player::moveLeft(Line *line) {
  if (isNotMoving()) {
    // Set state
    setState(PlayerState::MOVE_LEFT);
    // Change current line
    currentLineIndex--;
    // Change position
    auto move = cocos2d::MoveTo::create(
        2, {line->getPositionX(), this->getPositionY()});
    move->setTag(static_cast<int>(PlayerState::MOVE_LEFT));

    this->runAction(move);
  }
}

void Player::attack(float t) {
  // Set state
  // setState(PlayerState::ATTACK);

  // TODO: actual attack
}

void Player::update(float t) {
  bool isMoveRight = this->getNumberOfRunningActionsByTag(
                         static_cast<int>(PlayerState::MOVE_RIGHT)) > 0;
  bool isMoveLeft = this->getNumberOfRunningActionsByTag(
                        static_cast<int>(PlayerState::MOVE_LEFT)) > 0;

  if (!isMoveLeft && !isMoveRight) {
    setState(PlayerState::IDLE);
  }
}

bool Player::isNotMoving() {
  return currentState != PlayerState::MOVE_LEFT &&
         currentState != PlayerState::MOVE_RIGHT;
}

cocos2d::Rect Player::getExpandZone() {
  return cocos2d::Rect(
    this->getPositionX() + sprite->getPositionX() - sprite->getContentSize().width * 0.5f - EXPAND_ZONE_OFFSET,
    this->getPositionY() + sprite->getPositionY() - sprite->getContentSize().height * 0.5f - EXPAND_ZONE_OFFSET,
    sprite->getContentSize().width + EXPAND_ZONE_OFFSET * 2.0f,
    sprite->getContentSize().height + EXPAND_ZONE_OFFSET * 2.0f);
}

cocos2d::Rect Player::getBoundingBox() {
  return cocos2d::Rect(
    this->getPositionX() + sprite->getPositionX() - sprite->getContentSize().width * 0.5f,
    this->getPositionY() + sprite->getPositionY() - sprite->getContentSize().height * 0.5f,
    sprite->getContentSize().width,
    sprite->getContentSize().height);
}

void Player::setHit() {
  // Disable collisions when hit
  sprite->getPhysicsBody()->setEnabled(false);

  auto blinkCallback = cocos2d::CallFunc::create([this]() {
    // When blinking finished - enable
    sprite->getPhysicsBody()->setEnabled(true);
  });

  auto blink = cocos2d::Blink::create(1, 3);
  auto seq = cocos2d::Sequence::create(blink, blinkCallback, nullptr);
  this->runAction(seq);
}