//
// Created by Vlad on 16.09.2020.
//

#ifndef PROJ_ANDROID_GAMEOVERMENU_H
#define PROJ_ANDROID_GAMEOVERMENU_H

#include "cocos2d.h"

enum class GameOverElements { BACKGROUND };

enum class GameOverMenuState { NONE, RESTART, TO_MAIN_MENU };

struct GameOverMenu : public cocos2d::Scene {
  bool init(const std::function<void(GameOverMenu *)> &func);
  inline GameOverMenuState getState() const { return currentState; }

 private:
  std::string getSource(GameOverElements element);
  void gameOverMenuRestart(cocos2d::Ref *sender);
  void gameOverMenuToMainMenu(cocos2d::Ref *sender);

 private:
  cocos2d::Menu *menu;
  cocos2d::Sprite *background;
  GameOverMenuState currentState;

  std::function<void(GameOverMenu *)> mainFunc;
};

#endif  // PROJ_ANDROID_GAMEOVERMENU_H
