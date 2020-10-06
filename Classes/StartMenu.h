//
// Created by Vlad on 03.09.2020.
//

#ifndef PROJ_ANDROID_STARTMENU_H
#define PROJ_ANDROID_STARTMENU_H

#include "cocos2d.h"

enum class StartMenuState { NONE, START, EXIT };

struct StartMenu : public cocos2d::Scene {
  bool init(const std::function<void(StartMenu *)> &func);
  inline StartMenuState getState() { return currentState; }
  inline void setState(StartMenuState state) { currentState = state; }
  void update(float t);

 private:
  void menuStartCallback(cocos2d::Ref *sender);
  void menuExitCallback(cocos2d::Ref *sender);
 private:
  cocos2d::Sprite *background;
  cocos2d::Menu *menu;
  StartMenuState currentState;

  std::function<void(StartMenu *)> mainFunc;
};

#endif  // PROJ_ANDROID_STARTMENU_H
