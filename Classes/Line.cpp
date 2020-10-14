//
// Created by Vlad on 16.09.2020.
//

#include "Line.h"

bool Line::init(int numObjectsPerLine) {
  delay = static_cast<float>(cocos2d::random()) /
                static_cast<float>(RAND_MAX / MAX_DELAY);
  time -= delay;
  frequency = MIN_SPAWN_FREQUENCY +
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

      runningEnemies.push_back(enemy);

      delay = static_cast<float>(cocos2d::random()) /
                    static_cast<float>(RAND_MAX / MAX_DELAY);
      frequency = MIN_SPAWN_FREQUENCY +
                        static_cast<float>(cocos2d::random()) /
                        static_cast<float>(RAND_MAX / (MAX_SPAWN_FREQUENCY -
                                                       MIN_SPAWN_FREQUENCY));
      time = 0.0f - delay;
    } else {
      time += t;
    }

    if (runningEnemies.size() > maxRunningEnemies) {
      auto visibleSize = cocos2d::Director::getInstance()->getVisibleSize();

      for (auto it = runningEnemies.begin(); it != runningEnemies.end(); ++it) {
        if ((*it)->getPositionY() <= -visibleSize.height * 1.5f) {
          (*it)->removeFromParentAndCleanup(true);

          runningEnemies.erase(it);
        }
      }
    }
  }
}

void Line::setReload() {
  for (auto it = runningEnemies.begin(); it != runningEnemies.end(); ++it) {
    (*it)->removeFromParentAndCleanup(true);
  }
  runningEnemies.clear();

  delay = static_cast<float>(cocos2d::random()) /
    static_cast<float>(RAND_MAX / MAX_DELAY);
  frequency = MIN_SPAWN_FREQUENCY +
    static_cast<float>(cocos2d::random()) /
    static_cast<float>(RAND_MAX / (MAX_SPAWN_FREQUENCY -
      MIN_SPAWN_FREQUENCY));

  time = 0.0f - delay;
  currentState = LineState::NONE;
}

void Line::setStart() {
  currentState = LineState::RUN;
}

void Line::setPause() {
  currentState = LineState::PAUSE;
}