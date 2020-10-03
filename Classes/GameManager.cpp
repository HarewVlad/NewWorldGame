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
    case StartMenuState::NONE:
      break;
  }
}

// TODO: not in use now
void GameManager::onGameOverMenu(GameOverMenu *gameOverMenu) {
  switch (gameOverMenu->getState()) {
    case GameOverMenuState::RESTART:
      setPlay();
      break;
    case GameOverMenuState::TO_MAIN_MENU:
      setMenu();
      break;
    case GameOverMenuState::NONE:
      break;
  }
}
//

void GameManager::onLevel(Level *level) {
  switch (level->getState()) {
    case LevelState::RUN:
      break;
    case LevelState::PAUSE:
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
  levelManager->reloadCurrentLevel();
  levelManager->startCurrentLevel();

  currentState = GameState::PLAY;

  Director::getInstance()->pushScene(TransitionFade::create(
    0.5, levelManager->getCurrentLevel(), Color3B(0, 0, 0)));
}