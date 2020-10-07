//
// Created by Vlad on 14.09.2020.
//

#ifndef PROJ_ANDROID_INGAMEMENU_H
#define PROJ_ANDROID_INGAMEMENU_H

#include "cocos2d.h"
#include "Button.h"

enum class IngameMenuState { NONE, RESUME, RESET, TO_MAIN_MENU };

struct IngameMenu : public cocos2d::Scene {
  bool init(const std::function<void(IngameMenu *)> &func);
  inline IngameMenuState getState() { return currentState; }
  inline void setState(IngameMenuState state) { currentState = state; }
 private:
   // TODO: redo hardcoded buttons
  cocos2d::Sprite *background;
  Button *resume;
  Button *reset;
  Button *toMainMenu;

  IngameMenuState currentState;

  std::function<void(IngameMenu *)> mainFunc;
};

#endif  // PROJ_ANDROID_INGAMEMENU_H
