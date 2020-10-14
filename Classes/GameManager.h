#ifndef __GAMEMANAGER_H__
#define __GAMEMANAGER_H__

#include "cocos2d.h"

#include "Utils.h"
#include "StartMenu.h"
#include "IngameMenu.h"
#include "Level.h"
#include "About.h"

#include "../firebase_cpp_sdk/include/firebase/admob.h"

enum class GameState { NONE, MENU, PLAY};

class GameManager : public cocos2d::Scene {
 public:
  // Default
  static cocos2d::Scene *createScene();

  virtual bool init();

  CREATE_FUNC(GameManager);

  // My
  void update(float t);
  void setMenu();
  void setPlay();
 private:
  void onStartMenu(StartMenu *startMenu);
  void onIngameMenu(IngameMenu *ingameMenu);
  void onLevel(Level *level);
 private:
  StartMenu *startMenu;
  IngameMenu *ingameMenu;
  About *aboutWindow;
  Level *level;
  Player *player;

  GameState currentState;

  const int adShowRate = 3; // 1 ad every 3 game overs
};

#endif  // __GAMEMANAGER_H__
