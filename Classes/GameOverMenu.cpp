//
// Created by Vlad on 16.09.2020.
//

#include "GameOverMenu.h"

bool GameOverMenu::init(const std::function<void(GameOverMenu *)> &func) {
  if (!Scene::init()) {
    return false;
  }

  this->mainFunc = func;
  this->currentState = GameOverMenuState::NONE;

  auto origin = cocos2d::Director::getInstance()->getVisibleOrigin();
  auto size = cocos2d::Director::getInstance()->getVisibleSize();

  // Init background
  background = cocos2d::Sprite::create(getSource(GameOverElements::BACKGROUND));
  background->setScaleX(size.width / background->getContentSize().width);
  background->setScaleY(size.height / background->getContentSize().height);
  background->setPosition(origin + size * 0.5f);

  // Init menu
  menu = cocos2d::Menu::create();

  // Restart
  {
    auto label =
        cocos2d::Label::createWithTTF("Restart", "fonts/Marker Felt.ttf", 24);
    auto menuItem = cocos2d::MenuItemLabel::create(label);
    menuItem->setPositionY(24);
    menuItem->setCallback(
        CC_CALLBACK_1(GameOverMenu::gameOverMenuRestart, this));
    menu->addChild(menuItem);
  }

  // To main menu
  {
    auto label = cocos2d::Label::createWithTTF("To Main Menu",
                                               "fonts/Marker Felt.ttf", 24);
    auto menuItem = cocos2d::MenuItemLabel::create(label);
    menuItem->setPositionY(0);
    menuItem->setCallback(
        CC_CALLBACK_1(GameOverMenu::gameOverMenuToMainMenu, this));
    menu->addChild(menuItem);
  }

  // Add
  this->addChild(background);
  this->addChild(menu);

  return true;
}

std::string GameOverMenu::getSource(GameOverElements element) {
  switch (element) {
    case GameOverElements::BACKGROUND:
      return "gameOverMenuBackground.png";
  }
}

void GameOverMenu::gameOverMenuRestart(cocos2d::Ref *sender) {
  currentState = GameOverMenuState::RESTART;

  mainFunc(this);
}
void GameOverMenu::gameOverMenuToMainMenu(cocos2d::Ref *sender) {
  currentState = GameOverMenuState::TO_MAIN_MENU;

  mainFunc(this);
}