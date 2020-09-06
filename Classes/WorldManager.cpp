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
    auto size = cocos2d::Director::getInstance()->getVisibleSize();

    // Generate terrain
    float scale = 8;
    float offsetBlock = scale * 4;
    float offsetY = -TERRAIN_WIDTH * TERRAIN_HEIGHT * 1.6f;
    for (int i = 0; i < TERRAIN_HEIGHT; ++i) {
        for (int j = 0; j < TERRAIN_WIDTH; ++j) {
            if (j == TERRAIN_WIDTH - 1) {
                cocos2d::Sprite *grassBlock = cocos2d::Sprite::create(grassFileName);
                cocos2d::PhysicsBody *body = cocos2d::PhysicsBody::createBox(grassBlock->getContentSize(),
                                                                             cocos2d::PhysicsMaterial(0.0f, 0.0f, 1.0f));
                body->setDynamic(false);

                grassBlock->setScale(scale);
                grassBlock->setPosition(cocos2d::Vec2{origin.x + i * offsetBlock, origin.y + offsetY + j * offsetBlock});
                grassBlock->setPhysicsBody(body);
                blocks[i][j] = Block {BlockType::GRASS, grassBlock, grassBlock->getPosition()};
            } else {
                cocos2d::Sprite *dirtBlock = cocos2d::Sprite::create(dirtFileName);
                cocos2d::PhysicsBody *body = cocos2d::PhysicsBody::createBox(dirtBlock->getContentSize(),
                                                                             cocos2d::PhysicsMaterial(0.0f, 0.0f, 1.0f));
                body->setDynamic(false);

                dirtBlock->setScale(scale);
                dirtBlock->setPosition(cocos2d::Vec2{origin.x + i * offsetBlock, origin.y + offsetY + j * offsetBlock});
                dirtBlock->setPhysicsBody(body);
                blocks[i][j] = Block {BlockType::DIRT, dirtBlock, dirtBlock->getPosition()};
            }
        }
    }

    // Add blocks to node
    for (int i = 0; i < TERRAIN_HEIGHT; ++i) {
        for (int j = 0; j < TERRAIN_WIDTH; ++j) {
            this->addChild(blocks[i][j].sprite, 1, i * TERRAIN_WIDTH + j);
        }
    }
}