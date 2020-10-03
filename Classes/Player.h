//
// Created by Vlad on 04.09.2020.
//

#ifndef PROJ_ANDROID_PLAYER_H
#define PROJ_ANDROID_PLAYER_H

#include "Line.h"
#include "cocos2d.h"

enum class PlayerState {
  NONE,
  IDLE,
  MOVE_FORWARD,
  MOVE_BACKWARD,
  MOVE_RIGHT,
  MOVE_LEFT,
  ATTACK,
};

// TODO: Design and architecture
struct Player : public cocos2d::Node {
  void init(const std::string &filename);
  void addAnimation(PlayerState state, cocos2d::Animation *animation);
  inline PlayerState getState() { return currentState; }
  inline cocos2d::Sprite *getSprite() { return sprite; }
  inline void setLineIndex(int lineIndex) { currentLineIndex = lineIndex; }
  inline int getCurrentLineIndex() const { return currentLineIndex; }
  inline int getTag() { return TAG; }
  void setState(PlayerState state);
  void moveRight(float t, Line *line);
  void moveLeft(float t, Line *line);
  void moveForward(float t);
  void moveBackward(float t);
  void attack(float t);
  void update(float t) override;
  cocos2d::Rect getExpandZone();
  cocos2d::Rect getBoundingBox();

 private:
  void setAttackState();
  void setIdleState();
  void setMoveForwardState();
  void setMoveBackwardState();
  void setMoveRightState();
  void setMoveLeftState();
  bool isNotMoving();

 private:
  cocos2d::Sprite *sprite;
  std::unordered_map<int, cocos2d::Animation *> animations;
  int currentLineIndex;
  PlayerState currentState;

  cocos2d::Rect *expandZone;

  // Constants
  const float SCALE = 1.0f;
  const float SPEED = 50.0f;
  const float EXPAND_ZONE_OFFSET = 10.0f;
  const int TAG = 0x33;
};

#endif  // PROJ_ANDROID_PLAYER_H
