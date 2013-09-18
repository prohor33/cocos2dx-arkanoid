#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"

#include "SimpleAudioEngine.h"

class HelloWorld : public cocos2d::CCLayerColor
{
public:
	HelloWorld();
	~HelloWorld();

	// Here's a difference. Method 'init' in cocos2d-x returns bool, 
    // instead of returning 'id' in cocos2d-iphone
	virtual bool init();  

	// there's no 'id' in cpp, so we recommand to return the exactly class pointer
	static cocos2d::CCScene* scene();

	// a selector callback
	virtual void menuCloseCallback(cocos2d::CCObject* pSender);

	// implement the "static node()" method manually
	CREATE_FUNC(HelloWorld);

	void spriteMoveFinished(cocos2d::CCNode* sender);

	void gameLogic(float dt);

	void updateGame(float dt);

	void registerWithTouchDispatcher();
	void ccTouchesBegan(cocos2d::CCSet* touches, cocos2d::CCEvent* event);
	void ccTouchesMoved(cocos2d::CCSet* touches, cocos2d::CCEvent* event);
	void ccTouchesCancelled(cocos2d::CCSet* touches, cocos2d::CCEvent* event);
	void ccTouchesEnded(cocos2d::CCSet* touches, cocos2d::CCEvent* event);
	void bogieMoveFinished(cocos2d::CCNode* sender);
	void InitializeBoxes();
	void CreateBox( cocos2d::CCPoint p );

	static int max_points_result;
    
protected:
	cocos2d::CCArray *_targets;
	cocos2d::CCArray *_projectiles;
	cocos2d::CCArray *_boxes;
	int _projectilesDestroyed;
	int _boxesDestroyed;

	cocos2d::CCSprite *_ball;
	cocos2d::CCSprite* _bogie;

	float ball_velocity;
	float bogie_velocity;
	const int touch_size;
	bool bogie_is_drugging;
	int boxes_quant;

	// for ccTouchesMoved() event
	cocos2d::CCPoint last_touches_moved_p;
	double last_touches_moved_s;

};

#endif  // __HELLOWORLD_SCENE_H__
