//
// Created by Vlad on 05.09.2020.
//

#ifndef PROJ_ANDROID_CONTROLLERMANAGER_H
#define PROJ_ANDROID_CONTROLLERMANAGER_H

#include "SneakyInput.h"
#include "cocos2d.h"

struct ControllerManager : public cocos2d::Node {
  bool init(const cocos2d::Vec2 &position);
  inline cocos2d::Vec2 getStickPosition() const {
    return sneakyJoystick->getStickPosition();
  }
  inline bool getValue() const { return sneakyButton->getValue(); }
  inline void setValue(bool value) { sneakyButton->setValue(value); }

 private:
  void initJoystick();
  void initButton();

 public:
  SneakyJoystick *sneakyJoystick;
  SneakyJoystickSkinnedBase *sneakyJoystickBase;
  SneakyButton *sneakyButton;
  SneakyButtonSkinnedBase *sneakyButtonBase;
};

#endif  // PROJ_ANDROID_CONTROLLERMANAGER_H
