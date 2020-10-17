//
// Created by Vlad on 14.09.2020.
//

#include "IngameMenu.h"

IngameMenu *IngameMenu::create(const std::function<void(IngameMenu *)> &func) {
  IngameMenu *node = new (std::nothrow)IngameMenu();
  if(node && node->init(func))
  {
    node->autorelease();
    return node;
  }

  CC_SAFE_DELETE(node);
  return nullptr;
}

bool IngameMenu::init(const std::function<void(IngameMenu *)> &func) {
  this->mainFunc = func;
  this->currentState = IngameMenuState::NONE;

  auto origin = cocos2d::Director::getInstance()->getVisibleOrigin();
  auto visibleSize = cocos2d::Director::getInstance()->getVisibleSize();

  // Init background
  background = cocos2d::Sprite::create("IngameMenu/background.png");
  background->setPosition(origin + visibleSize * 0.5f);
  background->setScale(2.0f);

  // Create menu
  resume = Button::create("Buttons/Rect.png", cocos2d::Color3B::GRAY, [this](cocos2d::Ref *sender) {
    currentState = IngameMenuState::RESUME;

    if (mainFunc != nullptr) {
      mainFunc(this);
    }
  });
  resume->setText("Resume", cocos2d::Color3B::WHITE);
  resume->setPosition({ origin.x + visibleSize.width * 0.5f, origin.y + visibleSize.height * 0.6f });

  reset = Button::create("Buttons/Rect.png", cocos2d::Color3B::GRAY, [this](cocos2d::Ref *sender) {
    currentState = IngameMenuState::RESET;

    if (mainFunc != nullptr) {
      mainFunc(this);
    }
  });
  reset->setText("Reset", cocos2d::Color3B::WHITE);
  reset->setPosition({ origin.x + visibleSize.width * 0.5f, origin.y + visibleSize.height * 0.5f });

  toMainMenu = Button::create("Buttons/Rect.png", cocos2d::Color3B::GRAY, [this](cocos2d::Ref *sender) {
    currentState = IngameMenuState::TO_MAIN_MENU;

    if (mainFunc != nullptr) {
      mainFunc(this);
    }
  });
  toMainMenu->setText("To Main Menu", cocos2d::Color3B::WHITE);
  toMainMenu->setPosition({ origin.x + visibleSize.width * 0.5f, origin.y + visibleSize.height * 0.4f });

  this->addChild(background, 1);
  this->addChild(resume, 1);
  this->addChild(reset, 1);
  this->addChild(toMainMenu, 1);

  return true;
}