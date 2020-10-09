//
// Created by Vlad on 12.09.2020.
//

#ifndef PROJ_ANDROID_LEVEL_H
#define PROJ_ANDROID_LEVEL_H

#include "cocos2d.h"

#include "ControllerManager.h"
#include "GameOverMenu.h"
#include "IngameMenu.h"
#include "Line.h"
#include "Player.h"
#include "WeatherManager.h"
#include "Heart.h"

enum class LevelState { NONE, RUN, PAUSE, WIN, GAME_OVER };

enum class Components {
  BACKGROUND,
  LINES,
  PLAYER,
  WEATHER,
  CONTROLLERS,
  SCORE,
  COUNTDOWN,
  LIVES,
  INGAME_MENU
};

class Level : public cocos2d::Scene {
 public:
  bool init(Player *player, int numLines,
            int numObjectsPerLine,
            const std::function<void(Level *)> &func);

  void update(float t);
  void setState(LevelState state);

  void setReload();
  void setStart(bool isDelay);
  void setPause();
  void setGameOver();

  inline Line *getLine(int i) const { return lines[i]; }
  inline size_t getLinesCount() const { return lines.size(); }
  inline LevelState getState() const { return currentState; }
 private:
  void startAfterDelay(float t);
  void setInitialPlayerPosition();
  bool onPhysicsContactBegin(cocos2d::PhysicsContact &contact);
  bool onPhysicsContactPreSolve(cocos2d::PhysicsContact &contact, cocos2d::PhysicsContactPreSolve& solve);
 private:
  ControllerManager *controllerManager;
  WeatherManager *weatherManager;
  std::vector<Heart *> hearts;
  Player *player;

  cocos2d::Sprite *background;
  cocos2d::Label *scoreLabel;
  cocos2d::Label *countDownLabel;
  std::vector<Line *> lines;

  std::function<void(Level *)> mainFunc;

  LevelState currentState;

  int score;
  int numLivesLeft;

  // Constants
  const float INPUT_EPSILON = 20.0f;
  const float CONTACT_DISTANCE_EPSILON = 10.0f;
  const int NUM_LIVES = 3;
  const int NUM_POINTS_PER_TICK = 1;
  const float TIME_TO_START = 3;
};

#endif  // PROJ_ANDROID_LEVEL_H
