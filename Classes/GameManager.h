/****************************************************************************
 Copyright (c) 2017-2018 Xiamen Yaji Software Co., Ltd.
 
 http://www.cocos2d-x.org
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"

#include "MenuManager.h"
#include "WorldManager.h"
#include "WeatherManager.h"
#include "Player.h"
#include "SneakyInput.h"

const int MENU_Z = 5;
const int WEATHER_Z = 10;
const int WORLD_Z = 0;
const int PLAYER_Z = 1;
const int CONTROLLERS_Z = 4;

const float G = -100.0f;

enum class GameState {
    MENU,
    PLAY
};

class GameManager : public cocos2d::Scene
{
public:
    // Default
    static cocos2d::Scene* createScene();

    virtual bool init();
    void initJoystick();
    void initPressButton();
    
    CREATE_FUNC(GameManager);

    // My
    void update(float t);

private:
    // TODO: make predefined positions of z-buffer for all managers
    // Objects
    MenuManager *menuManager;
    WorldManager *worldManager;
    WeatherManager *weatherManager;
    Player *player;

    // Controllers
    SneakyJoystick *leftJoystick;
    SneakyButton *rightButton;

    // State
    GameState currentGameState;
};

#endif // __HELLOWORLD_SCENE_H__
