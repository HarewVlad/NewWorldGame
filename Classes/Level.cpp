//
// Created by Vlad on 12.09.2020.
//

#include "Level.h"

void Level::init(const std::vector<ObjectType> &objectsVariation, int numLines, int numObjectsPerLine) {
    // Init lines
    auto origin = cocos2d::Director::getInstance()->getVisibleOrigin();
    auto size = cocos2d::Director::getInstance()->getVisibleSize();

    float x = origin.x + size.width / (numLines + 1);
    for (int i = 0; i < numLines; ++i) {
        Line *line = new Line();
        line->init({x, origin.y + size.height + size.height / 4.0f},
                objectsVariation, numObjectsPerLine);
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

Object Line::createObject(ObjectType type, float scale) {
    cocos2d::Sprite *object = cocos2d::Sprite::create(getObjectSource(type));
    cocos2d::PhysicsBody *body = cocos2d::PhysicsBody::createBox(object->getContentSize(),
                                                                 cocos2d::PhysicsMaterial(0.0f, 0.0f, 1.0f));
    body->setDynamic(false);

    object->setScale(scale);
    object->setPosition(position);
    object->setPhysicsBody(body);
    return Object {type, object };
}

std::string Line::getObjectSource(ObjectType type) const {
    switch (type) {
        case ObjectType::CAR:
            return "box1.png";
        case ObjectType::UFO:
            return "";
        default:
            throw std::runtime_error("no such object");
    }
}

void Level::update(float t) {
    for (auto line : lines) {
        line->update(t);
    }
}

void Line::init(const cocos2d::Vec2 &position, const std::vector<ObjectType> &objectsVariation, int numObjectsPerLine) {
    // Set position
    this->position = position;
    // Set initial delay
    time -= static_cast<float>(random()) / static_cast<float>(RAND_MAX / MAX_DELAY);
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
            objects.push_back(createObject(objectType, 1.0f));
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
            auto move = cocos2d::MoveBy::create(t, {0, -0.5f});
            object.sprite->runAction(move);
        }

        if (lastRenderingObject == objects.size()
            && renderingObjects[lastRenderingObject - 1].sprite->getPositionY() <= 0) {
            currentState = LineState::FINISHED;
        }
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