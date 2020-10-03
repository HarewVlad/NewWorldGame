//
// Created by Vlad on 12.09.2020.
//

#include "Level.h"

#define DEBUG_ENABLED

bool Level::init(Player *player,
                 const std::string &backgroundFileName,
                 const std::string &ingameMenuBackgroundFileName,
                 const std::vector<ObjectType> &objectsVariation, int numLines,
                 int numObjectsPerLine, float speed,
                 const std::function<void(Level *)> &func) {
  if (!Scene::initWithPhysics()) {
    return false;
  }

  auto origin = cocos2d::Director::getInstance()->getVisibleOrigin();
  auto visibleSize = cocos2d::Director::getInstance()->getVisibleSize();

  // Background
  {
    background = cocos2d::Sprite::create(backgroundFileName);
    background->setScaleX(visibleSize.width /
                          background->getContentSize().width);
    background->setPosition(origin + visibleSize * 0.5f);

    this->addChild(background, static_cast<int>(Components::BACKGROUND),
                   static_cast<int>(Components::BACKGROUND));
  }

  // Lines
  {
    float x = visibleSize.width / (numLines + 1);
    for (int i = 0; i < numLines; ++i) {
      float y = visibleSize.height + visibleSize.height / 4.0f;

      Line *line = new Line();
      line->init(objectsVariation, numObjectsPerLine, speed);
      line->setPosition({x, y});
      lines.push_back(line);

      x += visibleSize.width / (numLines + 1);
    }

    for (auto line : lines) {
      this->addChild(line, static_cast<int>(Components::LINES));
    }
  }

  // Controllers
  {
    controllerManager = new ControllerManager();
    controllerManager->init(origin + visibleSize * 0.5f);

    this->addChild(controllerManager, static_cast<int>(Components::CONTROLLERS),
                   static_cast<int>(Components::CONTROLLERS));
  }

  // Label
  {
    scoreLabel = cocos2d::Label::createWithTTF("Score: " + std::to_string(score), "fonts/Marker Felt.ttf", 24);
    scoreLabel->setPosition({ origin.x + visibleSize.width * 0.5f, origin.y + visibleSize.height * 0.8f });

    this->addChild(scoreLabel, static_cast<int>(Components::SCORE), static_cast<int>(Components::SCORE));
  }
  
  // Player
  {
    this->player = player;
    setInitialPlayerPosition();

    this->addChild(player, static_cast<int>(Components::PLAYER),
      static_cast<int>(Components::PLAYER));
  }

  // Contact listener
  {
    auto contactListener = cocos2d::EventListenerPhysicsContact::create();
    contactListener->onContactBegin =
        CC_CALLBACK_1(Level::onPhysicsContactBegin, this);
    contactListener->onContactPreSolve =
        CC_CALLBACK_2(Level::onPhysicsContactPreSolve, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(contactListener, this);
  }

  // Ingame menu
  {
    ingameMenu = new IngameMenu();
    assert(ingameMenu->init(ingameMenuBackgroundFileName,
                            CC_CALLBACK_1(Level::onIngameMenu, this)));

    this->addChild(ingameMenu, static_cast<int>(Components::INGAME_MENU),
                   static_cast<int>(Components::INGAME_MENU));
  }

  // Weather
  {
    weatherManager = new WeatherManager();

    this->addChild(weatherManager, static_cast<int>(Components::WEATHER),
                   static_cast<int>(Components::WEATHER));
  }

  this->currentState = LevelState::NONE;
  this->score = 0.0f;
  this->mainFunc = func;

  this->getPhysicsWorld()->setGravity(cocos2d::Vec2(0, 0));
  this->getPhysicsWorld()->setSubsteps(3);

#ifdef DEBUG_ENABLED
  this->getPhysicsWorld()->setDebugDrawMask(0xffff);
#endif

  this->scheduleUpdate();

  return true;
}

void Level::update(float t) {
  if (currentState == LevelState::RUN) {
    auto joystickPosition =
        controllerManager->getStickPosition();
    cocos2d::log("Position -> {%f, %f}\n", joystickPosition.x, joystickPosition.y);
    auto isButtonPressed = controllerManager->getValue();

    // Update player position
    int playerLineIndex = player->getCurrentLineIndex();
    if (joystickPosition.x > INPUT_EPSILON) {
      if (playerLineIndex != getLinesCount() - 1) {
        player->moveRight(t, getLine(playerLineIndex + 1));
      }
    } else if (joystickPosition.x < -INPUT_EPSILON) {
      if (playerLineIndex != 0) {
        player->moveLeft(t, getLine(playerLineIndex - 1));
      }
    } else if (joystickPosition.y > INPUT_EPSILON) {
      player->moveForward(t);
    } else if (joystickPosition.y < -INPUT_EPSILON) {
      player->moveBackward(t);
    }

    if (isButtonPressed) {
      player->attack(t);
    }

    controllerManager->setValue(false);

    // Weather
    if (!weatherManager->haveEffect(WeatherType::RAIN)) {
      weatherManager->addWeatherEffect(WeatherType::RAIN);
      weatherManager->runWeatherEffects();
    }

    // Score
    scoreLabel->setString("Score : " + std::to_string(score));
  }
}

void Level::setReload() {
  for (auto line : lines) {
    line->setReload();
  }

  score = 0.0f;
  setInitialPlayerPosition();

  setState(LevelState::NONE);
}

void Level::setStart() {
  for (auto line : lines) {
    line->setStart();
  }

  setState(LevelState::RUN);
}

void Level::setPause() {
  if (currentState == LevelState::RUN) {
    for (auto line : lines) {
      line->setPause();
    }

    setState(LevelState::PAUSE);
  }
}

void Level::setGameOver() {
  if (currentState != LevelState::GAME_OVER) {
    score = 0.0f;

    setState(LevelState::GAME_OVER);
  }
}

bool Level::onPhysicsContactBegin(cocos2d::PhysicsContact &contact) {
  auto shapeA = contact.getShapeA();
  auto shapeB = contact.getShapeB();

  auto nodeA = shapeA->getBody()->getNode();
  auto nodeB = shapeB->getBody()->getNode();
  
  if (nodeA && nodeB) {
    int playerTag = player->getTag();
    if (nodeA->getTag() == playerTag || nodeB->getTag() == playerTag) {
      return true;
    }
  }
  return false;
}

bool Level::onPhysicsContactPreSolve(cocos2d::PhysicsContact &contact, PhysicsContactPreSolve& solve) {
  auto nodeA = contact.getShapeA()->getBody()->getNode();
  auto nodeB = contact.getShapeB()->getBody()->getNode();

  auto contactPoint = contact.getContactData()->points[0];
  if (player->getBoundingBox().containsPoint(contactPoint)) {
    setGameOver();
  }
  else {
    score += nodeA->getPosition().distance(nodeB->getPosition()) * 0.5f;

    return false;
  }

  return true;
}

void Level::setInitialPlayerPosition() {
  auto visibleSize = cocos2d::Director::getInstance()->getVisibleSize();

  int playerSpawnLineIndex = cocos2d::random() % lines.size();
  auto playerSpawnPosition = lines[playerSpawnLineIndex]->getPosition();
  playerSpawnPosition.y -= visibleSize.height;

  player->setLineIndex(playerSpawnLineIndex);
  player->setPosition(playerSpawnPosition);
}

void Level::onIngameMenu(IngameMenu *ingameMenu) {
  switch (ingameMenu->getState()) {
    case IngameMenuState::RESET:
      setReload();
      setStart();
      break;
    case IngameMenuState::RESUME:
      setStart();
      break;
    case IngameMenuState::TO_MAIN_MENU:
      // Player has surrendered
      setGameOver();
      break;
    case IngameMenuState::NONE:
      setPause();
      break;
  }
}

void Level::setState(LevelState state) {
  if (currentState != state) {
    currentState = state;

    if (mainFunc != nullptr) {
      mainFunc(this);
    }
  }
}