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
            cocos2d::SpriteFrameCache::getInstance()->addSpriteFramesWithFile("woodcutter_idle.plist");
            cocos2d::SpriteFrameCache::getInstance()->addSpriteFramesWithFile("woodcutter_run.plist");
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

        this->addChild(menuManager, MENU_Z);
    }

    // World
    {
        worldManager = new WorldManager();
        worldManager->init("worldBackground.png");
        worldManager->initTerrain("grassBlock.png", "dirtBlock.png");

        this->addChild(worldManager, WORLD_Z);
    }

    // Weather
    {
        weatherManager = new WeatherManager();

        this->addChild(weatherManager, WEATHER_Z);
    }

    // Player
    {
        player = new Player();
        cocos2d::Vec2 position = origin + visibleSize * 0.8f;
        player->init("woodcutter.png", position);

        // Animations
        {
            // Idle
            {
                auto frames = getSpriteFrames("woodcutter_idle_%03d.png", 4);
                auto animation = cocos2d::Animation::createWithSpriteFrames(frames, 1 / 4.0f);
                animation->retain();

                player->addAnimation(PlayerState::IDLE, animation);
            }

            // Run
            {
                auto frames = getSpriteFrames("woodcutter_run_%03d.png", 6);
                auto animation = cocos2d::Animation::createWithSpriteFrames(frames, 1 / 6.0f);
                animation->retain();

                player->addAnimation(PlayerState::RUN, animation);
            }
        }

        this->addChild(player, PLAYER_Z);
    }

    // Controllers
    {
        initJoystick();
        initPressButton();
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
            auto joystickPosition = leftJoystick->getStickPosition();
            auto isButtonPressed = rightButton->getValue();

            if (!joystickPosition.isZero()) { // Movement
                player->move(t, joystickPosition);
            } else {
                if (player->getState() != PlayerState::ATTACK) {
                    player->setState(PlayerState::IDLE);
                }
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
            // TODO: change later
            if (!weatherManager->haveEffect(WeatherType::RAIN)) {
                weatherManager->addWeatherEffect(WeatherType::RAIN);
                weatherManager->runWeatherEffects();
            }
        }
    }
}

void GameManager::initJoystick() {
    auto screenSize = Director::getInstance()->getVisibleSize();
    auto screenOrigin = Director::getInstance()->getVisibleOrigin();

    Rect joystickBaseDimensions;
    joystickBaseDimensions = Rect(0, 0, 100.f, 100.0f);

    Point joystickBasePosition;
    joystickBasePosition = Vec2(screenOrigin.x + screenSize.width / 8.0f, screenOrigin.y + screenSize.height * 0.2f);

    SneakyJoystickSkinnedBase *joystickBase = new SneakyJoystickSkinnedBase();
    joystickBase->init();
    joystickBase->setPosition(joystickBasePosition);
    joystickBase->setBackgroundSprite(Sprite::create("joystickBackground.png"));
    joystickBase->setThumbSprite(Sprite::create("joystickThumb.png"));

    SneakyJoystick *aJoystick = new SneakyJoystick();
    aJoystick->initWithRect(joystickBaseDimensions);
    aJoystick->autorelease();
    joystickBase->setJoystick(aJoystick);
    joystickBase->setPosition(joystickBasePosition);
    joystickBase->setOpacity(50);

    leftJoystick = joystickBase->getJoystick();
    leftJoystick->retain();
    this->addChild(joystickBase, CONTROLLERS_Z);
}

void GameManager::initPressButton() {
    auto screenSize = Director::getInstance()->getVisibleSize();
    auto screenOrigin = Director::getInstance()->getVisibleOrigin();

    Rect attackButtonDimensions = Rect(0, 0, 64.0f, 64.0f);
    Point attackButtonPosition;
    attackButtonPosition = Point(screenOrigin.x + screenSize.width * 0.9f, screenOrigin.y + screenSize.height * 0.2f);

    SneakyButtonSkinnedBase *attackButtonBase = SneakyButtonSkinnedBase::create();
    attackButtonBase->setPosition(attackButtonPosition);

    attackButtonBase->setDefaultSprite(Sprite::create("joystickThumb.png"));
    attackButtonBase->setActivatedSprite(Sprite::create("joystickThumb.png"));
    attackButtonBase->setDisabledSprite(Sprite::create("joystickThumb.png"));
    attackButtonBase->setPressSprite(Sprite::create("joystickThumb.png"));

    SneakyButton *aAttackButton = SneakyButton::create();
    aAttackButton->initWithRect(attackButtonDimensions);

//  aAttackButton->setIsHoldable(true);
//  aAttackButton->setIsToggleable(true);

    attackButtonBase->setButton(aAttackButton);
    attackButtonBase->setPosition(attackButtonPosition);
    attackButtonBase->setOpacity(50);

    rightButton = attackButtonBase->getButton();
    rightButton->retain();

    this->addChild(attackButtonBase, CONTROLLERS_Z);
}