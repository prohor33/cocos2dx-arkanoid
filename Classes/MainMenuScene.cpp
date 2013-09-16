#include "MainMenuScene.h"
#include "HelloWorldScene.h"

using namespace cocos2d;

bool MainMenuScene::init()  {
	if (CCScene::init())	{
		this->_layer = MainMenuLayer::create();
		this->_layer->retain();
		this->addChild(_layer);
		
		return true;
	} else  {
		return false;
	}
}

MainMenuScene::~MainMenuScene() {
	if (_layer) {
		_layer->release();
		_layer = NULL;
	}
}


bool MainMenuLayer::init()
{
	if (CCLayerColor::initWithColor(ccc4(255, 255, 255, 255)))	{
    // Create a "close" menu item with close icon, it's an auto release object.
    CCMenuItemImage *pStartGameItem = CCMenuItemImage::create(
        "start-the-game.png",
        "start-the-game-sel.png",
        this,
        menu_selector(MainMenuLayer::menuStartGameCallback));

    CCMenuItemImage *pExitItem = CCMenuItemImage::create(
        "exit.png",
        "exit-sel.png",
        this,
        menu_selector(MainMenuLayer::menuExitCallback));

    // Place the menu item bottom-right conner.
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();

    int v_space = 5;
    pExitItem->setPosition(ccp(origin.x + visibleSize.width/2,
        origin.y + visibleSize.height * 1 / 3));
    pStartGameItem->setPosition(ccp(pExitItem->getPositionX(),
        pExitItem->getPositionY() +
        pExitItem->getContentSize().height / 2 +
        pStartGameItem->getContentSize().height / 2 +
        v_space));

    // Create a menu with the "close" menu item, it's an auto release object.
    CCMenu* pMenu = CCMenu::create(pStartGameItem, pExitItem, NULL);
    pMenu->setPosition(CCPointZero);

    // Add the menu to HelloWorld layer as a child layer.
    this->addChild(pMenu, 1);

    // loading best score
    HelloWorld::max_points_result =
        CCUserDefault::sharedUserDefault()->getIntegerForKey(
            "max_points_result");

    // Max points label
    v_space = 10;
    std::stringstream strs;
    strs << HelloWorld::max_points_result;
    std::string temp_str = strs.str();
    char const* pchar = temp_str.c_str();
    max_points_label = CCLabelTTF::create(pchar,
        "Artial", 20);
    max_points_label->retain();
    max_points_label->setColor(ccc3(0, 255, 0));
    max_points_label->setPosition(
        ccp(pExitItem->getPositionX(),
            pStartGameItem->getPositionY() +
            pStartGameItem->getContentSize().height / 2 +
            max_points_label->getFontSize() / 2 + v_space));
    this->addChild(max_points_label);

    // Above max points label
    v_space = 3;
    CCLabelTTF* max_points_above_label = CCLabelTTF::create("Лучший результат",
        "Artial", 18);
    max_points_above_label->retain();
    max_points_above_label->setColor(ccc3(0, 0, 0));
    max_points_above_label->setPosition(
        ccp(pExitItem->getPositionX(),
            max_points_label->getPositionY() +
            max_points_label->getContentSize().height / 2 +
            max_points_above_label->getFontSize() / 2 + v_space));
    this->addChild(max_points_above_label);
	}
	else	{
		return false;
	}
}

MainMenuLayer::~MainMenuLayer() {
	if (max_points_label)	{
	  max_points_label->release();
	  max_points_label = NULL;
	}
}

void MainMenuLayer::menuExitCallback(CCObject* pSender) {
	// "close" menu item clicked
	CCDirector::sharedDirector()->end();
}

void MainMenuLayer::menuStartGameCallback(CCObject* pSender)  {
  // create a scene. it's an autorelease object
  CCScene *pScene = HelloWorld::scene();
	CCDirector::sharedDirector()->replaceScene(pScene);
}
