#ifndef _MAIN_MENU_SCENE_H_
#define _MAIN_MENU_SCENE_H_

#include "cocos2d.h"

class MainMenuLayer : public cocos2d::CCLayerColor  {
public:
	MainMenuLayer() : max_points_label(NULL) {};
  virtual ~MainMenuLayer();
  bool init();
  CREATE_FUNC(MainMenuLayer);

  virtual void menuExitCallback(CCObject* pSender);
  virtual void menuStartGameCallback(CCObject* pSender);

  CC_SYNTHESIZE_READONLY(cocos2d::CCLabelTTF*, max_points_label, Label);
};

class MainMenuScene : public cocos2d::CCScene {
public:
	MainMenuScene() : _layer(NULL) {};
  ~MainMenuScene();
  bool init();
  CREATE_FUNC(MainMenuScene);
  
  CC_SYNTHESIZE_READONLY(MainMenuLayer*, _layer, Layer);
};

#endif // _MAIN_MENU_SCENE_H_
