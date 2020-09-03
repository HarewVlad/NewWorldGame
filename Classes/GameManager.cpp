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

#include "GameManager.h"

USING_NS_CC;

Scene* GameManager::createScene()
{
    return GameManager::create();
}

bool GameManager::init()
{
    if ( !Scene::initWithPhysics() )
    {
        return false;
    }

    // Debug
    {
        this->getPhysicsWorld()->setDebugDrawMask(0xffff);
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    schedule(CC_SCHEDULE_SELECTOR(GameManager::update), 1 / 144.0f);

    // Menu
    {
        menuManager = new MenuManager();
        menuManager->init("menuBackground.jpg");

        this->addChild(menuManager, 1);
    }

    // World
    {
        worldManager = new WorldManager();
        worldManager->init("worldBackground.png");
        worldManager->initTerrain("grassBlock.png", "dirtBlock.png");

        this->addChild(worldManager, 0);
    }

    // Weather
    {
        weatherManager = new WeatherManager();
        weatherManager->addWeatherEffect(WeatherType::RAIN);
        weatherManager->runWeatherEffects();

        this->addChild(weatherManager, 1);
    }

    return true;
}

void GameManager::update(float t) {
    // Menu
    {
        MenuState menuState = menuManager->getState();
        switch (menuState) {
            case MenuState::NONE:
                break;
            case MenuState::START:
                // TODO: make some other preps
                break;
            case MenuState::EXIT:
                cocos2d::Director::getInstance()->end();
                break;
        }
    }

    // World
    {
        WorldState worldState = worldManager->getState();
        switch (worldState) {
            case WorldState::NONE:
                break;
        }
    }

    // Weather
    {

    }
}