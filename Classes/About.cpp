#include "About.h"

About* About::create(const std::function<void(Ref *)> &func) {
  About *node = new (std::nothrow)About();
  if (node && node->init(func))
  {
    node->autorelease();
    return node;
  }

  CC_SAFE_DELETE(node);
  return nullptr;
}

static cocos2d::Label *createLabel(const std::string &text) {
  auto label = cocos2d::Label::createWithTTF("", "fonts/ThaleahFat.ttf", 40);
  label->setString(text);
  label->setColor(cocos2d::Color3B::WHITE);

  return label;
}

bool About::init(const std::function<void(Ref *)> &func) {
  auto origin = cocos2d::Director::getInstance()->getVisibleOrigin();
  auto visibleSize = cocos2d::Director::getInstance()->getVisibleSize();

  // Init autors
  std::vector<std::string> autors = { "Detective - hugues laborde", "Falling monsters - superdark", "Game background - sorry, can't find =(" };
  cocos2d::Vec2 position = { origin + visibleSize * 0.5f };
  for (auto autor : autors) {
    auto label = createLabel(autor);
    label->setPosition(position);
    this->addChild(label, 1);

    // Adjust position
    position.y += label->getContentSize().height;
  }

  // Back button
  backButton = Button::create("Buttons/Rect.png", cocos2d::Color3B::GRAY, [this](cocos2d::Ref *sender) {
    if (mainFunc != nullptr) {
      mainFunc(this);
    }
  });
  backButton->setText("Back", cocos2d::Color3B::WHITE);
  backButton->setPosition({ origin.x + visibleSize.width / 6.0f,
    origin.y + visibleSize.height / 8.0f });

  // Background
  background = cocos2d::Sprite::create("StartMenu/sumk1Y.jpg");
  background->setScaleX(visibleSize.width / background->getContentSize().width);
  background->setScaleY(visibleSize.height / background->getContentSize().height);
  background->setPosition(origin + visibleSize * 0.5f);

  this->mainFunc = func;

  this->addChild(background, 0);
  this->addChild(backButton, 1);

  return true;
}