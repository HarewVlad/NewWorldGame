#ifndef __GAMEMANAGER_H__
#define __GAMEMANAGER_H__

#include "cocos2d.h"

#include "StartMenuManager.h"
#include "IngameMenuManager.h"
#include "LevelManager.h"
#include "WeatherManager.h"
#include "Player.h"
#include "SneakyInput.h"
#include "ControllerManager.h"
#include "LevelManager.h"

const float G = -300.0f;

enum class GameState {
    MENU,
    PLAY
};

enum class Components {
    LEVELS,
    PLAYER,
    WEATHER,
    CONTROLLERS,
    INGAME_MENU,
    MENU
};

class GameManager : public cocos2d::Scene
{
public:
    // Default
    static cocos2d::Scene* createScene();

    virtual bool init();

    CREATE_FUNC(GameManager);

    // My
    void update(float t);

private:
    // Objects
    StartMenuManager *startMenuManager;
    IngameMenuManager *ingameMenuManager;
    LevelManager *levelManager;
    WeatherManager *weatherManager;
    Player *player;

    // Controllers
    ControllerManager *controllerManager;

    // State
    GameState currentGameState;
};

#endif // __GAMEMANAGER_H__
