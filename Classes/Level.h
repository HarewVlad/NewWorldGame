//
// Created by Vlad on 12.09.2020.
//

#ifndef PROJ_ANDROID_LEVEL_H
#define PROJ_ANDROID_LEVEL_H

#include "cocos2d.h"
#include "Line.h"


struct PlayerSpawnData {
    cocos2d::Vec2 position;
    int lineIndex;
};

// TODO: store Lines in array, determine lines positions automatically
struct Level : public cocos2d::Node {
    void init(const std::vector<ObjectType> &objectsVariation, int numLines, int numObjectsPerLine, float speed);
    void setLinesState(LineState state);
    void setLineState(Line *line, LineState state);
    void update(float t);
    void reload();
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
