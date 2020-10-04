//
// Created by Vlad on 05.09.2020.
//

#include "ControllerManager.h"
bool ControllerManager::init(const cocos2d::Vec2 &position) {
  this->setPosition(position);

  initJoystick();
  initButton();

  return true;
}

void ControllerManager::initJoystick() {
  auto screenSize = Director::getInstance()->getVisibleSize();

  Point joystickBasePosition;
  joystickBasePosition =
      Vec2(-screenSize.width / 2.5f, -screenSize.height / 4.0f);

  sneakyJoystickBase = new SneakyJoystickSkinnedBase();
  sneakyJoystickBase->init();
  sneakyJoystickBase->setPosition(joystickBasePosition);
  sneakyJoystickBase->setBackgroundSprite(
      Sprite::create("Controllers\\joystickBackground.png"));
  sneakyJoystickBase->getBackgroundSprite()->setContentSize(
    sneakyJoystickBase->getBackgroundSprite()->getContentSize() * 0.5f);
  sneakyJoystickBase->setThumbSprite(Sprite::create("Controllers\\joystickThumb.png"));
  sneakyJoystickBase->getThumbSprite()->setContentSize(
    sneakyJoystickBase->getThumbSprite()->getContentSize() * 0.5f);

  SneakyJoystick *aJoystick = new SneakyJoystick();
  aJoystick->initWithRect({ 0, 0, sneakyJoystickBase->getBackgroundSprite()->getContentSize().width,
    sneakyJoystickBase->getBackgroundSprite()->getContentSize().height });
  aJoystick->autorelease();
  sneakyJoystickBase->setJoystick(aJoystick);
  sneakyJoystickBase->setPosition(joystickBasePosition);
  sneakyJoystickBase->setOpacity(50);

  sneakyJoystick = sneakyJoystickBase->getJoystick();
  sneakyJoystick->retain();
  this->addChild(sneakyJoystickBase);
}

void ControllerManager::initButton() {
  auto screenSize = Director::getInstance()->getVisibleSize();

  Rect attackButtonDimensions = Rect(0, 0, 64.0f, 64.0f);
  Point attackButtonPosition;
  attackButtonPosition =
      Point(screenSize.width / 2.5f, -screenSize.height / 4.0f);

  sneakyButtonBase = SneakyButtonSkinnedBase::create();
  sneakyButtonBase->setPosition(attackButtonPosition);

  sneakyButtonBase->setDefaultSprite(Sprite::create("Controllers\\joystickThumb.png"));
  sneakyButtonBase->setActivatedSprite(Sprite::create("Controllers\\joystickThumb.png"));
  sneakyButtonBase->setDisabledSprite(Sprite::create("Controllers\\joystickThumb.png"));
  sneakyButtonBase->setPressSprite(Sprite::create("Controllers\\joystickThumb.png"));

  SneakyButton *aAttackButton = SneakyButton::create();
  aAttackButton->initWithRect(attackButtonDimensions);

  //  aAttackButton->setIsHoldable(true);
  //  aAttackButton->setIsToggleable(true);

  sneakyButtonBase->setButton(aAttackButton);
  sneakyButtonBase->setPosition(attackButtonPosition);
  sneakyButtonBase->setOpacity(50);

  sneakyButton = sneakyButtonBase->getButton();
  sneakyButton->retain();
  this->addChild(sneakyButtonBase);
}