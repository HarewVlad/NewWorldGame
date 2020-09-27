//
// Created by Vlad on 16.09.2020.
//

#include "Line.h"

void Line::init(const std::vector<ObjectType> &objectsVariation, int numObjectsPerLine, float speed) {
    this->speed = speed;
    // Set initial delay
    delay = static_cast<float>(random()) / static_cast<float>(RAND_MAX / MAX_DELAY);
    time -= delay;
    // Set spawn frequency
    frequency = MIN_SPAWN_FREQUENCY + static_cast<float>(random()) / static_cast<float>(RAND_MAX / (MAX_SPAWN_FREQUENCY - MIN_SPAWN_FREQUENCY));
    // Set current object
    lastRenderingObject = 0;
    // Set line state
    currentState = LineState::IDLE;

    int temp = numObjectsPerLine / objectsVariation.size();
    int tempReminder = numObjectsPerLine % objectsVariation.size();

    for (auto objectType : objectsVariation) {
        for (int j = 0; j < temp; ++j) {
            objects.push_back(createObject(objectType, 3.0f));
        }
    }

    for (int i = 0; i < tempReminder; ++i) {
        int reminderDistributionIndex = random() % objectsVariation.size();
        objects.push_back(createObject(objectsVariation[reminderDistributionIndex], 1.0f));
    }

    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::shuffle(objects.begin(), objects.end(), std::default_random_engine(seed));

    // Add
    for (auto object : objects) {
        this->addChild(object.sprite);
    }
}

// TODO: mb redesign the code a little bit
void Line::update(float t) {
    if (currentState == LineState::RUNNING) {
        if (lastRenderingObject < objects.size()) {
            if (time >= frequency) {
                renderingObjects.push_back(objects[lastRenderingObject++]);

                time = 0.0f;
            } else {
                time += t;
            }
        }

        for (auto object : renderingObjects) {
            auto move = cocos2d::MoveBy::create(t, {0, -speed});
            auto rotate = cocos2d::RotateBy::create(t, speed);
            auto seq = cocos2d::Sequence::create(move, rotate, nullptr);
            object.sprite->runAction(seq);
        }

        if (lastRenderingObject == objects.size()
            && renderingObjects[lastRenderingObject - 1].sprite->getPositionY() <= 0) {
            currentState = LineState::FINISHED;
        }
    }
}

Object Line::createObject(ObjectType type, float scale) {
    cocos2d::Sprite *object = cocos2d::Sprite::create(getObjectSource(type));
    cocos2d::PhysicsBody *body = cocos2d::PhysicsBody::createBox(object->getContentSize(),
                                                                 cocos2d::PhysicsMaterial(0.0f, 0.0f, 1.0f));
    body->setDynamic(false);
    body->setContactTestBitmask(0xFFFFFFFF);

    object->setScale(scale);
    object->setPhysicsBody(body);
    return Object {type, object };
}

std::string Line::getObjectSource(ObjectType type) const {
    switch (type) {
        case ObjectType::BEER:
            return "Beer.png";
        case ObjectType::FISH:
            return "Fish.png";
        default:
            throw std::runtime_error("no such object");
    }
}

void Line::reload() {
    // Set objects initial position
    for (auto object : objects) {
        object.sprite->setPosition(this->getPosition());
    }

    // Clear
    renderingObjects.clear();
    time = 0.0f - delay;
    lastRenderingObject = 0;

    // Change state
    currentState = LineState::IDLE;
}
