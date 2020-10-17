//
// Created by Vlad on 03.09.2020.
//

#ifndef PROJ_ANDROID_STARTMENU_H
#define PROJ_ANDROID_STARTMENU_H

#include "cocos2d.h"
#include "Button.h"
#include "../firebase_cpp_sdk/include/firebase/admob.h"

enum class StartMenuState { NONE, START, EXIT, ABOUT };

struct StartMenu : public cocos2d::Scene {
    static StartMenu *create(const std::function<void(StartMenu *)> &func);
  bool init(const std::function<void(StartMenu *)> &func);
  inline StartMenuState getState() { return currentState; }
  void update(float t);
  void setShouldShowAd(bool flag);
private:
    void tryLoadAd();
  void tryShowAd();

 private:
  cocos2d::Sprite *background;
  Button *start;
  Button *exit;

  StartMenuState currentState;

  std::function<void(StartMenu *)> mainFunc;

  firebase::admob::InterstitialAd *interstitialAd;
  firebase::admob::AdRequest adRequest;

  bool shouldShowAds;
  bool isAdLoaded;
};

#endif  // PROJ_ANDROID_STARTMENU_H
