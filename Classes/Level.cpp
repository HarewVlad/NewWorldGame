//
// Created by Vlad on 12.09.2020.
//

#include "Level.h"

void Level::init(const std::vector<ObjectType> &objectsVariation, int numLines, int numObjectsPerLine, float speed) {
    // Init lines
    auto origin = cocos2d::Director::getInstance()->getVisibleOrigin();
    auto size = cocos2d::Director::getInstance()->getVisibleSize();

    float x = origin.x + size.width / (numLines + 1);
    for (int i = 0; i < numLines; ++i) {
        Line *line = new Line();
        line->init({x, origin.y + size.height + size.height / 4.0f},
                objectsVariation, numObjectsPerLine, speed);
        lines.push_back(line);

        x += size.width / (numLines + 1);
    }

    // Add
    for (auto line : lines) {
        this->addChild(line);
    }

    // Set player spawn data
    int playerSpawnLineIndex = random() % lines.size();
    auto playerSpawnPosition = lines[playerSpawnLineIndex]->getPosition();
    playerSpawnPosition.y -= size.height;

    playerSpawnData = {playerSpawnPosition, playerSpawnLineIndex};
}

void Level::update(float t) {
    for (auto line : lines) {
        line->update(t);
    }
}

void Level::setLineState(Line *line, LineState state) {
    line->setState(state);
}

void Level::setLinesState(LineState state) {
    for (auto line : lines) {
        line->setState(state);
    }
}

void Level::reload() {
    for (auto line : lines) {
        line->reload();
    }
}
