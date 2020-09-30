//
// Created by Vlad on 14.09.2020.
//

#include "IngameMenu.h"

bool IngameMenu::init(const std::string &backgroundFileName,
                      const std::function<void(IngameMenu *)> &func) {
  this->mainFunc = func;
  this->currentState = IngameMenuState::NONE;

  auto origin = cocos2d::Director::getInstance()->getVisibleOrigin();
  auto size = cocos2d::Director::getInstance()->getVisibleSize();

  // Init background
  background = cocos2d::Sprite::create(backgroundFileName);
  background->setScaleX(size.width / background->getContentSize().width);
  background->setPosition(origin + size * 0.5f);

  // Create menu labels
  auto resumeLabel =
      cocos2d::Label::createWithTTF("Resume", "fonts/Marker Felt.ttf", 24);
  resumeLabel->setColor({255, 0, 0});
  auto resetLabel =
      cocos2d::Label::createWithTTF("Reset", "fonts/Marker Felt.ttf", 24);
  resetLabel->setColor({255, 0, 0});
  auto toMainMenuLabel = cocos2d::Label::createWithTTF(
      "To Main Menu", "fonts/Marker Felt.ttf", 24);
  toMainMenuLabel->setColor({255, 0, 0});

  auto resume = cocos2d::MenuItemLabel::create(resumeLabel);
  resume->setPositionY(48);
  resume->setCallback(CC_CALLBACK_1(IngameMenu::resumeCallback, this));
  auto reset = cocos2d::MenuItemLabel::create(resetLabel);
  reset->setPositionY(24);
  reset->setCallback(CC_CALLBACK_1(IngameMenu::resetCallback, this));
  auto toMainMenu = cocos2d::MenuItemLabel::create(toMainMenuLabel);
  toMainMenu->setPositionY(0);
  toMainMenu->setCallback(CC_CALLBACK_1(IngameMenu::toMainMenuCallback, this));

  menu = cocos2d::Menu::create();
  menu->addChild(resume);
  menu->addChild(reset);
  menu->addChild(toMainMenu);

  // Init pause button
  menuButton = cocos2d::ui::Button::create(
      getSource(IngameMenuElements::PAUSE_BUTTON_NORMAL),
      getSource(IngameMenuElements::PAUSE_BUTTON_SELECTED),
      getSource(IngameMenuElements::PAUSE_BUTTON_DISABLED));
  menuButton->setTitleFontName("fonts/Marker Felt.ttf");
  menuButton->setTitleText("Menu");
  menuButton->setPosition(
      cocos2d::Vec2(origin.x + size.width / 8.0f,
                    origin.y + size.height - size.height / 8.0f));
  menuButton->addTouchEventListener(
      CC_CALLBACK_1(IngameMenu::onMenuCallback, this));

  this->addChild(background, 1);
  this->addChild(menu, 1);
  this->addChild(menuButton, 0);

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

std::string IngameMenu::getSource(IngameMenuElements element) const noexcept {
  switch (element) {
    case IngameMenuElements::PAUSE_BUTTON_NORMAL:
      return "normal_image.png";
    case IngameMenuElements::PAUSE_BUTTON_DISABLED:
      return "disabled_image.png";
    case IngameMenuElements::PAUSE_BUTTON_SELECTED:
      return "selected_image.png";
  }
}

void IngameMenu::show() {
  menuButton->setVisible(false);

  menu->setEnabled(true);
  menu->setVisible(true);
  background->setVisible(true);

  currentState = IngameMenuState::NONE;
}

void IngameMenu::hide() {
  menuButton->setVisible(true);

  menu->setEnabled(false);
  menu->setVisible(false);
  background->setVisible(false);
}