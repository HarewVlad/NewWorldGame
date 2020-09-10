//
// Created by Vlad on 05.09.2020.
//

#include "ControllerManager.h"
void ControllerManager::init(const cocos2d::Vec2 &position) {
    this->setPosition(position);

    initJoystick();
    initButton();
}

void ControllerManager::initJoystick() {
    auto screenSize = Director::getInstance()->getVisibleSize();

    Rect joystickBaseDimensions;
    joystickBaseDimensions = Rect(0, 0, 100.f, 100.0f);

    Point joystickBasePosition;
    joystickBasePosition = Vec2(-screenSize.width / 2.5f, -screenSize.height / 4.0f);

    sneakyJoystickBase = new SneakyJoystickSkinnedBase();
    sneakyJoystickBase->init();
    sneakyJoystickBase->setPosition(joystickBasePosition);
    sneakyJoystickBase->setBackgroundSprite(Sprite::create("joystickBackground.png"));
    sneakyJoystickBase->setThumbSprite(Sprite::create("joystickThumb.png"));

    SneakyJoystick *aJoystick = new SneakyJoystick();
    aJoystick->initWithRect(joystickBaseDimensions);
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
    attackButtonPosition = Point(screenSize.width / 2.5f, -screenSize.height / 4.0f);

    sneakyButtonBase = SneakyButtonSkinnedBase::create();
    sneakyButtonBase->setPosition(attackButtonPosition);

    sneakyButtonBase->setDefaultSprite(Sprite::create("joystickThumb.png"));
    sneakyButtonBase->setActivatedSprite(Sprite::create("joystickThumb.png"));
    sneakyButtonBase->setDisabledSprite(Sprite::create("joystickThumb.png"));
    sneakyButtonBase->setPressSprite(Sprite::create("joystickThumb.png"));

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