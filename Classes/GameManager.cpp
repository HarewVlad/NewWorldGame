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
            cocos2d::SpriteFrameCache::getInstance()->addSpriteFramesWithFile("adventurer_idle.plist");
            cocos2d::SpriteFrameCache::getInstance()->addSpriteFramesWithFile("adventurer_walk.plist");
            cocos2d::SpriteFrameCache::getInstance()->addSpriteFramesWithFile("adventurer_jump.plist");
            cocos2d::SpriteFrameCache::getInstance()->addSpriteFramesWithFile("adventurer_fall.plist");
            cocos2d::SpriteFrameCache::getInstance()->addSpriteFramesWithFile("adventurer_attack1.plist");
            cocos2d::SpriteFrameCache::getInstance()->addSpriteFramesWithFile("adventurer_attack2.plist");
        }
    }

    // Debug
    {
        this->getPhysicsWorld()->setGravity(cocos2d::Vec2(0, 0));
        this->getPhysicsWorld()->setSubsteps(3);
        // this->getPhysicsWorld()->setDebugDrawMask(0xffff);
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    // Start menu
    {
        startMenu = new StartMenu();
        startMenu->init("menuBackground.jpg");

        this->addChild(startMenu, static_cast<int>(Components::MENU), static_cast<int>(Components::MENU));
    }

    // Ingame menu
    {
        ingameMenu = new IngameMenu();
        (void)ingameMenu->init();

        this->addChild(ingameMenu, static_cast<int>(Components::INGAME_MENU), static_cast<int>(Components::INGAME_MENU));
    }

    // Game over menu
    {
        gameOverMenu = new GameOverMenu();
        (void)gameOverMenu->init();

        this->addChild(gameOverMenu, static_cast<int>(Components::GAME_OVER_MENU), static_cast<int>(Components::GAME_OVER_MENU));
    }

    // Levels
    {
        levelManager = new LevelManager();
        levelManager->init();

        // Level 1
        {
            Level *level = new Level();
            level->init({ObjectType::CAR}, 4, 10);

            levelManager->addLevel(1, level);
        }

        this->addChild(levelManager, static_cast<int>(Components::LEVELS), static_cast<int>(Components::LEVELS));
    }

    // Weather
    {
        weatherManager = new WeatherManager();

        this->addChild(weatherManager, static_cast<int>(Components::WEATHER), static_cast<int>(Components::WEATHER));
    }

    // Player
    {
        player = new Player();
        player->init("adventurer.png");
        player->setSpawnData(levelManager->getCurrentLevel()->getPlayerSpawnData());

        // Animations
        {
            // Idle
            {
                auto frames = getSpriteFrames("adventurer-idle-%02d.png", 4);
                auto animation = cocos2d::Animation::createWithSpriteFrames(frames, 1 / 4.0f);
                animation->retain();

                player->addAnimation(PlayerState::IDLE, animation);
            }

            // Run
            {
                auto frames = getSpriteFrames("adventurer-walk-%02d.png", 6);
                auto animation = cocos2d::Animation::createWithSpriteFrames(frames, 1 / 6.0f);
                animation->retain();

                player->addAnimation(PlayerState::MOVE_FORWARD, animation);
                player->addAnimation(PlayerState::MOVE_RIGHT, animation);
                player->addAnimation(PlayerState::MOVE_LEFT, animation);
            }

            // Attack
            {
                auto frames = getSpriteFrames("adventurer-attack2-%02d.png", 6);
                auto animation = cocos2d::Animation::createWithSpriteFrames(frames, 1 / 6.0f);
                animation->retain();

                player->addAnimation(PlayerState::ATTACK, animation);
            }
        }

        this->addChild(player, static_cast<int>(Components::PLAYER), static_cast<int>(Components::PLAYER));
    }

    // Controllers
    {
        controllerManager = new ControllerManager();
        controllerManager->init(origin + visibleSize * 0.5f);

        this->addChild(controllerManager, static_cast<int>(Components::CONTROLLERS), static_cast<int>(Components::CONTROLLERS));
    }

    // Contact listener
    {
        auto contactListener = cocos2d::EventListenerPhysicsContact::create();
        contactListener->onContactBegin = CC_CALLBACK_1(GameManager::onPhysicsContactBegin, this);
        _eventDispatcher->addEventListenerWithSceneGraphPriority(contactListener, this);
    }

    schedule(CC_SCHEDULE_SELECTOR(GameManager::update), 1 / 75.0f);

    return true;
}
// TODO: can be a problem, when entering menu, other buttons can still be active
void GameManager::update(float t) {
    if (currentState == GameState::MENU) {
        StartMenuState startMenuState = startMenu->getState();
        switch (startMenuState) {
            case StartMenuState::IDLE:
                break;
            case StartMenuState::START:
                // Start activities
                levelManager->startCurrentLevel();
                // Hide menu
                startMenu->hide();
                // Change state
                currentState = GameState::PLAY;
                break;
            case StartMenuState::EXIT:
                cocos2d::Director::getInstance()->end();
                break;
        }
    } else if (currentState == GameState::PLAY) {
        {
            auto joystickPosition = controllerManager->getStickPosition().getNormalized();
            auto isButtonPressed = controllerManager->getValue();

            // Movement
            int playerLineIndex = player->getCurrentLineIndex();
            if (joystickPosition.x > 0) {
                if (playerLineIndex != levelManager->getCurrentLevel()->getLinesCount() - 1) {
                    player->moveRight(t, levelManager->getCurrentLevel()->getLine(
                            playerLineIndex + 1));
                }
            } else if (joystickPosition.x < 0) {
                if (playerLineIndex != 0) {
                    player->moveLeft(t, levelManager->getCurrentLevel()->getLine(
                            playerLineIndex - 1));
                }
            }
            if (joystickPosition.y != 0) {
                player->moveForward(t, joystickPosition.y);
            }

            if (isButtonPressed) {
                player->attack(t);
            }

            player->update(t);

            controllerManager->setValue(false); // 75 fps causes sneakyButton to register multiple presses on one press
        }

        // World
        {
            levelManager->update(t);
        }

        // Weather
        {
            // TODO: change later
            if (!weatherManager->haveEffect(WeatherType::RAIN)) {
                weatherManager->addWeatherEffect(WeatherType::RAIN);
                weatherManager->runWeatherEffects();
            }
        }

        // Ingame menu
        {
            IngameMenuState ingameMenuState = ingameMenu->getState();
            switch (ingameMenuState) {
                case IngameMenuState::IDLE:
                    break;
                case IngameMenuState::PAUSE:
                    // Pause activities
                    levelManager->pauseCurrentLevel();
                    // Show menu
                    startMenu->show();
                    // Change states
                    currentState = GameState::MENU;
                    break;
            }
        }
    } else if (currentState == GameState::GAME_OVER) {
        GameOverState gameOverState = gameOverMenu->getState();
        switch (gameOverState) {
            case GameOverState::IDLE:
                break;
            case GameOverState::RESTART:
                levelManager->getCurrentLevel()->restart();
                // Change state
                currentState = GameState::PLAY;
                break;
            case GameOverState::TO_MAIN_MENU:
                // Pause activities
                levelManager->pauseCurrentLevel();
                // Hide game over menu
                gameOverMenu->hide();
                // Show start menu
                startMenu->show();
                // Change state
                currentState = GameState::MENU;
                break;
        }
    }
}

bool GameManager::onPhysicsContactBegin(cocos2d::PhysicsContact &contact) {
    auto nodeA = contact.getShapeA()->getBody()->getNode();
    auto nodeB = contact.getShapeB()->getBody()->getNode();

    if (nodeA && nodeB) {
        int playerTag = player->getTag();
        if (nodeA->getTag() == playerTag) {
            // Set game over
            currentState = GameState::GAME_OVER;

            // Show menu
            gameOverMenu->show();
        } else if (nodeB->getTag() == playerTag) {
            // Set game over
            currentState = GameState::GAME_OVER;

            // Show menu
            gameOverMenu->show();
        }
    }

    return true;
}