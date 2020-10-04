#include "cocos2d.h"

class Heart : public cocos2d::Node {
public:
  bool init();

  inline cocos2d::Size getHeartSize() const {
    return heart->getContentSize();
  }
private:
  cocos2d::Sprite *heart;
  cocos2d::Sprite *border;
  cocos2d::Sprite *background;
};