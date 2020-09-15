//
// Created by Vlad on 03.09.2020.
//

#include "LevelManager.h"

bool LevelManager::init() {
    this->currentLevel = 1;

    return true;
}

void LevelManager::addLevel(int i, Level *level) {
    levels[i] = level;

    this->addChild(level);
}

void LevelManager::update(float t) {
    levels[currentLevel]->update(t);
}

void LevelManager::startCurrentLevel() {
    levels[currentLevel]->setLinesState(LineState::RUNNING);
}

void LevelManager::pauseCurrentLevel() {
    levels[currentLevel]->setLinesState(LineState::IDLE);
}