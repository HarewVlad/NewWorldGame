//
// Created by Vlad on 24.09.2020.
//

#include "Button.h"

Button* Button::create(const std::string &filename, const cocos2d::Color3B &color, const std::function<void(Ref *)> &func) {
    Button *node = new (std::nothrow)Button();
    if(node && node->init(filename, color, func))
    {
        node->autorelease();
        return node;
    }

    CC_SAFE_DELETE(node);
    return nullptr;
}

bool Button::init(const std::string &filename, const cocos2d::Color3B &color, const std::function<void(Ref *)> &func) {
    main = cocos2d::Sprite::create(filename);
    main->setColor(color);
    main->setPositionY(OFFSET_Y);

    buttonText = cocos2d::Label::createWithTTF("", "fonts/ThaleahFat.ttf", 36);
    buttonText->setPosition(main->getContentSize() / 2.0f);
    main->addChild(buttonText);

    base = cocos2d::Sprite::createWithTexture(main->getTexture());
    float  newR = main->getColor().r * (BASE_COLOR_MUL);
    float  newG = main->getColor().g * (BASE_COLOR_MUL);
    float  newB = main->getColor().b * (BASE_COLOR_MUL);
    base->setColor(cocos2d::Color3B(newR, newG, newB));
    
    shadow = cocos2d::Sprite::createWithTexture(main->getTexture());
    shadow->setOpacity(50);
    shadow->setColor(cocos2d::Color3B::BLACK);
    shadow->setPositionY(-OFFSET_Y);

    touchEvent = cocos2d::EventListenerTouchOneByOne::create();
    touchEvent->onTouchBegan = CC_CALLBACK_2(Button::onTouchBegan, this);
    touchEvent->onTouchEnded = CC_CALLBACK_2(Button::onTouchEnded, this);
    touchEvent->onTouchMoved = CC_CALLBACK_2(Button::onTouchMoved, this);
    touchEvent->onTouchCancelled = CC_CALLBACK_2(Button::onTouchCancelled, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(touchEvent, this);

    this->currentState = ButtonState::IDLE;
    this->isEnabled = true;
    this->mainFunc = func;

    this->scheduleUpdate();

    this->addChild(shadow, 0);
    this->addChild(base, 1);
    this->addChild(main, 2);

    return true;
}

void Button::setIdleState() {
    if (currentState != ButtonState::IDLE) {
      if (currentState != ButtonState::DRAGOUT) {
        main->stopAllActions();
        main->runAction(cocos2d::JumpTo::create(ANIMATION_SPEED * 2.5f, { 0, OFFSET_Y }, OFFSET_Y, 1));
        }
        

        currentState = ButtonState::IDLE;
    }
}

void Button::setDragoutState() {
    if (currentState != ButtonState::DRAGOUT) {
        main->stopAllActions();
        main->runAction(cocos2d::JumpTo::create(ANIMATION_SPEED * 2.5f, { 0, OFFSET_Y }, OFFSET_Y, 1));

        currentState = ButtonState::DRAGOUT;
    }
}

void Button::setPushedState() {
    if (currentState != ButtonState::PUSHED) {
        main->stopAllActions();
        main->runAction(cocos2d::MoveTo::create(ANIMATION_SPEED, { 0, OFFSET_Y / 3.0f }));

        currentState = ButtonState::PUSHED;
    }
}

bool Button::onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event) {
    if (isEnabled && getButtonBoundingBox().containsPoint(touch->getLocation())) {
        setPushedState();

        if (mainFunc != nullptr) {
          mainFunc(this);
        }
    }

    return true;
}

void Button::onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *event) {
    setIdleState();
}

void Button::onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *event) {
    if (currentState != ButtonState::IDLE) {
        if (touch->getPreviousLocation().distance(touch->getLocation()) > epsilon) {
            if (getButtonBoundingBox().containsPoint(touch->getLocation())) {
                setPushedState();
            } else if (getExpandZone().containsPoint(touch->getLocation())) {
                setPushedState();
            } else if (getSafeZone().containsPoint(touch->getLocation())) {
                setDragoutState();
            } else {
                setIdleState();
            }
        }
    }
}

void Button::onTouchCancelled(cocos2d::Touch *touch, cocos2d::Event *event) {
    setIdleState();
}

cocos2d::Rect Button::getButtonBoundingBox() const {
    return cocos2d::Rect(
            this->getPositionX() + main->getPositionX() - main->getBoundingBox().size.width * 0.5f,
            this->getPositionY() + main->getPositionY() - main->getBoundingBox().size.height * 0.5f,
            main->getBoundingBox().size.width,
            main->getBoundingBox().size.height);
}

cocos2d::Rect Button::getExpandZone() const {
    cocos2d::Rect contentSize = getButtonBoundingBox();
    return cocos2d::Rect(contentSize.getMinX() - expandZoneOffset,
                         contentSize.getMinY() - expandZoneOffset,
                         contentSize.size.width + expandZoneOffset * 2.0f,
                         contentSize.size.height + expandZoneOffset * 2.0f);
}

cocos2d::Rect Button::getSafeZone() const {
    cocos2d::Rect contentSize = getButtonBoundingBox();
    return cocos2d::Rect(contentSize.getMinX() - safeZoneOffset,
            contentSize.getMinY() - safeZoneOffset,
            contentSize.size.width + safeZoneOffset * 2.0f,
            contentSize.size.height + safeZoneOffset * 2.0f);
}

void Button::setText(const std::string &text, const cocos2d::Color3B &color) {
  buttonText->setString(text);
  buttonText->setTextColor(cocos2d::Color4B(color));
}

void Button::setEnabled(bool flag) {
  this->isEnabled = flag;
}