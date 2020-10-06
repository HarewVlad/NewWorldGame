//
// Created by Vlad on 05.09.2020.
//

#include "ControllerManager.h"
bool ControllerManager::init(const std::function<void(Ref *)> &func) {
  initLeftButton();
  initRightButton();

  this->mainFunc = func;
  this->currentState = ControllerManagerState::NONE;

  return true;
}

void ControllerManager::initLeftButton() {
  auto screenOrigin = cocos2d::Director::getInstance()->getVisibleOrigin();
  auto screenSize = cocos2d::Director::getInstance()->getVisibleSize();

  leftButton = Button::create("Controllers/Triangle.png", cocos2d::Color3B::YELLOW, [this](cocos2d::Ref *sender) {
    currentState = ControllerManagerState::LEFT_BUTTON_PRESSED;

    if (mainFunc != nullptr) {
      mainFunc(this);
    }
  });
  leftButton->setPosition(screenOrigin.x + screenSize.width / 8.0f,
    screenOrigin.y + screenSize.height / 8.0f);
  leftButton->runAction(cocos2d::RotateBy::create(0, 90));

  this->addChild(leftButton);
}

void ControllerManager::initRightButton() {
  auto screenOrigin = cocos2d::Director::getInstance()->getVisibleOrigin();
  auto screenSize = cocos2d::Director::getInstance()->getVisibleSize();

  rightButton = Button::create("Controllers/Triangle.png", cocos2d::Color3B::YELLOW, [this](cocos2d::Ref *sender) {
    currentState = ControllerManagerState::RIGHT_BUTTON_PRESSED;

    if (mainFunc != nullptr) {
      mainFunc(this);
    }
  });
  rightButton->setPosition(screenOrigin.x + screenSize.width - screenSize.width / 8.0f,
    screenOrigin.y + screenSize.height / 8.0f);
  rightButton->runAction(cocos2d::RotateBy::create(0, -90));

  this->addChild(rightButton);
}