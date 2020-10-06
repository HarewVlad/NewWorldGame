//
// Created by Vlad on 05.09.2020.
//

#ifndef PROJ_ANDROID_CONTROLLERMANAGER_H
#define PROJ_ANDROID_CONTROLLERMANAGER_H

#include "Button.h"
#include "cocos2d.h"

enum class ControllerManagerState {
  NONE,
  LEFT_BUTTON_PRESSED,
  RIGHT_BUTTON_PRESSED
};

struct ControllerManager : public cocos2d::Node {
  bool init(const std::function<void(Ref *)> &func);
  inline ControllerManagerState getState() const {
    return currentState;
  }
 private:
  void initRightButton();
  void initLeftButton();

 public:
   Button *rightButton;
   Button *leftButton;

   std::function<void(Ref *)> mainFunc;

   ControllerManagerState currentState;
};

#endif  // PROJ_ANDROID_CONTROLLERMANAGER_H
