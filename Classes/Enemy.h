//
// Created by Vlad on 10.09.2020.
//

#ifndef PROJ_ANDROID_ENEMY_H
#define PROJ_ANDROID_ENEMY_H

#include "Utils.h"
#include "cocos2d.h"

enum class EnemyType {
  BANDIT, BEAR, ENT, GOLEM, MUSHROOM, LAST_ELEMENT
};

enum class EnemyState {
  IDLE,
  WALK
};

static std::string enumToString(EnemyType type);

class Enemy : public cocos2d::Node {
public:
  void init(EnemyType type);
  void setStop();
  void setStart();
  static int getEnemiesCount();
private:
  int getFrameCount(EnemyType type);
 private:
  cocos2d::Sprite *sprite;
  std::unordered_map<int, cocos2d::Animation *> animations;

  EnemyState currentState;
};

#endif  // PROJ_ANDROID_ENEMY_H
