//
// Created by Vlad on 03.09.2020.
//

#ifndef PROJ_ANDROID_STARTMENU_H
#define PROJ_ANDROID_STARTMENU_H

#include "cocos2d.h"

enum class StartMenuState {
    IDLE,
    START,
    EXIT
};

struct StartMenu : public cocos2d::Node {
    void init(const std::string &filename);
    inline StartMenuState getState() {
        return currentState;
    }
    inline void setState(StartMenuState state) {
        currentState = state;
    }
    void show();
    void hide();
private:
    void menuStartCallback(cocos2d::Ref *sender);
    void menuExitCallback(cocos2d::Ref *sender);
private:
    cocos2d::Sprite *background;
    cocos2d::Menu *menu;
    StartMenuState currentState;
};

#endif //PROJ_ANDROID_STARTMENU_H
