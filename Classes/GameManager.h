#ifndef __GAMEMANAGER_H__
#define __GAMEMANAGER_H__

#include "LevelManager.h"
#include "StartMenu.h"
#include "cocos2d.h"

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
  void onGameOverMenu(GameOverMenu *gameOverMenu);
  void onLevel(Level *level);

 private:
  StartMenu *startMenu;
  GameOverMenu *gameOverMenu;
  LevelManager *levelManager;

  GameState currentState;
};

#endif  // __GAMEMANAGER_H__
