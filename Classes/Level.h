//
// Created by Vlad on 12.09.2020.
//

#ifndef PROJ_ANDROID_LEVEL_H
#define PROJ_ANDROID_LEVEL_H

#include "ControllerManager.h"
#include "GameOverMenu.h"
#include "IngameMenu.h"
#include "Line.h"
#include "Player.h"
#include "WeatherManager.h"
#include "cocos2d.h"

enum class LevelState { NONE, RUN, PAUSE, WIN, GAME_OVER };

enum class Components {
  BACKGROUND,
  LINES,
  PLAYER,
  WEATHER,
  CONTROLLERS,
  INGAME_MENU
};

class Level : public cocos2d::Scene {
 public:
  bool init(const std::string &backgroundName,
            const std::string &ingameMenuBackgroundFileName,
            const std::vector<ObjectType> &objectsVariation, int numLines,
            int numObjectsPerLine, float speed,
            const std::function<void(Level *)> &func);

  void update(float t);
  void setState(LevelState state);

  void setReload();
  void setStart();
  void setPause();
  void setResume();
  void setGameOver();

  inline Line *getLine(int i) const { return lines[i]; }
  inline size_t getLinesCount() const { return lines.size(); }
  inline LevelState getState() const { return currentState; }

 private:
  void setInitialPlayerPosition();
  bool onPhysicsContactBegin(cocos2d::PhysicsContact &contact);
  void onIngameMenu(IngameMenu *ingameMenu);

 private:
  ControllerManager *controllerManager;
  IngameMenu *ingameMenu;
  GameOverMenu *gameOverMenu;
  WeatherManager *weatherManager;
  Player *player;

  cocos2d::Sprite *background;
  std::vector<Line *> lines;

  std::function<void(Level *)> mainFunc;

  LevelState currentState;
};

#endif  // PROJ_ANDROID_LEVEL_H
