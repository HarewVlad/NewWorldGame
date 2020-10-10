//
// Created by Vlad on 03.09.2020.
//

#include "StartMenu.h"

bool StartMenu::init(const std::function<void(StartMenu *)> &func) {
  if (!Scene::init()) {
    return false;
  }

  this->mainFunc = func;
  this->currentState = StartMenuState::NONE;

  // Orientation
  auto origin = cocos2d::Director::getInstance()->getVisibleOrigin();
  auto size = cocos2d::Director::getInstance()->getVisibleSize();

  // Create background
  background = cocos2d::Sprite::create("StartMenu/sumk1Y.jpg");
  background->setScaleX(size.width / background->getContentSize().width);
  background->setPosition(origin + size * 0.5f);

  // Create menu
  auto start = Button::create("Buttons/Rect.png", cocos2d::Color3B::GRAY, [this](cocos2d::Ref *sender) {
    currentState = StartMenuState::START;

    if (mainFunc != nullptr) {
      mainFunc(this);
    }
  });
  start->setText("Start", cocos2d::Color3B::WHITE);
  start->setPosition({ origin.x + size.width * 0.5f, origin.y + size.height * 0.5f });
  auto exit = Button::create("Buttons/Rect.png", cocos2d::Color3B::GRAY, [this](cocos2d::Ref *sender) {
    currentState = StartMenuState::EXIT;

    if (mainFunc != nullptr) {
      mainFunc(this);
    }
  });
  exit->setText("Exit", cocos2d::Color3B::WHITE);
  exit->setPosition({ origin.x + size.width * 0.5f, origin.y + size.height * 0.4f });

  this->addChild(background, 0);
  this->addChild(start, 1);
  this->addChild(exit, 1);

  return true;
}