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
    assert(startMenu->init("menuBackground.jpg",
                           CC_CALLBACK_1(GameManager::onStartMenu, this)));
  }

  // Game over menu
  {
    gameOverMenu = new GameOverMenu();
    assert(
        gameOverMenu->init(CC_CALLBACK_1(GameManager::onGameOverMenu, this)));
  }

  // Levels
  {
    levelManager = new LevelManager();
    levelManager->init();

    // Level 1
    {
      // TODO: fix level architecture (Make 1 scene and reuse it)
      Level *level = new Level();
      level->init("ingameMenuBackground.png", "worldBackground.png",
                  {ObjectType::BEER, ObjectType::FISH}, 4, 10, 0.9f,
                  CC_CALLBACK_1(GameManager::onLevel, this));

      levelManager->addLevel(1, level);
    }
  }

  this->currentState = GameState::NONE;

  this->scheduleUpdate();

  return true;
}

void GameManager::update(float t) {
  if (currentState != GameState::MENU) {
    setState(GameState::MENU);
  }
}

void GameManager::onStartMenu(StartMenu *startMenu) {
  switch (startMenu->getState()) {
    case StartMenuState::START:
      setState(GameState::PLAY);
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
      setState(GameState::PLAY);
      break;
    case GameOverMenuState::TO_MAIN_MENU:
      setState(GameState::MENU);
      break;
    case GameOverMenuState::NONE:
      break;
  }
}

void GameManager::onLevel(Level *level) {
  switch (level->getState()) {
    case LevelState::RUN:
      break;
    case LevelState::PAUSE:
      setState(GameState::PAUSE);
      break;
    case LevelState::GAME_OVER:
      setState(GameState::GAME_OVER);
      break;
    case LevelState::WIN:
      break;
    case LevelState::NONE:
      break;
  }
}

void GameManager::setState(GameState state) {
  switch (state) {
    case GameState::MENU: {
      levelManager->pauseCurrentLevel();

      currentState = GameState::MENU;

      Director::getInstance()->replaceScene(
          TransitionFade::create(0.5, startMenu, Color3B(0, 255, 255)));
    } break;
    case GameState::PLAY: {
      levelManager->startCurrentLevel();

      currentState = GameState::PLAY;

      Director::getInstance()->replaceScene(TransitionFade::create(
          0.5, levelManager->getCurrentLevel(), Color3B(0, 255, 255)));
    } break;
    case GameState::PAUSE: {
      currentState = GameState::PAUSE;

      Director::getInstance()->replaceScene(startMenu);
    } break;
    case GameState::GAME_OVER: {
      currentState = GameState::GAME_OVER;

      Director::getInstance()->replaceScene(startMenu);
    } break;
    case GameState::NONE:
      break;
  }
}