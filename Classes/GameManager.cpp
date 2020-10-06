// TODO: Create baseclass Menu and than inh from it (GameOverMenu, IngameMenu
// ...)
// TODO: Redesign states
// TODO: Menu, Levels - different scenes

#include "GameManager.h"

USING_NS_CC;

Scene *GameManager::createScene() { return GameManager::create(); }

static cocos2d::Vector<cocos2d::SpriteFrame *> getSpriteFrames(const char *fmt,
  int count) {
  auto spriteFrameCache = cocos2d::SpriteFrameCache::getInstance();
  cocos2d::Vector<cocos2d::SpriteFrame *> spriteFrames;
  char str[256];
  for (int i = 1; i < count; ++i) {
    sprintf(str, fmt, i);
    spriteFrames.pushBack(spriteFrameCache->getSpriteFrameByName(str));
  }
  return spriteFrames;
}

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

  // Game over menu
  {
    gameOverMenu = new GameOverMenu();
    assert(
        gameOverMenu->init(CC_CALLBACK_1(GameManager::onGameOverMenu, this)));
  }
  
  // Player
  {
    player = new Player();
    player->init("detective.png");

    /*
    // Sprite frame cache
    {
      cocos2d::SpriteFrameCache::getInstance()->addSpriteFramesWithFile(
        "elf_idle.plist");
      cocos2d::SpriteFrameCache::getInstance()->addSpriteFramesWithFile(
        "elf_walk.plist");
    }
    */

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


    /*
    // Animations
    {
      // Idle
      {
        auto frames = getSpriteFrames("Elf_M_Idle_%d.png", 4);
        auto animation =
          cocos2d::Animation::createWithSpriteFrames(frames, 1 / 4.0f);
        animation->retain();

        player->addAnimation(PlayerState::IDLE, animation);
      }

      // Run
      {
        auto frames = getSpriteFrames("Elf_M_Walk_%d.png", 4);
        auto animation =
          cocos2d::Animation::createWithSpriteFrames(frames, 1 / 4.0f);
        animation->retain();

        player->addAnimation(PlayerState::MOVE_FORWARD, animation);
        player->addAnimation(PlayerState::MOVE_RIGHT, animation);
        player->addAnimation(PlayerState::MOVE_LEFT, animation);
      }
    }
    */
  }

  // Level
  {
    level = new Level();
    level->init(player,
                {ObjectType::BEER, ObjectType::FISH}, 4, 10, 3.0f,
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
  level->setReload();
  level->setStart();

  currentState = GameState::PLAY;

  Director::getInstance()->pushScene(TransitionFade::create(
    0.5, level, Color3B(0, 0, 0)));
}