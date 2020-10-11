#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "Button.h"

class About : public cocos2d::Scene {
public:
  static About* create(const std::function<void(Ref *)> &func);
  bool init(const std::function<void(Ref *)> &func);
private:
  cocos2d::Sprite *background;
  Button *backButton;

  std::function<void(Ref *)> mainFunc;
};