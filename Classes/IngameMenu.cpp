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

  // Create menu labels
  auto resumeLabel =
      cocos2d::Label::createWithTTF("Resume", "fonts/ThaleahFat.ttf", 28);
  resumeLabel->setColor(cocos2d::Color3B::YELLOW);
  auto resetLabel =
      cocos2d::Label::createWithTTF("Reset", "fonts/ThaleahFat.ttf", 28);
  resetLabel->setColor(cocos2d::Color3B::YELLOW);
  auto toMainMenuLabel = cocos2d::Label::createWithTTF(
      "To Main Menu", "fonts/ThaleahFat.ttf", 28);
  toMainMenuLabel->setColor(cocos2d::Color3B::YELLOW);

  auto resume = cocos2d::MenuItemLabel::create(resumeLabel);
  resume->setPositionY(56);
  resume->setCallback(CC_CALLBACK_1(IngameMenu::resumeCallback, this));
  auto reset = cocos2d::MenuItemLabel::create(resetLabel);
  reset->setPositionY(28);
  reset->setCallback(CC_CALLBACK_1(IngameMenu::resetCallback, this));
  auto toMainMenu = cocos2d::MenuItemLabel::create(toMainMenuLabel);
  toMainMenu->setPositionY(0);
  toMainMenu->setCallback(CC_CALLBACK_1(IngameMenu::toMainMenuCallback, this));

  menu = cocos2d::Menu::create();
  menu->addChild(resume);
  menu->addChild(reset);
  menu->addChild(toMainMenu);

  // Init pause button
  menuEnterButton = Button::create("IngameMenu/Button/Square.png", 
    cocos2d::Color3B::BLUE, 
    CC_CALLBACK_1(IngameMenu::onMenuCallback, this));
  menuEnterButton->setPosition(
      cocos2d::Vec2(origin.x + size.width / 8.0f,
                    origin.y + size.height - size.height / 8.0f));
  menuEnterButton->setText("Menu", cocos2d::Color3B::YELLOW);

  this->addChild(background, 1);
  this->addChild(menu, 1);
  this->addChild(menuEnterButton, 0);

  hide();

  return true;
}

void IngameMenu::onMenuCallback(cocos2d::Ref *sender) {
  show();

  if (mainFunc != nullptr) {
    mainFunc(this);
  }
}

void IngameMenu::resetCallback(cocos2d::Ref *sender) {
  currentState = IngameMenuState::RESET;

  if (mainFunc != nullptr) {
    mainFunc(this);
  }

  hide();
}

void IngameMenu::toMainMenuCallback(cocos2d::Ref *sender) {
  currentState = IngameMenuState::TO_MAIN_MENU;

  if (mainFunc != nullptr) {
    mainFunc(this);
  }

  hide();
}

void IngameMenu::resumeCallback(cocos2d::Ref *sender) {
  currentState = IngameMenuState::RESUME;

  if (mainFunc != nullptr) {
    mainFunc(this);
  }

  hide();
}

void IngameMenu::show() {
  menuEnterButton->setVisible(false);

  menu->setEnabled(true);
  menu->setVisible(true);
  background->setVisible(true);

  currentState = IngameMenuState::NONE;
}

void IngameMenu::hide() {
  menuEnterButton->setVisible(true);

  menu->setEnabled(false);
  menu->setVisible(false);
  background->setVisible(false);
}