//
// Created by Vlad on 03.09.2020.
//

#include "WorldManager.h"

void WorldManager::init(const std::string &filename) {
    // Orientation
    auto origin = cocos2d::Director::getInstance()->getVisibleOrigin();
    auto size = cocos2d::Director::getInstance()->getVisibleSize();

    // Create background
    background = cocos2d::Sprite::create(filename);
    background->setScaleX(size.width / background->getContentSize().width);
    background->setPosition(origin + size * 0.5f);

    // Add components to the node
    this->addChild(background, 0);
}

void WorldManager::initTerrain(const std::string &grassFileName, const std::string &dirtFileName) {
    // Orientation
    auto origin = cocos2d::Director::getInstance()->getVisibleOrigin();

    // Generate terrain
    for (int i = 0; i < TERRAIN_HEIGHT; ++i) {
        float x = origin.x + i * BLOCK_OFFSET;
        for (int j = 0; j < TERRAIN_WIDTH; ++j) {
            float y = origin.y + BLOCK_OFFSET_Y + j * BLOCK_OFFSET;

            Block block = j == TERRAIN_WIDTH - 1 ? generateBlock(BlockType::GRASS, x, y, BLOCK_SCALE) :
                          generateBlock(BlockType::DIRT, x, y, BLOCK_SCALE);
            blocks.push_back(block);
        }
    }

    // Add blocks to the node
    for (auto b : blocks) {
        this->addChild(b.sprite, 1);
    }
}

void WorldManager::update(const cocos2d::Vec2 &position) {
    if (position.x + D > blocks.back().position.x) {
        auto origin = cocos2d::Director::getInstance()->getVisibleOrigin();

        for (int i = 0; i < CHUNK_SIZE; ++i)
        {
            float x = blocks.back().position.x + BLOCK_OFFSET;
            for (int j = 0; j < TERRAIN_WIDTH; ++j) {
                float y = origin.y + BLOCK_OFFSET_Y + j * BLOCK_OFFSET;

                Block block = j == TERRAIN_WIDTH - 1 ? generateBlock(BlockType::GRASS, x, y, BLOCK_SCALE) :
                              generateBlock(BlockType::DIRT, x, y, BLOCK_SCALE);
                blocks.push_back(block);
                this->addChild(block.sprite, 1);
            }
        }
    } else if (position.x - D < blocks.front().position.x) {
        auto origin = cocos2d::Director::getInstance()->getVisibleOrigin();

        for (int i = 0; i < CHUNK_SIZE; ++i)
        {
            float x = blocks.front().position.x - BLOCK_OFFSET;
            for (int j = 0; j < TERRAIN_WIDTH; ++j) {
                float y = origin.y + BLOCK_OFFSET_Y + j * BLOCK_OFFSET;

                Block block = j == TERRAIN_WIDTH - 1 ? generateBlock(BlockType::GRASS, x, y, BLOCK_SCALE) :
                              generateBlock(BlockType::DIRT, x, y, BLOCK_SCALE);
                blocks.push_front(block);
                this->addChild(block.sprite, 1);
            }
        }
    }
}

Block WorldManager::generateBlock(BlockType type, float x, float y, float scale) {
    cocos2d::Sprite *block = cocos2d::Sprite::create(getBlockSource(type));
    cocos2d::PhysicsBody *body = cocos2d::PhysicsBody::createBox(block->getContentSize(),
                                                                 cocos2d::PhysicsMaterial(0.0f, 0.0f, 1.0f));
    body->setDynamic(false);

    block->setScale(scale);
    block->setPosition(cocos2d::Vec2{x, y});
    block->setPhysicsBody(body);
    return Block {type, block, block->getPosition()};
}

std::string WorldManager::getBlockSource(BlockType type) const {
    switch (type) {
        case BlockType::GRASS:
            return "grassBlock.png";
        case BlockType::DIRT:
            return "dirtBlock.png";
        default:
            throw std::runtime_error("block type doesn't exist");
    }
}