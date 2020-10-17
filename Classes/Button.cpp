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
    m_pMainSprite = cocos2d::Sprite::create(filename);
    m_pMainSprite->setColor(color);
    m_pMainSprite->setPositionY(OFFSET_Y);

    m_pButtonText = cocos2d::Label::createWithTTF("", "fonts/ThaleahFat.ttf", 36);
    m_pButtonText->setPosition(m_pMainSprite->getContentSize() / 2.0f);
    m_pMainSprite->addChild(m_pButtonText);

    m_pBaseSprite = cocos2d::Sprite::createWithTexture(m_pMainSprite->getTexture());
    float  newR = m_pMainSprite->getColor().r * (BASE_COLOR_MUL);
    float  newG = m_pMainSprite->getColor().g * (BASE_COLOR_MUL);
    float  newB = m_pMainSprite->getColor().b * (BASE_COLOR_MUL);
    m_pBaseSprite->setColor(cocos2d::Color3B(newR, newG, newB));
    
    m_pShadowSprite = cocos2d::Sprite::createWithTexture(m_pMainSprite->getTexture());
    m_pShadowSprite->setOpacity(50);
    m_pShadowSprite->setColor(cocos2d::Color3B::BLACK);
    m_pShadowSprite->setPositionY(-OFFSET_Y);

    touchEvent = cocos2d::EventListenerTouchOneByOne::create();
    touchEvent->onTouchBegan = CC_CALLBACK_2(Button::onTouchBegan, this);
    touchEvent->onTouchEnded = CC_CALLBACK_2(Button::onTouchEnded, this);
    touchEvent->onTouchMoved = CC_CALLBACK_2(Button::onTouchMoved, this);
    touchEvent->onTouchCancelled = CC_CALLBACK_2(Button::onTouchCancelled, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(touchEvent, this);

    this->m_CurrentState = ButtonState::IDLE;
    this->m_isEnabled = true;
    this->mainFunc = func;

    this->scheduleUpdate();

    this->addChild(m_pShadowSprite, 0);
    this->addChild(m_pBaseSprite, 1);
    this->addChild(m_pMainSprite, 2);

    return true;
}

void Button::setIdleState() {
    if (m_CurrentState != ButtonState::IDLE) {
      if (m_CurrentState != ButtonState::DRAGOUT) {
        m_pMainSprite->stopAllActions();
        m_pMainSprite->runAction(cocos2d::JumpTo::create(ANIMATION_SPEED * 2.5f, { 0, OFFSET_Y }, OFFSET_Y, 1));
        }
        

        m_CurrentState = ButtonState::IDLE;
    }
}

void Button::setDragoutState() {
    if (m_CurrentState != ButtonState::DRAGOUT) {
        m_pMainSprite->stopAllActions();
        m_pMainSprite->runAction(cocos2d::JumpTo::create(ANIMATION_SPEED * 2.5f, { 0, OFFSET_Y }, OFFSET_Y, 1));

        m_CurrentState = ButtonState::DRAGOUT;
    }
}

void Button::setPushedState() {
    if (m_CurrentState != ButtonState::PUSHED) {
        m_pMainSprite->stopAllActions();
        m_pMainSprite->runAction(cocos2d::MoveTo::create(ANIMATION_SPEED, { 0, OFFSET_Y / 3.0f }));

        m_CurrentState = ButtonState::PUSHED;
    }
}

bool Button::onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event) {
    if (m_isEnabled && getButtonBoundingBox().containsPoint(touch->getLocation())) {
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
    if (m_CurrentState != ButtonState::IDLE) {
        if (touch->getPreviousLocation().distance(touch->getLocation()) > EPSILON) {
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
            this->getPositionX() + m_pMainSprite->getPositionX() - m_pMainSprite->getBoundingBox().size.width * 0.5f,
            this->getPositionY() + m_pMainSprite->getPositionY() - m_pMainSprite->getBoundingBox().size.height * 0.5f,
            m_pMainSprite->getBoundingBox().size.width,
            m_pMainSprite->getBoundingBox().size.height);
}

cocos2d::Rect Button::getExpandZone() const {
    cocos2d::Rect contentSize = getButtonBoundingBox();
    return cocos2d::Rect(contentSize.getMinX() - EXPAND_ZONE_OFFSET,
                         contentSize.getMinY() - EXPAND_ZONE_OFFSET,
                         contentSize.size.width + EXPAND_ZONE_OFFSET * 2.0f,
                         contentSize.size.height + EXPAND_ZONE_OFFSET * 2.0f);
}

cocos2d::Rect Button::getSafeZone() const {
    cocos2d::Rect contentSize = getButtonBoundingBox();
    return cocos2d::Rect(contentSize.getMinX() - SAFE_ZONE_OFFSET,
            contentSize.getMinY() - SAFE_ZONE_OFFSET,
            contentSize.size.width + SAFE_ZONE_OFFSET * 2.0f,
            contentSize.size.height + SAFE_ZONE_OFFSET * 2.0f);
}

void Button::setText(const std::string &text, const cocos2d::Color3B &color) {
  m_pButtonText->setString(text);
  m_pButtonText->setTextColor(cocos2d::Color4B(color));
}

void Button::setEnabled(bool flag) {
  this->m_isEnabled = flag;
}