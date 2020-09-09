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

static cocos2d::Vector<cocos2d::SpriteFrame *> getSpriteFrames(const char *fmt, int count) {
    auto spriteFrameCache = cocos2d::SpriteFrameCache::getInstance();
    cocos2d::Vector<cocos2d::SpriteFrame *> spriteFrames;
    char str[256];
    for (int i = 0; i < count; ++i) {
        sprintf(str, fmt, i);
        spriteFrames.pushBack(spriteFrameCache->getSpriteFrameByName(str));
    }
    return spriteFrames;
}

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

    // Cache
    {
        // Sprite frame cache
        {
            cocos2d::SpriteFrameCache::getInstance()->addSpriteFramesWithFile("enemy_idle.plist");
            cocos2d::SpriteFrameCache::getInstance()->addSpriteFramesWithFile("enemy_walk.plist");
        }
    }

    // Debug
    {
        this->getPhysicsWorld()->setGravity(cocos2d::Vec2(0, G));
        this->getPhysicsWorld()->setDebugDrawMask(0xffff);
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    // Menu
    {
        menuManager = new MenuManager();
        menuManager->init("menuBackground.jpg");

        this->addChild(menuManager, MENU_Z, static_cast<int>(Components::MENU));
    }

    // World
    {
        worldManager = new WorldManager();
        worldManager->init("worldBackground.png");
        worldManager->initTerrain("grassBlock.png", "dirtBlock.png");

        this->addChild(worldManager, WORLD_Z, static_cast<int>(Components::WORLD));
    }

    // Weather
    {
        weatherManager = new WeatherManager();

        this->addChild(weatherManager, WEATHER_Z, static_cast<int>(Components::WEATHER));
    }

    // Player
    {
        player = new Player();
        player->init("Satyr_03_Idle_000.png", origin + visibleSize * 0.5f);

        // Animations
        {
            // Idle
            {
                auto frames = getSpriteFrames("Satyr_03_Idle_%03d.png", 12);
                auto animation = cocos2d::Animation::createWithSpriteFrames(frames, 1 / 12.0f);
                animation->retain();

                player->addAnimation(PlayerState::IDLE, animation);
            }

            // Run
            {
                auto frames = getSpriteFrames("Satyr_03_Walking_%03d.png", 18);
                auto animation = cocos2d::Animation::createWithSpriteFrames(frames, 1 / 18.0f);
                animation->retain();

                player->addAnimation(PlayerState::RUN, animation);
            }
        }

        // Player Layer
        {
            playerLayer = new PlayerLayer();
            playerLayer->init(player, worldManager);

            this->addChild(playerLayer, PLAYER_Z, static_cast<int>(Components::PLAYER));
        }
    }

    // Controllers
    {
        controllerManager = new ControllerManager();
        controllerManager->init(origin + visibleSize * 0.5f);

        this->addChild(controllerManager, CONTROLLERS_Z, static_cast<int>(Components::CONTROLLERS));
    }

    schedule(CC_SCHEDULE_SELECTOR(GameManager::update), 1 / 144.0f);

    return true;
}

void GameManager::update(float t) {
    if (currentGameState == GameState::MENU)
    {
        MenuState menuState = menuManager->getState();
        switch (menuState) {
            case MenuState::NONE:
                break;
            case MenuState::START:
                // Start the game
                currentGameState = GameState::PLAY;
                break;
            case MenuState::EXIT:
                cocos2d::Director::getInstance()->end();
                break;
        }
    } else if (currentGameState == GameState::PLAY)
    {
        // Movement
        {
            auto joystickPosition = controllerManager->getStickPosition().getNormalized();
            auto isButtonPressed = controllerManager->getValue();

            if (!joystickPosition.isZero()) { // Movement
                player->move(t, joystickPosition);
            } else {
                if (player->getState() != PlayerState::ATTACK) {
                    player->setState(PlayerState::IDLE);
                }
            }
        }

        // Follow
        {
            worldManager->getBackground()->setPosition(player->getPosition());
        }

        // World
        {
            WorldState worldState = worldManager->getState();
            switch (worldState) {
                case WorldState::NONE:
                    break;
            }

            worldManager->update(player->getPosition());
        }

        // Weather
        {
            // TODO: change later
            if (!weatherManager->haveEffect(WeatherType::RAIN)) {
                weatherManager->addWeatherEffect(WeatherType::RAIN);
                weatherManager->runWeatherEffects();
            }
        }
    }
}