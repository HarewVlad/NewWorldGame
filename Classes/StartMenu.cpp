//
// Created by Vlad on 03.09.2020.
//

#include "StartMenu.h"

void StartMenu::init(const std::string &filename) {
    // Orientation
    auto origin = cocos2d::Director::getInstance()->getVisibleOrigin();
    auto size = cocos2d::Director::getInstance()->getVisibleSize();

    // Create background
    background = cocos2d::Sprite::create(filename);
    background->setScaleX(size.width / background->getContentSize().width);
    background->setPosition(origin + size * 0.5f);

    // Create menu labels
    auto startLabel = cocos2d::Label::createWithTTF("Start", "fonts/Marker Felt.ttf", 24);
    auto exitLabel = cocos2d::Label::createWithTTF("Exit", "fonts/Marker Felt.ttf", 24);

    auto start = cocos2d::MenuItemLabel::create(startLabel);
    start->setPositionY(24);
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
}

void StartMenu::menuStartCallback(cocos2d::Ref *sender) {
    currentState = StartMenuState::START;
}
void StartMenu::menuExitCallback(cocos2d::Ref *sender) {
    currentState = StartMenuState::EXIT;
}

void StartMenu::show() {
    background->runAction(cocos2d::FadeIn::create(1.0f));
    menu->runAction(cocos2d::FadeIn::create(1.0f));

    menu->setEnabled(true);

    // Set state
    currentState = StartMenuState::IDLE;
}

void StartMenu::hide() {
    background->runAction(cocos2d::FadeOut::create(1.0f));
    menu->runAction(cocos2d::FadeOut::create(1.0f));

    menu->setEnabled(false);

    // Set state
    currentState = StartMenuState::IDLE;
}