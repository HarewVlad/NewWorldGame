//
// Created by Vlad on 12.09.2020.
//

#ifndef PROJ_ANDROID_LEVEL_H
#define PROJ_ANDROID_LEVEL_H

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
    int lastRenderingObject;
    LineState currentState;

    // Constants
    const float MAX_DELAY = 5.5f;
    const float MIN_SPAWN_FREQUENCY = 0.5f;
    const float MAX_SPAWN_FREQUENCY = 5.0f;
};

struct PlayerSpawnData {
    cocos2d::Vec2 position;
    int lineIndex;
};

// TODO: store Lines in array, determine lines positions automatically
struct Level : public cocos2d::Node {
    void init(const std::vector<ObjectType> &objectsVariation, int numLines, int numObjectsPerLine);
    void setLinesState(LineState state);
    void setLineState(Line *line, LineState state);
    void update(float t);
    cocos2d::Sprite *getBackground() const {
        return background;
    }
    inline Line *getLine(int i) {
        return lines[i];
    }
    inline size_t getLinesCount() const {
        return lines.size();
    }
    inline PlayerSpawnData getPlayerSpawnData() {
        return playerSpawnData;
    }
private:
    cocos2d::Sprite *background;

    // Line positions
    std::vector<Line *> lines;

    // Spawn position
    PlayerSpawnData playerSpawnData;
};


#endif //PROJ_ANDROID_LEVEL_H
