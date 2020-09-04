//
// Created by Vlad on 03.09.2020.
//

#include "WeatherManager.h"

void WeatherManager::addWeatherEffect(WeatherType type) {
    // NOTE: can have multiple weather effects at one
    currentWeather |= static_cast<int>(type);
}

void WeatherManager::runWeatherEffects() {
    if (currentWeather & static_cast<int>(WeatherType::RAIN)) {
        auto emitter = cocos2d::ParticleRain::create();
        //emitter->setStartRadius(100.0f);
        //emitter->setEndRadius(0);
        //emitter->setEndRadius(cocos2d::ParticleSystem::START_RADIUS_EQUAL_TO_END_RADIUS);
        //emitter->setEndRadiusVar(0);

        this->addChild(emitter, static_cast<int>(WeatherType::RAIN));
    }

    if (currentWeather & static_cast<int>(WeatherType::SNOW)) {
        auto emitter = cocos2d::ParticleSnow::create();

        this->addChild(emitter, static_cast<int>(WeatherType::SNOW));
    }
}
void WeatherManager::stopAllEffects() {
    this->removeFromParentAndCleanup(true);
}

void WeatherManager::stopEffect(WeatherType type) {
    this->removeChild(this->getChildByTag(static_cast<int>(type)), true);
}

bool WeatherManager::haveEffect(WeatherType type) {
    return currentWeather & static_cast<int>(type);
}