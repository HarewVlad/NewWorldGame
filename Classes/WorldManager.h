//
// Created by Vlad on 03.09.2020.
//

#ifndef PROJ_ANDROID_WORLDMANAGER_H
#define PROJ_ANDROID_WORLDMANAGER_H

#include "cocos2d.h"

const int TERRAIN_WIDTH = 16;
const int TERRAIN_HEIGHT = 16;

enum class WorldState {
    NONE
};

enum class BlockType {
    DIRT,
    GRASS
};

struct Block {
    BlockType type;
    cocos2d::Sprite *sprite;
    cocos2d::Vec2 position;
};

struct WorldManager : public cocos2d::Node {
    void init(const std::string &filename);
    void initTerrain(const std::string &grassFileName, const std::string &dirtFileName);
    inline WorldState getState() {
        return currentWorldState;
    }
    inline cocos2d::Sprite *getBackground() {
        return background;
    }
private:
    Block blocks[TERRAIN_HEIGHT][TERRAIN_WIDTH];
    cocos2d::Sprite *background;
    WorldState currentWorldState;
};


#endif //PROJ_ANDROID_WORLDMANAGER_H
