#ifndef __GAMEMANAGER_H__
#define __GAMEMANAGER_H__

#include "cocos2d.h"

#include "StartMenu.h"
#include "IngameMenu.h"
#include "LevelManager.h"
#include "WeatherManager.h"
#include "Player.h"
#include "SneakyInput.h"
#include "ControllerManager.h"
#include "LevelManager.h"
#include "GameOverMenu.h"

const float G = -300.0f;

enum class GameState {
    MENU,
    PLAY,
    GAME_OVER
};

enum class Components {
    LEVELS,
    PLAYER,
    WEATHER,
    CONTROLLERS,
    INGAME_MENU,
    GAME_OVER_MENU,
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
    bool onPhysicsContactBegin(cocos2d::PhysicsContact &contact); // TODO: mb create different class for collision managment
private:
    // Objects
    StartMenu *startMenu;
    IngameMenu *ingameMenu;
    GameOverMenu *gameOverMenu;
    LevelManager *levelManager;
    WeatherManager *weatherManager;
    Player *player;

    // Controllers
    ControllerManager *controllerManager;

    // State
    GameState currentState;
};

#endif // __GAMEMANAGER_H__
