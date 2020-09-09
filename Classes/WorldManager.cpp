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
        float x = origin.x + i * offsetBlock;
        for (int j = 0; j < TERRAIN_WIDTH; ++j) {
            float y = origin.y + offsetY + j * offsetBlock;

            blocks[i * TERRAIN_WIDTH + j] = j == TERRAIN_WIDTH - 1 ? generateBlock(BlockType::GRASS, x, y, scale) :
                    generateBlock(BlockType::DIRT, x, y, scale);
        }
    }

    // Add blocks to node
    for (int i = 0; i < TERRAIN_HEIGHT; ++i) {
        for (int j = 0; j < TERRAIN_WIDTH; ++j) {
            this->addChild(blocks[i * TERRAIN_WIDTH + j] .sprite, 1);
        }
    }
}

void WorldManager::update(const cocos2d::Vec2 &position) {
    if (position.x + D > blocks[TERRAIN_HEIGHT - 1].position.x) {
        // Remove leftmost blocks
//        for (int j = 0; j < TERRAIN_WIDTH; ++j) {
//            this->removeChild(blocks[j].sprite, true);
//        }

        // Move blocks to left by 1 column
        // memcpy(&blocks[0], &blocks[TERRAIN_WIDTH], (TERRAIN_WIDTH * TERRAIN_HEIGHT - TERRAIN_WIDTH * 2) * sizeof(Block)); // First one and last one we don't copy

        // Generate new blocks
        auto origin = cocos2d::Director::getInstance()->getVisibleOrigin();

        int i = TERRAIN_HEIGHT - 1;
        float x = blocks[i * TERRAIN_WIDTH].position.x + offsetBlock;
        for (int j = 0; j < TERRAIN_WIDTH; ++j) {
            float y = origin.y + offsetY + j * offsetBlock;

            blocks[i * TERRAIN_WIDTH + j] = j == TERRAIN_WIDTH - 1 ? generateBlock(BlockType::GRASS, x, y, scale) :
                                            generateBlock(BlockType::DIRT, x, y, scale);
        }

        // Fill new rightmost blocks
        for (int j = 0; j < TERRAIN_WIDTH; ++j) {
            this->addChild(blocks[i * TERRAIN_WIDTH + j].sprite, 1);
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