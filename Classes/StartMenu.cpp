//
// Created by Vlad on 03.09.2020.
//

#include "StartMenu.h"

static firebase::admob::AdParent getAdParent() {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
  // Returns the iOS RootViewController's main view (i.e. the EAGLView).
  return (id)Director::getInstance()->getOpenGLView()->getEAGLView();
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
  // Returns the Android Activity.
  return cocos2d::JniHelper::getActivity();
#else
  // A void* for any other environments.
  return 0;
#endif
}

void StartMenu::setShouldShowAd(bool flag) {
  shouldShowAds = flag;
}

bool StartMenu::init(const std::function<void(StartMenu *)> &func) {
  if (!Scene::init()) {
    return false;
  }

  this->mainFunc = func;
  this->currentState = StartMenuState::NONE;

  // Orientation
  auto origin = cocos2d::Director::getInstance()->getVisibleOrigin();
  auto size = cocos2d::Director::getInstance()->getVisibleSize();

  // Create background
  background = cocos2d::Sprite::create("StartMenu/sumk1Y.jpg");
  background->setScaleX(size.width / background->getContentSize().width);
  background->setScaleY(size.height / background->getContentSize().height);
  background->setPosition(origin + size * 0.5f);

  // Create menu
  auto startButton = Button::create("Buttons/Rect.png", cocos2d::Color3B::GRAY, [this](cocos2d::Ref *sender) {
    currentState = StartMenuState::START;

    if (mainFunc != nullptr) {
      mainFunc(this);
    }
  });
  startButton->setText("Start", cocos2d::Color3B::WHITE);
  startButton->setPosition({ origin.x + size.width * 0.5f, origin.y + size.height * 0.6f });
  auto exitButton = Button::create("Buttons/Rect.png", cocos2d::Color3B::GRAY, [this](cocos2d::Ref *sender) {
    currentState = StartMenuState::EXIT;

    if (mainFunc != nullptr) {
      mainFunc(this);
    }
  });
  exitButton->setText("Exit", cocos2d::Color3B::WHITE);
  exitButton->setPosition({ origin.x + size.width * 0.5f, origin.y + size.height * 0.5f });
  auto aboutButton = Button::create("Buttons/Rect.png", cocos2d::Color3B::GRAY, [this](cocos2d::Ref *sender) {
    currentState = StartMenuState::ABOUT;

    if (mainFunc != nullptr) {
      mainFunc(this);
    }
  });
  aboutButton->setText("About", cocos2d::Color3B::WHITE);
  aboutButton->setPosition({ origin.x + size.width * 0.5f, origin.y + size.height * 0.4f });

  // Ads
  {
    interstitialAd = new firebase::admob::InterstitialAd();
    interstitialAd->Initialize(getAdParent(), "XXX");

    adRequest = {};
    adRequest.birthday_day = 1;
    adRequest.birthday_month = 1;
    adRequest.birthday_year = 2015;
    adRequest.gender = firebase::admob::kGenderUnknown;
  }

  this->addChild(background, 0);
  this->addChild(startButton, 1);
  this->addChild(exitButton, 1);
  this->addChild(aboutButton, 1);

  this->scheduleUpdate();

  return true;
}

void StartMenu::update(float t) {
  if (shouldShowAds) {
    if (!isAdLoaded) {
      tryLoadAd();
    } else {
      tryShowAd();
    }
  }
}

void StartMenu::tryLoadAd() {
  if (interstitialAd->InitializeLastResult().status() ==
      firebase::kFutureStatusComplete &&
      interstitialAd->InitializeLastResult().error() ==
      firebase::admob::kAdMobErrorNone) {
    interstitialAd->LoadAd(adRequest);

    isAdLoaded = true;
  }
}
void StartMenu::tryShowAd() {
  if (interstitialAd->LoadAdLastResult().status() ==
      firebase::kFutureStatusComplete &&
      interstitialAd->LoadAdLastResult().error() ==
      firebase::admob::kAdMobErrorNone) {
    interstitialAd->Show();

    isAdLoaded = false;
    shouldShowAds = false;
  }
}