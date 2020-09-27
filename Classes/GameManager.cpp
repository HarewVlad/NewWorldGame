// TODO: Create baseclass Menu and than inh from it (GameOverMenu, IngameMenu ...)
// TODO: Redesign states
// TODO: Menu, Levels - different scenes

#include "GameManager.h"

USING_NS_CC;

static cocos2d::Vector<cocos2d::SpriteFrame *> getSpriteFrames(const char *fmt, int count) {
    auto spriteFrameCache = cocos2d::SpriteFrameCache::getInstance();
    cocos2d::Vector<cocos2d::SpriteFrame *> spriteFrames;
    char str[256];
    for (int i = 1; i < count; ++i) {
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
            cocos2d::SpriteFrameCache::getInstance()->addSpriteFramesWithFile("elf_idle.plist");
            cocos2d::SpriteFrameCache::getInstance()->addSpriteFramesWithFile("elf_walk.plist");
        }
    }

    // Debug
    {
        this->getPhysicsWorld()->setGravity(cocos2d::Vec2(0, 0));
        this->getPhysicsWorld()->setSubsteps(3);
        this->getPhysicsWorld()->setDebugDrawMask(0xffff);
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    // Start menu
    {
        startMenu = new StartMenu();
        (void)startMenu->init("menuBackground.jpg", CC_CALLBACK_1(GameManager::onStartMenu, this));
    }

    // Ingame menu
    {
        ingameMenu = new IngameMenu();
        (void)ingameMenu->init(CC_CALLBACK_1(GameManager::onIngameMenu, this));

        this->addChild(ingameMenu, static_cast<int>(Components::INGAME_MENU), static_cast<int>(Components::INGAME_MENU));
    }

    // Game over menu
    {
        gameOverMenu = new GameOverMenu();
        (void)gameOverMenu->init(CC_CALLBACK_1(GameManager::onGameOverMenu, this));

        this->addChild(gameOverMenu, static_cast<int>(Components::GAME_OVER_MENU), static_cast<int>(Components::GAME_OVER_MENU));
    }

    // Levels
    {
        levelManager = new LevelManager();
        levelManager->init();

        // Level 1
        {
            Level *level = new Level();
            level->init({ObjectType::BEER, ObjectType::FISH}, 4, 10, 0.9f);

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
        player->init("elf.png");
        player->setSpawnData(levelManager->getCurrentLevel()->getPlayerSpawnData());

        // Animations
        {
            // Idle
            {
                auto frames = getSpriteFrames("Elf_M_Idle_%d.png", 4);
                auto animation = cocos2d::Animation::createWithSpriteFrames(frames, 1 / 4.0f);
                animation->retain();

                player->addAnimation(PlayerState::IDLE, animation);
            }

            // Run
            {
                auto frames = getSpriteFrames("Elf_M_Walk_%d.png", 4);
                auto animation = cocos2d::Animation::createWithSpriteFrames(frames, 1 / 4.0f);
                animation->retain();

                player->addAnimation(PlayerState::MOVE_FORWARD, animation);
                player->addAnimation(PlayerState::MOVE_RIGHT, animation);
                player->addAnimation(PlayerState::MOVE_LEFT, animation);
            }

            // Attack
            {
                
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

    this->scheduleUpdate();

    // Set initial screen
    Director::getInstance()->replaceScene(startMenu);

    return true;
}
// TODO: can be a problem, when entering menu, other buttons can still be active
void GameManager::update(float t) {
    if (currentState == GameState::PLAY) {
        {
            auto joystickPosition = controllerManager->getStickPosition().getNormalized();
            auto isButtonPressed = controllerManager->getValue();

            // Movement
            int playerLineIndex = player->getCurrentLineIndex();
            if (joystickPosition.x > 0.5f) {
                if (playerLineIndex != levelManager->getCurrentLevel()->getLinesCount() - 1) {
                    player->moveRight(t, levelManager->getCurrentLevel()->getLine(
                            playerLineIndex + 1));
                }
            } else if (joystickPosition.x < -0.5f) {
                if (playerLineIndex != 0) {
                    player->moveLeft(t, levelManager->getCurrentLevel()->getLine(
                            playerLineIndex - 1));
                }
            } else if (joystickPosition.y > 0.5f) {
                player->moveForward(t);
            } else if (joystickPosition.y < -0.5f) {
                player->moveBackward(t);
            }

            if (isButtonPressed) {
                player->attack(t);
            }

            controllerManager->setValue(
                    false); // 75 fps causes sneakyButton to register multiple presses on one press
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

bool GameManager::onPhysicsContactBegin(cocos2d::PhysicsContact &contact) {
    auto nodeA = contact.getShapeA()->getBody()->getNode();
    auto nodeB = contact.getShapeB()->getBody()->getNode();

    if (nodeA && nodeB) {
        int playerTag = player->getTag();
        if (nodeA->getTag() == playerTag ||
            nodeB->getTag() == playerTag) {
            setGameOverState();
        }
    }

    return true;
}

void GameManager::onStartMenu(StartMenu *startMenu) {
    switch (startMenu->getState()) {
        case StartMenuState::START:
            setPlayState();
            break;
        case StartMenuState::EXIT:
            cocos2d::Director::getInstance()->end();
            break;
        case StartMenuState::NONE:
            break;
    }
}

void GameManager::onGameOverMenu(GameOverMenu *gameOverMenu) {
    switch (gameOverMenu->getState()) {
        case GameOverMenuState::RESTART:
            setPlayState();
            break;
        case GameOverMenuState::TO_MAIN_MENU:
            setMenuState();
            break;
        case GameOverMenuState::NONE:
            break;
    }
}

void GameManager::onIngameMenu(IngameMenu *ingameMenu) {
    switch (ingameMenu->getState()) {
        case IngameMenuState::PAUSE:
            setPauseState();
            break;
        case IngameMenuState::NONE:
            break;
    }
}

void GameManager::setPauseState() {
    // Pause activities
    levelManager->pauseCurrentLevel();

    // Show ingame menu
    Director::getInstance()->replaceScene(startMenu); // TODO: make and show ingame menu

    // Change states
    currentState = GameState::PAUSE;
}
void GameManager::setPlayState() {
    // Restart activities
    levelManager->reloadCurrentLevel();
    levelManager->startCurrentLevel();
    player->setSpawnData(levelManager->getCurrentLevel()->getPlayerSpawnData());

    // Change state
    currentState = GameState::PLAY;
}
void GameManager::setGameOverState() {
    // Set game over
    currentState = GameState::GAME_OVER;

    // Show menu
    gameOverMenu->show();
}
void GameManager::setMenuState() {
    // Pause activities
    levelManager->pauseCurrentLevel();

    // Show start menu
    Director::getInstance()->replaceScene(startMenu);

    // Change state
    currentState = GameState::MENU;
}