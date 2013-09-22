#include "HelloWorldScene.h"
#include "GameOverScene.h"
#include "MainMenuScene.h"
#include "SimpleAudioEngine.h"

using namespace cocos2d;

int HelloWorld::max_points_result = 0;

HelloWorld::~HelloWorld() {
  // release ball velocity
  CCPoint* ball_vel = static_cast<CCPoint*>(_ball->getUserData());
  if( ball_vel ) {
    delete ball_vel;
  }

  // release bogie velocity
  CCPoint* bogie_vel = static_cast<CCPoint*>(_bogie->getUserData());
  if( bogie_vel ) {
    delete bogie_vel;
  }

  if (_targets) {
		_targets->release();
		_targets = NULL;
	}

	if (_projectiles) {
		_projectiles->release();
		_projectiles = NULL;
	}

	// cpp don't need to call super dealloc
	// virtual destructor will do this
}

HelloWorld::HelloWorld() :
_boxes(NULL)
,_boxesDestroyed(0)
,ball_velocity(150/1)
,bogie_velocity(450/1) // 150pixels/10sec
,bogie_is_drugging(false)
,touch_size(50) {
}

CCScene* HelloWorld::scene()  {
	CCScene * scene = NULL;
	do  {
    // 'scene' is an autorelease object
    scene = CCScene::create();
    CC_BREAK_IF(! scene);

    // 'layer' is an autorelease object
    HelloWorld *layer = HelloWorld::create();
    CC_BREAK_IF(! layer);

    // add layer as a child to scene
    scene->addChild(layer);
	} while (0);

	// return the scene
	return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init() {
	bool bRet = false;
	do  {
		//////////////////////////////////////////////////////////////////////////
		// super init first
		//////////////////////////////////////////////////////////////////////////

		CC_BREAK_IF(! CCLayerColor::initWithColor(ccc4(255, 255, 255, 255)));

		// 1. Add a menu item with "X" image, which is clicked to quit the program.
		// Create a "close" menu item with close icon, it's an auto release object.
		CCMenuItemImage *pCloseItem = CCMenuItemImage::create(
			"CloseNormal.png",
			"CloseSelected.png",
			this,
			menu_selector(HelloWorld::menuCloseCallback));
		CC_BREAK_IF(! pCloseItem);

    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();

    // Place the menu item bottom-right corner.
		pCloseItem->setPosition(ccp(origin.x + visibleSize.width -
		    pCloseItem->getContentSize().width/2,
        origin.y + visibleSize.height -
        pCloseItem->getContentSize().height/2));

		// Create a menu with the "close" menu item, it's an auto release object.
		CCMenu* pMenu = CCMenu::create(pCloseItem, NULL);
		pMenu->setPosition(CCPointZero);
		CC_BREAK_IF(!pMenu);

		// Add the menu to HelloWorld layer as a child layer.
		this->addChild(pMenu, 1);

		////////////////////////////
		// 3. bogie
		_bogie = CCSprite::create();
		CCTexture2D* texture = CCTextureCache::sharedTextureCache()->addImage("bogie.png");
		_bogie->setTexture(texture);
		CCRect rect = CCRectZero;
		rect.size = texture->getContentSize();
		_bogie->setContentSize(rect.size);
		_bogie->setTextureRect(rect);
		_bogie->setPosition(ccp(origin.x + visibleSize.width/2,
		    origin.y + 20));
    CCPoint* bogie_velocity = new CCPoint(CCPointZero);
    _bogie->setUserData(bogie_velocity);

		this->addChild(_bogie);


		////////////////////////////
		// 4. ball
		_ball = CCSprite::create();
		CCTexture2D* texture_ball =
		    CCTextureCache::sharedTextureCache()->addImage("ball.png");
		_ball->setTexture(texture_ball);
		CCRect rect_ball = CCRectZero;
		rect_ball.size = texture_ball->getContentSize();
		_ball->setContentSize(rect_ball.size);
		_ball->setTextureRect(rect_ball);
		_ball->setPosition(ccp(origin.x + visibleSize.width/2,
                                 origin.y + visibleSize.height/2));
		_ball->setScale(0.5);
		this->addChild(_ball);

		CCPoint new_target(visibleSize.width + 100, visibleSize.height + 500);
		float actualDuration = (new_target - _ball->getPosition()).getLength() /
		    ball_velocity;
		// Create the actions
		CCFiniteTimeAction* actionMove = CCMoveTo::create( (float)actualDuration,
				new_target );

		CCSequence* ccsequence = CCSequence::create(actionMove, NULL, NULL);
		ccsequence->setTag(0);

		_ball->runAction( ccsequence );

		CCPoint* velocity = new CCPoint((new_target -
		    _ball->getPosition()) / (float)actualDuration);
		_ball->setUserData(velocity);

		this->schedule(schedule_selector(HelloWorld::gameLogic), 1.0);

		this->setTouchEnabled(true);

		_targets = new CCArray;
		_projectiles = new CCArray;
		_boxes = new CCArray;

		this->schedule( schedule_selector(HelloWorld::updateGame) );

		//CocosDenshion::SimpleAudioEngine::sharedEngine()->playBackgroundMusic("background-music-aac.wav", true);

		bRet = true;

		InitializeBoxes();

	} while (0);

	return bRet;
}

void HelloWorld::InitializeBoxes() {
	CCSize winSize = CCDirector::sharedDirector()->getVisibleSize();
	int quant_x = 10;
	int quant_y = 6;
	boxes_quant = quant_x * quant_y;
	CCPoint start(winSize.width / 10, winSize.height * 0.65);
	CCPoint delta(winSize.width * 0.8 / (quant_x - 1),
	    winSize.height / 3 / (quant_y - 1));
	for (int i = 0; i < quant_x; i++) {
		for (int j = 0; j < quant_y; j++) {
			CreateBox(ccp(start.x + i*delta.x, start.y + j*delta.y));
		}
	}
}

void HelloWorld::CreateBox(CCPoint p) {
	CCSprite* box = CCSprite::create();
	CCTexture2D* texture = CCTextureCache::sharedTextureCache()->addImage("box.png");
	box->setTexture(texture);
	CCRect rect = CCRectZero;
	rect.size = texture->getContentSize();
	box->setContentSize(rect.size);
	box->setTextureRect(rect);
	box->setPosition(p);

	this->addChild(box);
	_boxes->addObject(box);
}

void HelloWorld::menuCloseCallback(CCObject* pSender) {
  // create a scene. it's an autorelease object
	MainMenuScene *mainMenuScene = MainMenuScene::create();
	CCDirector::sharedDirector()->replaceScene(mainMenuScene);
}

void HelloWorld::gameLogic(float dt)  {
}

void HelloWorld::ccTouchesBegan(cocos2d::CCSet* touches, cocos2d::CCEvent* event) {
  CCTouch *myTouch = (CCTouch *)touches->anyObject();
  CCPoint location = myTouch->getLocation();

  CCSize winSize = CCDirector::sharedDirector()->getVisibleSize();
  CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();
  CCPoint touch_s(touch_size,touch_size);

  CCRect bogieRect = CCRectMake(
    _bogie->getPosition().x - (_bogie->getContentSize().width * _bogie->getScale()/2),
    _bogie->getPosition().y - (_bogie->getContentSize().height * _bogie->getScale()/2),
    _bogie->getContentSize().width * _bogie->getScale(),
    _bogie->getContentSize().height * _bogie->getScale());

  CCRect touchRect = CCRectMake(
    location.x - touch_s.x / 2,
    location.y - touch_s.y / 2,
    touch_s.x / 2,
    touch_s.y / 2);

  if (touchRect.intersectsRect(bogieRect)) {
    bogie_is_drugging = true;
    last_touches_moved_p = location;
    struct cc_timeval now;
    CCTime::gettimeofdayCocos2d(&now, NULL);
    last_touches_moved_s = (now.tv_sec + (double)(now.tv_usec / 1000000.0f));
  }
}

void HelloWorld::ccTouchesMoved(CCSet* touches, CCEvent* event)	{

  CCTouch *myTouch = (CCTouch *)touches->anyObject();
  CCPoint location = myTouch->getLocation();

  if( bogie_is_drugging ) {
    CCPoint delta_p = location - last_touches_moved_p;
    last_touches_moved_p = location;
    struct cc_timeval now;
    double now_s;
    CCTime::gettimeofdayCocos2d(&now, NULL);
    now_s = (now.tv_sec + (double)(now.tv_usec / 1000000.0f));
    double delta_s = now_s - last_touches_moved_s;
    last_touches_moved_s = now_s;
    CCPoint* bogie_vel = static_cast<CCPoint*>(_bogie->getUserData());
    *bogie_vel = CCPoint(delta_p.x, 0) / delta_s;

    _bogie->stopAllActions();
    CCPoint bogie_p = _bogie->getPosition();
    CCPoint new_target;
    new_target.x = location.x;
    new_target.y = bogie_p.y;
    // Create the actions
    _bogie->setPositionX(new_target.x);
  }

}

void HelloWorld::ccTouchesCancelled(CCSet* touches, CCEvent* event)	{
  CCTouch *myTouch = (CCTouch *)touches->anyObject();
  CCPoint location = myTouch->getLocation();

  bogie_is_drugging = false;
  CCPoint* bogie_vel = static_cast<CCPoint*>(_bogie->getUserData());
  *bogie_vel = CCPointZero;
}

void HelloWorld::ccTouchesEnded(CCSet* touches, CCEvent* event)	{
  bogie_is_drugging = false;
  // Choose one of the touches to work with
  CCTouch* touch = (CCTouch*)(touches->anyObject());
  CCPoint location = touch->getLocation();

  CCLog("++++++++after  x:%f, y:%f", location.x, location.y);
    
  CCSize winSize = CCDirector::sharedDirector()->getVisibleSize();
  CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();

  _bogie->stopAllActions();
  CCPoint bogie_p = _bogie->getPosition();
  CCPoint new_target;
  new_target.x = location.x;
  new_target.y = bogie_p.y;
  // Create the actions
  float actualDuration = (new_target - bogie_p).getLength() / bogie_velocity;
  CCFiniteTimeAction* actionMove = CCMoveTo::create(actualDuration,
      new_target);
  CCFiniteTimeAction* actionMoveDone = CCCallFuncN::create(this,
      callfuncN_selector(HelloWorld::bogieMoveFinished));

  _bogie->runAction(CCSequence::create(actionMove,
      actionMoveDone, NULL));
  CCPoint* bogie_vel = static_cast<CCPoint*>(_bogie->getUserData());
  *bogie_vel = (new_target - bogie_p) / actualDuration;

  //CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("pew-pew-lei.wav");
}

void HelloWorld::bogieMoveFinished(CCNode* sender) {
  CCPoint* bogie_vel = static_cast<CCPoint*>(_bogie->getUserData());
  *bogie_vel = CCPointZero;
}

void HelloWorld::updateGame(float dt) {
	CCSize winSize = CCDirector::sharedDirector()->getVisibleSize();
  CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();

	// compute wall-ball intersections
	int rect_size = 50;
	CCRect left_rect = CCRectMake( origin.x-rect_size, origin.y-rect_size,
			rect_size, winSize.height+2*rect_size );
	CCRect right_rect = CCRectMake( origin.x+winSize.width, origin.y-rect_size,
			rect_size, winSize.height+2*rect_size );
	CCRect up_rect = CCRectMake( origin.x-rect_size, origin.y+winSize.height,
			winSize.width+2*rect_size, rect_size );
	CCRect down_rect = CCRectMake( origin.x-rect_size, origin.y-rect_size,
			winSize.width+2*rect_size, rect_size );

	CCRect ballRect = CCRectMake(
			_ball->getPosition().x -
			(_ball->getContentSize().width * _ball->getScale() / 2),
			_ball->getPosition().y -
			(_ball->getContentSize().height * _ball->getScale() / 2),
			_ball->getContentSize().width * _ball->getScale(),
			_ball->getContentSize().height * _ball->getScale());

	CCRect bogieRect = CCRectMake(
			_bogie->getPosition().x -
			(_bogie->getContentSize().width * _bogie->getScale() / 2),
			_bogie->getPosition().y -
			(_bogie->getContentSize().height * _bogie->getScale() / 2),
			_bogie->getContentSize().width * _bogie->getScale(),
			_bogie->getContentSize().height * _bogie->getScale());

	CCPoint ball_p = _ball->getPosition();
	// vector to target (destination)
	CCPoint targ_vect;
	// get ball velocity
	CCPoint* check_vel = static_cast<CCPoint*>(_ball->getUserData());
	targ_vect = *check_vel;
	bool newAction = false;
	CCPoint new_target;
	// it should be bigger than max screen size
	int ball_destination = 1000;

	if( ballRect.intersectsRect(left_rect) && targ_vect.x < 0 ) {
		newAction = true;
		targ_vect.x = fabs( targ_vect.x );
	}
	if( ballRect.intersectsRect(right_rect) && targ_vect.x > 0 ) {
		newAction = true;
		targ_vect.x = -fabs( targ_vect.x );
	}
	if( ballRect.intersectsRect(up_rect) && targ_vect.y > 0 ) {
		newAction = true;
		targ_vect.y = -fabs( targ_vect.y );
	}
	if( ballRect.intersectsRect(down_rect) && targ_vect.y < 0 ) {
		// here we loose ;(
		GameOverScene *gameOverScene = GameOverScene::create();
		gameOverScene->getLayer()->getLabel()->setString("You Lose :[");
    std::stringstream strs;
    strs << _boxesDestroyed;
    std::string temp_str = strs.str();
    char const* pchar = temp_str.c_str();
    gameOverScene->getLayer()->getPointsLabel()->setString( pchar );
		CCDirector::sharedDirector()->replaceScene(gameOverScene);
	}

	// compute bogie-ball intersections
	if( ballRect.intersectsRect(bogieRect) && targ_vect.y < 0 &&
	    (_ball->getPositionY() +
	    _ball->getContentSize().height / 2 * _ball->getScale()) >=
	    (_bogie->getPositionY() +
	    _bogie->getContentSize().height / 2 * _bogie->getScale())) {
		newAction = true;
		targ_vect.y = fabs(targ_vect.y);
		CCPoint* bogie_vel = static_cast<CCPoint*>(_bogie->getUserData());
		if( fabs(bogie_vel->x) > targ_vect.getLength() )
		  targ_vect.x += targ_vect.getLength() * bogie_vel->x / fabs(bogie_vel->x);
		else
		  targ_vect.x += bogie_vel->x / 2;
	}

	// compute ball-boxes intersections
	CCArray* boxesToDelete = new CCArray;
	CCObject* it = NULL;
	CCObject* jt = NULL;
	CCARRAY_FOREACH(_boxes, it) {
		CCSprite *box = dynamic_cast<CCSprite*>(it);
		CCRect boxRect = CCRectMake(
				box->getPosition().x - (box->getContentSize().width*box->getScale()/2),
				box->getPosition().y - (box->getContentSize().height*box->getScale()/2),
				box->getContentSize().width*box->getScale(),
				box->getContentSize().height*box->getScale());

		if( ballRect.intersectsRect(boxRect) ) {
			boxesToDelete->addObject( it );
	    newAction = true;
	    targ_vect.y *= -1;
		}
	}

	// change ball trajectory
  if( newAction ) {
    _ball->stopAllActions();
    new_target = targ_vect * ball_destination + ball_p;
    // Create the action
    float actualDuration = ( new_target - ball_p ).getLength() / ball_velocity;
    CCFiniteTimeAction* actionMove = CCMoveTo::create( actualDuration,
        new_target );
    _ball->runAction( CCSequence::create(actionMove, NULL) );
    *check_vel = ( new_target - ball_p ) / actualDuration;
    CCLog( "newAction: check_vel = (%f,%f)", check_vel->x, check_vel->y );
  }

  // delete intersection-boxes
	CCARRAY_FOREACH(boxesToDelete, jt)	{
		CCSprite *box = dynamic_cast<CCSprite*>(jt);
		_boxes->removeObject(box);
		this->removeChild(box, true);

		_boxesDestroyed++;
		if( _boxesDestroyed > max_points_result ) {
			max_points_result = _boxesDestroyed;
			CCUserDefault::sharedUserDefault()->setIntegerForKey("max_points_result",
			    max_points_result);
		}
		if ( _boxes->data->num == 0 ) {
		  // No boxes left -> we win!
			GameOverScene *gameOverScene = GameOverScene::create();
			gameOverScene->getLayer()->getLabel()->setString("You Win!");
			std::stringstream strs;
			strs << _boxesDestroyed;
			std::string temp_str = strs.str();
			char const* pchar = temp_str.c_str();
			gameOverScene->getLayer()->getPointsLabel()->setString( pchar );
			CCDirector::sharedDirector()->replaceScene(gameOverScene);
		}
	}
}

void HelloWorld::registerWithTouchDispatcher()  {
	// CCTouchDispatcher::sharedDispatcher()->addTargetedDelegate(this,0,true);
    CCDirector::sharedDirector()->getTouchDispatcher()->addStandardDelegate(this,0);
}
