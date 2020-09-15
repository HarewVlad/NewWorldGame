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
        startMenuManager = new StartMenuManager();
        startMenuManager->init("menuBackground.jpg");

        this->addChild(startMenuManager, static_cast<int>(Components::MENU), static_cast<int>(Components::MENU));
    }

    // Ingame menu
    {
        ingameMenuManager = new IngameMenuManager();
        (void)ingameMenuManager->init();

        this->addChild(ingameMenuManager, static_cast<int>(Components::INGAME_MENU), static_cast<int>(Components::INGAME_MENU));
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

                player->addAnimation(PlayerState::MOVE, animation);
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

    schedule(CC_SCHEDULE_SELECTOR(GameManager::update), 1 / 75.0f);

    return true;
}
// TODO: can be a problem, when entering menu, other buttons can still be active
void GameManager::update(float t) {
    if (currentGameState == GameState::MENU) {
        StartMenuState startMenuState = startMenuManager->getState();
        switch (startMenuState) {
            case StartMenuState::IDLE:
                break;
            case StartMenuState::START:
                // Start activities
                levelManager->startCurrentLevel();
                // Hide menu
                startMenuManager->hide();
                // Change states
                ingameMenuManager->setState(IngameMenuState::IDLE);
                currentGameState = GameState::PLAY;
                break;
            case StartMenuState::EXIT:
                cocos2d::Director::getInstance()->end();
                break;
        }
    } else if (currentGameState == GameState::PLAY) {
        {
            auto joystickPosition = controllerManager->getStickPosition().getNormalized();
            auto isButtonPressed = controllerManager->getValue();

            // Movement
            int playerLineIndex = player->getCurrentLine();
            if (joystickPosition.x > 0) {
                if (playerLineIndex != levelManager->getCurrentLevel()->getLinesCount()) {
                    player->moveToLine(t, levelManager->getCurrentLevel()->getLine(
                            playerLineIndex + 1));
                }
            }
            if (joystickPosition.x < 0) {
                if (playerLineIndex != 0) {
                    player->moveToLine(t, levelManager->getCurrentLevel()->getLine(
                            playerLineIndex - 1));
                }
            }
            if (joystickPosition.y != 0) {
                player->moveForward(t, joystickPosition.y); // TODO: set maximum amount what player can reach while move forward
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
            IngameMenuState ingameMenuState = ingameMenuManager->getState();
            switch (ingameMenuState) {
                case IngameMenuState::IDLE:
                    break;
                case IngameMenuState::PAUSE:
                    // Pause activities
                    levelManager->pauseCurrentLevel();
                    // Show menu
                    startMenuManager->show();
                    // Change states
                    startMenuManager->setState(StartMenuState::IDLE);
                    currentGameState = GameState::MENU;
                    break;
            }
        }
    }
}