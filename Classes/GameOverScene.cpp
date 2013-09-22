#include "GameOverScene.h"
#include "HelloWorldScene.h"
#include"MainMenuScene.h"

using namespace cocos2d;

bool GameOverScene::init()  {
	if( CCScene::init() ) {
		this->_layer = GameOverLayer::create();
		this->_layer->retain();
		this->addChild(_layer);
		
		return true;
	} else  {
		return false;
	}
}

GameOverScene::~GameOverScene() {
	if (_layer)	{
		_layer->release();
		_layer = NULL;
	}
}


bool GameOverLayer::init()  {
	if (CCLayerColor::initWithColor(ccc4(255, 255, 255, 255))) {
		CCSize winSize = CCDirector::sharedDirector()->getWinSize();
		this->_label = CCLabelTTF::create("", "Artial", 32);
		_label->retain();
		_label->setColor(ccc3(0, 0, 0));
		_label->setPosition(ccp(winSize.width / 2, winSize.height * 2 / 5));
		this->addChild(_label);
		
		int v_space = 10;
		this->_points_label = CCLabelTTF::create("", "Artial", 40);
		_points_label->retain();
		_points_label->setColor(ccc3(0, 255, 0));
		_points_label->setPosition(
		    ccp(winSize.width / 2,
		    _label->getPositionY() + _label->getFontSize() + v_space));
		this->addChild(_points_label);

		CCLabelTTF* above_points_label = CCLabelTTF::create("Your Score",
		    "Artial", 32);
		above_points_label->retain();
		above_points_label->setColor(ccc3(0, 0, 0));
		above_points_label->setPosition(
        ccp(winSize.width / 2,
            _points_label->getPositionY() +
            _points_label->getFontSize() + v_space));
		this->addChild(above_points_label);

		this->runAction( CCSequence::create(
		    CCDelayTime::create(3),
        CCCallFunc::create(this,
        callfunc_selector(GameOverLayer::gameOverDone)),
        NULL));
		
		return true;
	}
	else  {
		return false;
	}
}

void GameOverLayer::gameOverDone()  {
	MainMenuScene *mainMenuScene = MainMenuScene::create();
	CCDirector::sharedDirector()->replaceScene(mainMenuScene);
}

GameOverLayer::~GameOverLayer() {
	if (_label) {
		_label->release();
		_label = NULL;
	}
}
