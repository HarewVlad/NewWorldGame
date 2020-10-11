//
// Created by Vlad on 03.09.2020.
//

#ifndef PROJ_ANDROID_STARTMENU_H
#define PROJ_ANDROID_STARTMENU_H

#include "cocos2d.h"
#include "Button.h"

enum class StartMenuState { NONE, START, EXIT, ABOUT };

struct StartMenu : public cocos2d::Scene {
  bool init(const std::function<void(StartMenu *)> &func);
  inline StartMenuState getState() { return currentState; }

 private:
  cocos2d::Sprite *background;
  Button *start;
  Button *exit;

  StartMenuState currentState;

  std::function<void(StartMenu *)> mainFunc;
};

#endif  // PROJ_ANDROID_STARTMENU_H
