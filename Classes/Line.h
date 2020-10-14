//
// Created by Vlad on 16.09.2020.
//

#ifndef PROJ_ANDROID_LINE_H
#define PROJ_ANDROID_LINE_H

#include "cocos2d.h"
#include "Enemy.h"

enum class LineState { NONE, PAUSE, RUN, FINISHED };

struct Line : public cocos2d::Node {
  bool init(int numObjectsPerLine);
  void update(float t) override;
  void setReload();
  void setStart();
  void setPause();
 private:
    std::vector<Enemy *> runningEnemies;
    int maxRunningEnemies = 100;

  float time;
  float frequency;
  float delay;
  LineState currentState;

  // Constants
  const float MAX_DELAY = 2.5f;
  const float MIN_SPAWN_FREQUENCY = 1.0f;
  const float MAX_SPAWN_FREQUENCY = 3.0f;
  const float SCALE = 1.0f;
};

#endif  // PROJ_ANDROID_LINE_H
