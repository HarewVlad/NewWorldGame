//
// Created by Vlad on 14.09.2020.
//

#ifndef PROJ_ANDROID_INGAMEMENU_H
#define PROJ_ANDROID_INGAMEMENU_H

#include "cocos2d.h"
#include "ui/CocosGUI.h"

enum class IngameMenuState { NONE, RESUME, RESET, TO_MAIN_MENU };

struct IngameMenu : public cocos2d::Node {
  bool init(const std::function<void(IngameMenu *)> &func);
  inline IngameMenuState getState() { return currentState; }
  inline void setState(IngameMenuState state) { currentState = state; }
 private:
  void onMenuCallback(cocos2d::Ref *sender);
  void resetCallback(cocos2d::Ref *sender);
  void toMainMenuCallback(cocos2d::Ref *sender);
  void resumeCallback(cocos2d::Ref *sender);
  void show();
  void hide();
 private:
  cocos2d::Sprite *background;
  cocos2d::ui::Button *menuButton;
  cocos2d::Menu *menu;
  IngameMenuState currentState;

  std::function<void(IngameMenu *)> mainFunc;
};

#endif  // PROJ_ANDROID_INGAMEMENU_H
