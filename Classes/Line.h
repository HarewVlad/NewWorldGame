//
// Created by Vlad on 16.09.2020.
//

#ifndef PROJ_ANDROID_LINE_H
#define PROJ_ANDROID_LINE_H

#include "cocos2d.h"

enum class ObjectType {
    CAR,
    UFO
    // ...
};

struct Object {
    ObjectType type;
    cocos2d::Sprite *sprite;
};

enum class LineState {
    IDLE,
    RUNNING,
    FINISHED
};

struct Line : public cocos2d::Node { // TODO: move to Line.cpp / Line.h
    void init(const cocos2d::Vec2 &position, const std::vector<ObjectType> &objectsVariation, int numObjectsPerLine);
    void update(float t);
    void restart();
    inline void setState(LineState state) {
        currentState = state;
    }
    inline cocos2d::Vec2 getPosition() {
        return position;
    }
private:
    Object createObject(ObjectType type, float scale);
    std::string getObjectSource(ObjectType type) const;
private:
    cocos2d::Vec2 position;
    std::vector<Object> objects; // All objects of a monster spawn line
    std::vector<Object> renderingObjects; // Objects that are actually rendering on the screen
    float time;
    float frequency;
    float delay;
    int lastRenderingObject;
    LineState currentState;

    // Constants
    const float MAX_DELAY = 5.5f;
    const float MIN_SPAWN_FREQUENCY = 0.5f;
    const float MAX_SPAWN_FREQUENCY = 5.0f;
};

#endif //PROJ_ANDROID_LINE_H
