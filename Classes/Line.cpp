//
// Created by Vlad on 16.09.2020.
//

#include "Line.h"

bool Line::init(int numObjectsPerLine, float speed) {
  this->speed = speed;
  this->delay = static_cast<float>(cocos2d::random()) /
                static_cast<float>(RAND_MAX / MAX_DELAY);
  this->time -= delay;
  this->frequency = MIN_SPAWN_FREQUENCY +
                    static_cast<float>(cocos2d::random()) /
                        static_cast<float>(RAND_MAX / (MAX_SPAWN_FREQUENCY -
                                                       MIN_SPAWN_FREQUENCY));

  int temp = numObjectsPerLine / Enemy::getEnemiesCount();
  int tempReminder = numObjectsPerLine % Enemy::getEnemiesCount();

  for (int i = 0; i < Enemy::getEnemiesCount(); ++i) {
    for (int j = 0; j < temp; ++j) {
      Enemy *enemy = new Enemy();
      enemy->init(static_cast<EnemyType>(i));

      enemies.push_back(enemy);
    }
  }

  for (int i = 0; i < tempReminder; ++i) {
    int reminderDistributionIndex = cocos2d::random() % Enemy::getEnemiesCount();

    Enemy *enemy = new Enemy();
    enemy->init(static_cast<EnemyType>(reminderDistributionIndex));

    enemies.push_back(enemy);
  }

  unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
  std::shuffle(enemies.begin(), enemies.end(),
               std::default_random_engine(seed));

  // Add
  for (auto enemy : enemies) {
    this->addChild(enemy);
  }

  this->scheduleUpdate();

  return true;
}

void Line::update(float t) {
  if (currentState == LineState::RUN) {
    if (lastRenderingEnemy < enemies.size()) {
      if (time >= frequency) {
        renderingEnemies.push_back(enemies[lastRenderingEnemy++]);

        time = 0.0f;
      } else {
        time += t;
      }
    }

    for (auto enemy : renderingEnemies) {
      auto move = cocos2d::MoveBy::create(t, {0, -speed});
      //auto rotate = cocos2d::RotateBy::create(t, speed);
      //auto seq = cocos2d::Sequence::create(move, rotate, nullptr);
      enemy->runAction(move);
    }

    auto visibleSize = cocos2d::Director::getInstance()->getVisibleSize();

    if (lastRenderingEnemy == enemies.size() &&
        renderingEnemies[lastRenderingEnemy - 1]->getPositionY() <= -visibleSize.height * 1.5f) {
      currentState = LineState::FINISHED;
    }
  }
}

void Line::setReload() {
  for (auto enemy : enemies) {
    enemy->setPosition(0, 0);
  }

  renderingEnemies.clear();
  time = 0.0f - delay;
  lastRenderingEnemy = 0;

  // Change characteristics
  this->delay = static_cast<float>(cocos2d::random()) /
    static_cast<float>(RAND_MAX / MAX_DELAY);
  this->time -= delay;
  this->frequency = MIN_SPAWN_FREQUENCY +
    static_cast<float>(cocos2d::random()) /
    static_cast<float>(RAND_MAX / (MAX_SPAWN_FREQUENCY -
      MIN_SPAWN_FREQUENCY));

  // Shuffle objects
  unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
  std::shuffle(enemies.begin(), enemies.end(),
    std::default_random_engine(seed));

  currentState = LineState::NONE;
}

void Line::setStart() {
  currentState = LineState::RUN;

  for (auto enemy : enemies) {
    enemy->setStart();
  }
}

void Line::setPause() {
  currentState = LineState::PAUSE;

  for (auto enemy : enemies) {
    enemy->setStop();
  }
}