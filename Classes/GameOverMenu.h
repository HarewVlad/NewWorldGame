//
// Created by Vlad on 16.09.2020.
//

#ifndef PROJ_ANDROID_GAMEOVERMENU_H
#define PROJ_ANDROID_GAMEOVERMENU_H

#include "cocos2d.h"

enum class GameOverElements {
    BACKGROUND
};

enum class GameOverState {
    IDLE,
    RESTART,
    TO_MAIN_MENU
};

struct GameOverMenu : public cocos2d::Node {
    bool init() override;
    inline GameOverState getState() const {
        return currentState;
    }
    void hide();
    void show();
private:
    std::string getSource(GameOverElements element);
    void gameOverMenuRestart(cocos2d::Ref *sender);
    void gameOverMenuToMainMenu(cocos2d::Ref *sender);
private:
    cocos2d::Menu *menu;
    cocos2d::Sprite *background;
    GameOverState currentState;
};


#endif //PROJ_ANDROID_GAMEOVERMENU_H
