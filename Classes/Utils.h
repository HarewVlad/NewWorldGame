#pragma once
#include "cocos2d.h"

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