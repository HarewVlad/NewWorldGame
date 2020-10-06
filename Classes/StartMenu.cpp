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
  background = cocos2d::Sprite::create("StartMenu/background.jpg");
  background->setScaleX(size.width / background->getContentSize().width);
  background->setPosition(origin + size * 0.5f);

  // Create menu labels
  auto startLabel =
      cocos2d::Label::createWithTTF("Start", "fonts/ThaleahFat.ttf", 28);
  startLabel->setColor(cocos2d::Color3B::YELLOW);
  auto exitLabel =
      cocos2d::Label::createWithTTF("Exit", "fonts/ThaleahFat.ttf", 28);
  exitLabel->setColor(cocos2d::Color3B::YELLOW);

  auto start = cocos2d::MenuItemLabel::create(startLabel);
  start->setPositionY(28);
  start->setCallback(CC_CALLBACK_1(StartMenu::menuStartCallback, this));
  auto exit = cocos2d::MenuItemLabel::create(exitLabel);
  exit->setPositionY(0);
  exit->setCallback(CC_CALLBACK_1(StartMenu::menuExitCallback, this));

  menu = cocos2d::Menu::create();
  menu->addChild(start);
  menu->addChild(exit);

  // Add components to the node
  this->addChild(background);
  this->addChild(menu);

  this->scheduleUpdate();

  return true;
}

void StartMenu::menuStartCallback(cocos2d::Ref *sender) {
  currentState = StartMenuState::START;

  mainFunc(this);
}
void StartMenu::menuExitCallback(cocos2d::Ref *sender) {
  currentState = StartMenuState::EXIT;

  mainFunc(this);
}

void StartMenu::update(float t) {}