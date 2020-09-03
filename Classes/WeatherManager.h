//
// Created by Vlad on 03.09.2020.
//

#ifndef PROJ_ANDROID_WEATHERMANAGER_H
#define PROJ_ANDROID_WEATHERMANAGER_H

#include "cocos2d.h"

enum class WeatherType {
    NONE,
    RAIN,
    SNOW
};

struct WeatherManager : public cocos2d::Node {
    void addWeatherEffect(WeatherType type);
    void runWeatherEffects();
    void stopWeatherEffects();
    inline int getType() {
        return currentWeather;
    }
private:
    int currentWeather; // Can have multiple weather effects at once (RAIN | STROM ...)
};


#endif //PROJ_ANDROID_WEATHERMANAGER_H
