//
// Created by Vlad on 14.09.2020.
//

#include "IngameMenu.h"

bool IngameMenu::init() {
    auto origin = cocos2d::Director::getInstance()->getVisibleOrigin();
    auto size = cocos2d::Director::getInstance()->getVisibleSize();

    // Init pause button
    menuButton = cocos2d::ui::Button::create(getSource(IngameMenuElements::PAUSE_BUTTON_NORMAL),
            getSource(IngameMenuElements::PAUSE_BUTTON_SELECTED),
            getSource(IngameMenuElements::PAUSE_BUTTON_DISABLED));
    menuButton->setTitleFontName("fonts/Marker Felt.ttf");
    menuButton->setTitleText("Menu");
    menuButton->setPosition(cocos2d::Vec2(origin.x + size.width / 8.0f, origin.y + size.height - size.height / 8.0f));
    menuButton->addTouchEventListener(CC_CALLBACK_1(IngameMenu::menuPauseCallback, this));

    this->addChild(menuButton);

    return true;
}

void IngameMenu::menuPauseCallback(cocos2d::Ref *sender) {
    currentState = IngameMenuState::PAUSE;
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