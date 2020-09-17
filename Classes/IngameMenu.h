//
// Created by Vlad on 14.09.2020.
//

#ifndef PROJ_ANDROID_INGAMEMENU_H
#define PROJ_ANDROID_INGAMEMENU_H

#include "cocos2d.h"
#include "ui/CocosGUI.h"

enum class IngameMenuState {
    IDLE,
    PAUSE
};

enum class IngameMenuElements {
    PAUSE_BUTTON_NORMAL,
    PAUSE_BUTTON_SELECTED,
    PAUSE_BUTTON_DISABLED,
    GAME_OVER_BACKGROUND
};

struct IngameMenu : public cocos2d::Node {
    bool init() override;
    inline IngameMenuState getState() {
        return currentState;
    }
    inline void setState(IngameMenuState state) {
        currentState = state;
    }
private:
    std::string getSource(IngameMenuElements element) const noexcept; // TODO: make for all components ElementSourceManager.cpp / .h
    void menuPauseCallback(cocos2d::Ref *sender);
private:
    cocos2d::ui::Button *menuButton;
    IngameMenuState currentState;
};

#endif //PROJ_ANDROID_INGAMEMENU_H
