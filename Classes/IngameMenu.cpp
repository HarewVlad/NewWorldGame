//
// Created by Vlad on 14.09.2020.
//

#include "IngameMenu.h"

bool IngameMenu::init(const std::function<void(IngameMenu *)> &func) {
  this->mainFunc = func;
  this->currentState = IngameMenuState::NONE;

  auto origin = cocos2d::Director::getInstance()->getVisibleOrigin();
  auto size = cocos2d::Director::getInstance()->getVisibleSize();

  // Init background
  background = cocos2d::Sprite::create("IngameMenu/background.png");
  background->setScaleX(size.width / background->getContentSize().width);
  background->setPosition(origin + size * 0.5f);

  // Create menu
  resume = Button::create("Buttons/Rect.png", cocos2d::Color3B::GRAY, [this](cocos2d::Ref *sender) {
    currentState = IngameMenuState::RESUME;

    if (mainFunc != nullptr) {
      mainFunc(this);
    }
  });
  resume->setText("Resume", cocos2d::Color3B::WHITE);
  resume->setPosition(origin + size * 0.5f);

  reset = Button::create("Buttons/Rect.png", cocos2d::Color3B::GRAY, [this](cocos2d::Ref *sender) {
    currentState = IngameMenuState::RESET;

    if (mainFunc != nullptr) {
      mainFunc(this);
    }
  });
  reset->setText("Reset", cocos2d::Color3B::WHITE);
  reset->setPosition({ origin.x + size.width * 0.5f, origin.y + size.height * 0.4f });

  toMainMenu = Button::create("Buttons/Rect.png", cocos2d::Color3B::GRAY, [this](cocos2d::Ref *sender) {
    currentState = IngameMenuState::TO_MAIN_MENU;

    if (mainFunc != nullptr) {
      mainFunc(this);
    }
  });
  toMainMenu->setText("To Main Menu", cocos2d::Color3B::WHITE);
  toMainMenu->setPosition({ origin.x + size.width * 0.5f, origin.y + size.height * 0.3f });

  this->addChild(background, 1);
  this->addChild(resume, 1);
  this->addChild(reset, 1);
  this->addChild(toMainMenu, 1);

  return true;
}