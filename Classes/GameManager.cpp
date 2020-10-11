// TODO: Create baseclass Menu and than inh from it (GameOverMenu, IngameMenu
// ...)
// TODO: Redesign states
// TODO: Menu, Levels - different scenes

#include "GameManager.h"

USING_NS_CC;

Scene *GameManager::createScene() { return GameManager::create(); }

bool GameManager::init() {
  if (!Scene::init()) {
    return false;
  }

  auto visibleSize = Director::getInstance()->getVisibleSize();
  Vec2 origin = Director::getInstance()->getVisibleOrigin();

  // Start menu
  {
    startMenu = new StartMenu();
    assert(startMenu->init(CC_CALLBACK_1(GameManager::onStartMenu, this)));
  }

  // Ingame menu
  {
    ingameMenu = new IngameMenu();
    assert(ingameMenu->init(CC_CALLBACK_1(GameManager::onIngameMenu, this)));
  }
  
  // Player
  {
    player = new Player();
    player->init("detective.png");

    // Cache
    {
      cocos2d::SpriteFrameCache::getInstance()->addSpriteFramesWithFile(
        "detective_idle.plist");
      cocos2d::SpriteFrameCache::getInstance()->addSpriteFramesWithFile(
        "detective_run.plist");
    }

    // Animations
    {
      // Idle
      {
        auto frames = getSpriteFrames("Idle_%02d.png", 4);
        auto animation =
          cocos2d::Animation::createWithSpriteFrames(frames, 1 / 4.0f);
        animation->retain();

        player->addAnimation(PlayerState::IDLE, animation);
      }

      // Run
      {
        auto frames = getSpriteFrames("Run_%02d.png", 8);
        auto animation =
          cocos2d::Animation::createWithSpriteFrames(frames, 1 / 8.0f);
        animation->retain();

        player->addAnimation(PlayerState::MOVE_RIGHT, animation);
        player->addAnimation(PlayerState::MOVE_LEFT, animation);
      }
    }
  }

  // About
  {
    aboutWindow = About::create([this](cocos2d::Ref *sender) {
      cocos2d::Director::getInstance()->pushScene(cocos2d::TransitionFade::create(
        0.5, startMenu, cocos2d::Color3B(0, 0, 0)));
    });
    aboutWindow->retain();
  }

  // Level
  {
    level = new Level();
    level->init(player, 4, 10,
                CC_CALLBACK_1(GameManager::onLevel, this));
  }

  this->currentState = GameState::NONE;

  this->scheduleUpdate();

  return true;
}

void GameManager::update(float t) {
  if (currentState != GameState::MENU) {
    setMenu();
  }
}

void GameManager::onStartMenu(StartMenu *startMenu) {
  switch (startMenu->getState()) {
    case StartMenuState::START:
      setPlay();
      break;
    case StartMenuState::EXIT:
      cocos2d::Director::getInstance()->end();
      break;
    case StartMenuState::ABOUT:
      Director::getInstance()->pushScene(TransitionFade::create(
        0.5, aboutWindow, Color3B(0, 0, 0)));
      break;
    case StartMenuState::NONE:
      break;
  }
}

void GameManager::onIngameMenu(IngameMenu *ingameMenu) {
  switch (ingameMenu->getState()) {
  case IngameMenuState::RESET:
    setPlay();
    break;
  case IngameMenuState::RESUME:
    level->setStart(false);

    Director::getInstance()->pushScene(TransitionFade::create(
      0.5, level, Color3B(0, 0, 0)));
    break;
  case IngameMenuState::TO_MAIN_MENU:
    // Player has surrendered
    level->setGameOver();
    break;
  case IngameMenuState::NONE:
    level->setPause();
    break;
  }
}

void GameManager::onLevel(Level *level) {
  switch (level->getState()) {
    case LevelState::RUN:
      break;
    case LevelState::PAUSE:
      Director::getInstance()->pushScene(TransitionFade::create(
        0.5, ingameMenu, Color3B(0, 0, 0)));
      break;
    case LevelState::GAME_OVER:
      setMenu();
      break;
    case LevelState::WIN:
      break;
    case LevelState::NONE:
      break;
  }
}

void GameManager::setMenu() {
  currentState = GameState::MENU;

  Director::getInstance()->pushScene(
    TransitionFade::create(0.5, startMenu, Color3B(0, 0, 0)));
}

void GameManager::setPlay() {
  level->setReload();
  level->setStart(true);

  currentState = GameState::PLAY;

  Director::getInstance()->pushScene(TransitionFade::create(
    0.5, level, Color3B(0, 0, 0)));
}