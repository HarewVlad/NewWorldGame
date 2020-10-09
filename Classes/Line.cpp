//
// Created by Vlad on 16.09.2020.
//

#include "Line.h"

bool Line::init(int numObjectsPerLine) {
  this->delay = static_cast<float>(cocos2d::random()) /
                static_cast<float>(RAND_MAX / MAX_DELAY);
  this->time -= delay;
  this->frequency = MIN_SPAWN_FREQUENCY +
                    static_cast<float>(cocos2d::random()) /
                        static_cast<float>(RAND_MAX / (MAX_SPAWN_FREQUENCY -
                                                       MIN_SPAWN_FREQUENCY));
  this->scheduleUpdate();

  return true;
}

void Line::update(float t) {
  if (currentState == LineState::RUN) {
    if (time >= frequency) {
      int randomType = cocos2d::random() % Enemy::getEnemiesCount();

      Enemy *enemy = Enemy::create(static_cast<EnemyType>(randomType));
      enemy->setStart();
      this->addChild(enemy);

      time = 0.0f;
    } else {
      time += t;
    }
  }
}

void Line::setReload() {
  time = 0.0f - delay;

  // Change characteristics
  this->delay = static_cast<float>(cocos2d::random()) /
    static_cast<float>(RAND_MAX / MAX_DELAY);
  this->time -= delay;
  this->frequency = MIN_SPAWN_FREQUENCY +
    static_cast<float>(cocos2d::random()) /
    static_cast<float>(RAND_MAX / (MAX_SPAWN_FREQUENCY -
      MIN_SPAWN_FREQUENCY));

  currentState = LineState::NONE;
}

void Line::setStart() {
  currentState = LineState::RUN;
}

void Line::setPause() {
  currentState = LineState::PAUSE;
}