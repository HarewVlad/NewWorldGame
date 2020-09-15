//
// Created by Vlad on 14.09.2020.
//

#include "IngameMenuManager.h"

bool IngameMenuManager::init() {
    auto origin = cocos2d::Director::getInstance()->getVisibleOrigin();
    auto size = cocos2d::Director::getInstance()->getVisibleSize();

    menuButton = cocos2d::ui::Button::create("normal_image.png", "selected_image.png", "disabled_image.png");
    menuButton->setTitleFontName("fonts/Marker Felt.ttf");
    menuButton->setTitleText("Menu");
    menuButton->setPosition(cocos2d::Vec2(origin.x + size.width / 8.0f, origin.y + size.height - size.height / 8.0f));
    menuButton->addTouchEventListener(CC_CALLBACK_1(IngameMenuManager::menuPauseCallback, this));

    this->addChild(menuButton);

    return true;
}

void IngameMenuManager::menuPauseCallback(cocos2d::Ref *sender) {
    currentState = IngameMenuState::PAUSE;
}