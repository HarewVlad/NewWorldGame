//
// Created by Vlad on 03.09.2020.
//

#ifndef PROJ_ANDROID_MENUMANAGER_H
#define PROJ_ANDROID_MENUMANAGER_H

#include "cocos2d.h"

enum class MenuState {
    NONE,
    START,
    EXIT
};

struct MenuManager : public cocos2d::Node {
    void init(const std::string &filename);
    inline MenuState getState() {
        return currentMenuState;
    }
private:
    void menuStartCallback(cocos2d::Ref *sender);
    void menuExitCallback(cocos2d::Ref *sender);
private:
    cocos2d::Sprite *background;
    cocos2d::Menu *menu;
    MenuState currentMenuState;
};

#endif //PROJ_ANDROID_MENUMANAGER_H
