//
// Created by Vlad on 03.09.2020.
//

#include "LevelManager.h"

bool LevelManager::init() {
  this->currentLevel = 1;

  return true;
}

void LevelManager::addLevel(int i, Level *level) { levels[i] = level; }

void LevelManager::startCurrentLevel() { levels[currentLevel]->setStart(); }

void LevelManager::pauseCurrentLevel() { levels[currentLevel]->setPause(); }

void LevelManager::resumeCurrentLevel() { levels[currentLevel]->setResume(); }