#include "Heart.h"

bool Heart::init() {
  heart = cocos2d::Sprite::create("Heart/heart.png");
  background = cocos2d::Sprite::create("Heart/background.png");
  border = cocos2d::Sprite::create("Heart/border.png");

  this->addChild(background);
  this->addChild(border);
  this->addChild(heart);

  return true;
}