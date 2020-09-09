//
// Created by Vlad on 03.09.2020.
//

// TODO: save current world somewhere
// TODO: destroy not visible tiles

#ifndef PROJ_ANDROID_WORLDMANAGER_H
#define PROJ_ANDROID_WORLDMANAGER_H

#include "cocos2d.h"

const int TERRAIN_WIDTH = 2;
const int TERRAIN_HEIGHT = 20;

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
    void update(const cocos2d::Vec2 &position);
    inline WorldState getState() const {
        return currentWorldState;
    }
    inline cocos2d::Sprite *getBackground() const {
        return background;
    }
private:
    Block generateBlock(BlockType type, float x, float y, float scale);
    std::string getBlockSource(BlockType type) const;
private:
    std::list<Block> blocks;
    // Block blocks[TERRAIN_HEIGHT * TERRAIN_WIDTH];
    cocos2d::Sprite *background;
    WorldState currentWorldState;

    // Block constants
    float BLOCK_SCALE = 4;
    float BLOCK_OFFSET = BLOCK_SCALE * 4;
    float BLOCK_OFFSET_Y = -TERRAIN_WIDTH * TERRAIN_HEIGHT * 1.6f;
    int CHUNK_SIZE = 32; // When player triggers terrain generation, generates chunk of 16 blocks

    // Distance from player to block position, at which terrain gen starting
    const float D = BLOCK_OFFSET * CHUNK_SIZE * 0.8f;
};


#endif //PROJ_ANDROID_WORLDMANAGER_H
