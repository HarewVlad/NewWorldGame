//
// Created by Vlad on 24.09.2020.
//

#pragma once

#include "cocos2d.h"
#include "ui/CocosGUI.h"

class ButtonState {
public:
    static const int IDLE = 1;
    static const int PUSHED = 2;
    static const int DRAGOUT = 3;
};

class Button : public cocos2d::Node {
public:
    static Button* create(const std::string &filename, const cocos2d::Color3B &color, const std::function<void(Ref *)> &func);

    bool init(const std::string &filename, const cocos2d::Color3B &color, const std::function<void(Ref *)> &func);
    inline constexpr int getCurrentState() {
        return m_CurrentState;
    }
    void setText(const std::string &text, const cocos2d::Color3B &color);
    void setEnabled(bool flag);
private:
    bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event);
    void onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *event);
    void onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *event);
    void onTouchCancelled(cocos2d::Touch *touch, cocos2d::Event *event);
    cocos2d::Rect getButtonBoundingBox() const;
    cocos2d::Rect getExpandZone() const;
    cocos2d::Rect getSafeZone() const;
protected:
    virtual void setIdleState();
    virtual void setDragoutState();
    virtual void setPushedState();
protected:
    std::function<void (Ref *)> mainFunc;

    cocos2d::EventListenerTouchOneByOne *touchEvent;
    cocos2d::Sprite *m_pMainSprite;
    cocos2d::Sprite *m_pBaseSprite;
    cocos2d::Sprite *m_pShadowSprite;

    cocos2d::Label *m_pButtonText;

    int m_CurrentState;

    // Constants
    const float EPSILON = 0.5f;
    const float EXPAND_ZONE_OFFSET = 50.0f;
    const float SAFE_ZONE_OFFSET = 100.0f;
    const float OFFSET_Y = 16.0f;
    const float BASE_COLOR_MUL = 0.7f;
    const float ANIMATION_SPEED = 0.05f;

    bool m_isEnabled;
};