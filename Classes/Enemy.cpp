//
// Created by Vlad on 10.09.2020.
//

#include "Enemy.h"

// TODO: make EnumCustom with proper way of converting enum to string using templates
std::string enumToString(EnemyType type) {
  switch (type) {
  case EnemyType::BANDIT:
    return "Bandit";
  case EnemyType::BEAR:
    return "Bear";
  case EnemyType::ENT:
    return "Ent";
  case EnemyType::GOLEM:
    return "Golem";
  case EnemyType::MUSHROOM:
    return "Mushroom";
  }
}

int Enemy::getFrameCount(EnemyType type) {
  switch (type) {
  case EnemyType::BANDIT:
    return 4;
  case EnemyType::BEAR:
    return 4;
  case EnemyType::ENT:
    return 4;
  case EnemyType::GOLEM:
    return 6;
  case EnemyType::MUSHROOM:
    return 4;
  }
}

Enemy* Enemy::create(EnemyType type) {
  Enemy *node = new (std::nothrow)Enemy();
  if (node && node->init(type))
  {
    node->autorelease();
    return node;
  }

  CC_SAFE_DELETE(node);
  return nullptr;
}

bool Enemy::init(EnemyType type) {
  std::string typeAsString = enumToString(type);
  std::string mainSpriteSource = typeAsString + ".png";
  std::string walkConfig = typeAsString + "_Walk_%d.png";
  std::string walkCache = "Enemies/" + typeAsString + "_Walk.plist";

  // Main sprite
  sprite = cocos2d::Sprite::create(mainSpriteSource);
  sprite->setScale(SCALE);
  cocos2d::PhysicsBody *body = cocos2d::PhysicsBody::createBox(
    sprite->getContentSize(), cocos2d::PhysicsMaterial(0.0f, 0.0f, 1.0f));
  body->setDynamic(false);
  body->setContactTestBitmask(0xFFFFFFFF);

  // sprite->setScale(scale);
  sprite->setPhysicsBody(body);

  // Cache
  cocos2d::SpriteFrameCache::getInstance()->addSpriteFramesWithFile(
    walkCache);

  // Animations
  // Walk
  {
    auto frames = getSpriteFrames(walkConfig.c_str(), getFrameCount(type));
    auto animation =
      cocos2d::Animation::createWithSpriteFrames(frames, 1 / static_cast<float>(getFrameCount(type)));
    animation->retain();

    animations[static_cast<int>(EnemyState::WALK)] = animation;
  }
  
  this->addChild(sprite);

  // this->scheduleUpdate();

  return true;
}

void Enemy::setStop() {
  if (currentState != EnemyState::IDLE) {
    currentState = EnemyState::IDLE;

    sprite->stopAllActions();
  }
}

void Enemy::setStart() {
  if (currentState != EnemyState::WALK) {
    currentState = EnemyState::WALK;

    // Movement
    auto moveDown = cocos2d::MoveBy::create(MOVE_DURATION, { 0, -SPEED});
    this->runAction(cocos2d::RepeatForever::create(moveDown));

    // Play animation
    sprite->stopAllActions();
    auto animation = animations[static_cast<int>(EnemyState::WALK)];
    auto animate = cocos2d::Animate::create(animation);
    sprite->runAction(cocos2d::RepeatForever::create(animate));
  }
}

void Enemy::update(float t) {
  /*
  auto visibleSize = cocos2d::Director::getInstance()->getVisibleSize();

  if (this->getPositionY() <= -visibleSize.height * 1.5f) {
    this->removeFromParentAndCleanup(true);
  }
  */
}

int Enemy::getEnemiesCount(){
  return static_cast<int>(EnemyType::LAST_ELEMENT);
}