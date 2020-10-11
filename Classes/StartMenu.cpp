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
  background->setScaleY(size.height / background->getContentSize().height);
  background->setPosition(origin + size * 0.5f);

  // Create menu
  auto startButton = Button::create("Buttons/Rect.png", cocos2d::Color3B::GRAY, [this](cocos2d::Ref *sender) {
    currentState = StartMenuState::START;

    if (mainFunc != nullptr) {
      mainFunc(this);
    }
  });
  startButton->setText("Start", cocos2d::Color3B::WHITE);
  startButton->setPosition({ origin.x + size.width * 0.5f, origin.y + size.height * 0.6f });
  auto exitButton = Button::create("Buttons/Rect.png", cocos2d::Color3B::GRAY, [this](cocos2d::Ref *sender) {
    currentState = StartMenuState::EXIT;

    if (mainFunc != nullptr) {
      mainFunc(this);
    }
  });
  exitButton->setText("Exit", cocos2d::Color3B::WHITE);
  exitButton->setPosition({ origin.x + size.width * 0.5f, origin.y + size.height * 0.5f });
  auto aboutButton = Button::create("Buttons/Rect.png", cocos2d::Color3B::GRAY, [this](cocos2d::Ref *sender) {
    currentState = StartMenuState::ABOUT;

    if (mainFunc != nullptr) {
      mainFunc(this);
    }
  });
  aboutButton->setText("About", cocos2d::Color3B::WHITE);
  aboutButton->setPosition({ origin.x + size.width * 0.5f, origin.y + size.height * 0.4f });

  this->addChild(background, 0);
  this->addChild(startButton, 1);
  this->addChild(exitButton, 1);
  this->addChild(aboutButton, 1);

  return true;
}