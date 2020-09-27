//
// Created by Vlad on 03.09.2020.
//

// TODO: save current world somewhere
// TODO: destroy not visible tiles

#ifndef PROJ_ANDROID_LEVELMANAGER_H
#define PROJ_ANDROID_LEVELMANAGER_H

#include "Level.h"

struct LevelManager : public cocos2d::Node {
    bool init() override;
    void addLevel(int i, Level *level);
    void update(float t) override;
    void startCurrentLevel();
    void reloadCurrentLevel();
    void pauseCurrentLevel();
    Level *getLevel(int i) {
        return levels[i];
    }
    inline void setCurrentLevelIndex(int i) {
        currentLevel = i;
    }
    inline int getCurrentLevelIndex() const {
        return currentLevel;
    }
    inline Level *getCurrentLevel() {
        return levels[currentLevel];
    }
private:
    std::unordered_map<int, Level *> levels;
    int currentLevel;
};


#endif //PROJ_ANDROID_LEVELMANAGER_H
