//
// Created by Vlad on 16.09.2020.
//

#ifndef PROJ_ANDROID_LINE_H
#define PROJ_ANDROID_LINE_H

#include "cocos2d.h"
#include "Enemy.h"

enum class LineState { NONE, PAUSE, RUN, FINISHED };

struct Line : public cocos2d::Node {
  bool init(int numObjectsPerLine, float speed);
  void update(float t) override;
  void setReload();
  void setStart();
  void setPause();
  inline void setSpeed(float speed) {
    this->speed = speed;
  }
 private:
  std::vector<Enemy *> enemies;  // All objects of a monster spawn line
  std::vector<Enemy *>
      renderingEnemies;  // Objects that are actually rendering on the screen
  float time;
  float frequency;
  float delay;
  float speed;
  int lastRenderingEnemy;
  LineState currentState;

  // Constants
  const float MAX_DELAY = 2.5f;
  const float MIN_SPAWN_FREQUENCY = 0.5f;
  const float MAX_SPAWN_FREQUENCY = 4.0f;
  const float SCALE = 1.0f;
};

#endif  // PROJ_ANDROID_LINE_H
