//
// Created by Vlad on 14.09.2020.
//

#ifndef PROJ_ANDROID_INGAMEMENUMANAGER_H
#define PROJ_ANDROID_INGAMEMENUMANAGER_H

#include "cocos2d.h"
#include "ui/CocosGUI.h"

enum class IngameMenuState {
    IDLE,
    PAUSE
};

struct IngameMenuManager : public cocos2d::Node {
    bool init() override;
    inline IngameMenuState getState() {
        return currentState;
    }
    inline void setState(IngameMenuState state) {
        currentState = state;
    }
private:
    void menuPauseCallback(cocos2d::Ref *sender);
private:
    cocos2d::ui::Button *menuButton;
    IngameMenuState currentState;
};

#endif //PROJ_ANDROID_INGAMEMENUMANAGER_H
