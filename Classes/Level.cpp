//
// Created by Vlad on 12.09.2020.
//

#include "Level.h"

#define DEBUG_ENABLED

bool Level::init(Player *player, int numLines,
                 int numObjectsPerLine,
                 const std::function<void(Level *)> &func) {
  if (!Scene::initWithPhysics()) {
    return false;
  }

  auto origin = cocos2d::Director::getInstance()->getVisibleOrigin();
  auto visibleSize = cocos2d::Director::getInstance()->getVisibleSize();

  // Background
  {
    background = cocos2d::Sprite::create("Level/Background/background.png");
    background->setScaleX(visibleSize.width /
                          background->getContentSize().width);
    background->setScaleY(visibleSize.height /
                          background->getContentSize().height / 1.5f);
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
      line->init(numObjectsPerLine);
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
    controllerManager->init([this](cocos2d::Ref *sender) {
      switch (controllerManager->getState()) {
        case ControllerManagerState::LEFT_BUTTON_PRESSED:
        {
          // Move player left
          int playerLineIndex = this->player->getCurrentLineIndex();
          if (playerLineIndex != getLinesCount() - 1) {
            this->player->moveRight(getLine(playerLineIndex + 1));
          }
        }
        break;
        case ControllerManagerState::RIGHT_BUTTON_PRESSED:
        {
          // Move player right
          int playerLineIndex = this->player->getCurrentLineIndex();
          if (playerLineIndex != 0) {
            this->player->moveLeft(getLine(playerLineIndex - 1));
          }
        }
        break;
      }
    });

    this->addChild(controllerManager, static_cast<int>(Components::CONTROLLERS),
                   static_cast<int>(Components::CONTROLLERS));
  }

  // Score
  {
    scoreLabel = cocos2d::Label::createWithTTF("Score : " + std::to_string(score), "fonts/ThaleahFat.ttf", 40);
    scoreLabel->setPosition({ origin.x + visibleSize.width * 0.5f, origin.y + visibleSize.height * 0.8f });

    this->addChild(scoreLabel, static_cast<int>(Components::SCORE), static_cast<int>(Components::SCORE));
  }

  // Countdown
  {
    countDownLabel = cocos2d::Label::createWithTTF("" + std::to_string(score), "fonts/ThaleahFat.ttf", 45);
    countDownLabel->setPosition({ origin.x + visibleSize.width * 0.5f, origin.y + visibleSize.height * 0.7f });

    this->addChild(countDownLabel, static_cast<int>(Components::COUNTDOWN), static_cast<int>(Components::COUNTDOWN));
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

  // Hearts
  {
    Heart *heart = new Heart();
    (void)heart->init();
    heart->setPosition({ origin.x + visibleSize.width * 0.9f, origin.y + visibleSize.height * 0.9f });
    heart->setScale(2.0f);
    hearts.push_back(heart);
    this->addChild(heart, static_cast<int>(Components::LIVES), static_cast<int>(Components::LIVES));
  }

  // Ingame menu
  {
    // Ingame menu enter button
    auto menuEnterButton = Button::create("IngameMenu/Button/Square.png",
      cocos2d::Color3B::GRAY,
      [this](cocos2d::Ref *sender) {
      setPause();
    });
    menuEnterButton->setPosition(
      cocos2d::Vec2(origin.x + visibleSize.width / 8.0f,
        origin.y + visibleSize.height - visibleSize.height / 8.0f));
    menuEnterButton->setText("Menu", cocos2d::Color3B::WHITE);

    this->addChild(menuEnterButton, static_cast<int>(Components::INGAME_MENU),
      static_cast<int>(Components::INGAME_MENU));
  }

  // Weather
  {
    weatherManager = new WeatherManager();

    this->addChild(weatherManager, static_cast<int>(Components::WEATHER),
                   static_cast<int>(Components::WEATHER));
  }

  this->currentState = LevelState::NONE;
  this->score = 0;
  this->numLivesLeft = NUM_LIVES;
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
    // Weather
    if (!weatherManager->haveEffect(WeatherType::RAIN)) {
      weatherManager->addWeatherEffect(WeatherType::RAIN);
      weatherManager->runWeatherEffects();
    }

    // Score
    scoreLabel->setString("Score : " + std::to_string(score));

    // Lives
    // TODO: redo this shit
    switch (numLivesLeft) {
    case 0:
      hearts[numLivesLeft]->setVisible(false);
      break;
    }
  }
}

void Level::setReload() {
  for (auto line : lines) {
    line->setReload();
  }

  for (auto heart : hearts) {
    heart->setVisible(true);
  }

  score = 0;
  numLivesLeft = NUM_LIVES;
  setInitialPlayerPosition();

  setState(LevelState::NONE);
}

void Level::setStart(bool isDelay) {
  if (isDelay) {
    this->schedule(CC_SCHEDULE_SELECTOR(Level::startAfterDelay), 1 / 60.0f);
  } else {
    for (auto line : lines) {
      line->setStart();
    }

    setState(LevelState::RUN);
  }
}

void Level::startAfterDelay(float t) {
  static float time = TIME_TO_START;
  if (time > 0.0f) {
    time -= t;

    int timeCeiled = ceilf(time);
    if (timeCeiled != 0) {
      countDownLabel->setString(std::to_string(timeCeiled));
    } else {
      countDownLabel->setString("LET'S GO!");
    }
    
  } else {
    for (auto line : lines) {
      line->setStart();
    }

    setState(LevelState::RUN);

    time = TIME_TO_START;

    auto seq = cocos2d::Sequence::create(cocos2d::FadeOut::create(1.0f), 
      cocos2d::CallFunc::create([this]() {
      countDownLabel->setString("");
      countDownLabel->runAction(cocos2d::FadeIn::create(0));
    }), nullptr);
    countDownLabel->runAction(seq);

    this->unschedule(CC_SCHEDULE_SELECTOR(Level::startAfterDelay));
  }
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
    score = 0;

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

bool Level::onPhysicsContactPreSolve(cocos2d::PhysicsContact &contact, cocos2d::PhysicsContactPreSolve& solve) {
  auto nodeA = contact.getShapeA()->getBody()->getNode();
  auto nodeB = contact.getShapeB()->getBody()->getNode();

  auto contactPoint = contact.getContactData()->points[0];
  if (player->getBoundingBox().containsPoint(contactPoint)) {
    if (player->getCollisionState()) {
      if (numLivesLeft != 0) {
        numLivesLeft--;

        player->setHit();
      }
      else {
        setGameOver();
      }
    }
  }
  else if (player->getCollisionState()) {
    score += NUM_POINTS_PER_TICK;

    return false;
  }

  return true;
}

void Level::setInitialPlayerPosition() {
  auto visibleSize = cocos2d::Director::getInstance()->getVisibleSize();

  int playerSpawnLineIndex = cocos2d::random() % lines.size();
  auto playerSpawnPosition = lines[playerSpawnLineIndex]->getPosition();
  playerSpawnPosition.y -= visibleSize.height;

  player->stopAllActions();
  player->setLineIndex(playerSpawnLineIndex);
  player->setPosition(playerSpawnPosition);
}

void Level::setState(LevelState state) {
  if (currentState != state) {
    currentState = state;

    if (mainFunc != nullptr) {
      mainFunc(this);
    }
  }
}