#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"
#include "ui\CocosGUI.h"
#include "cocos2d.h"
#include "AudioEngine.h"

#include "GameHelp.h"
#include "GameScene_1.h"

using namespace CocosDenshion;
USING_NS_CC;

//POI : 这里就是游戏的第一个界面

Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    //  super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    //POI : 下面是在部署菜单？

	//POI ： 先弄好菜单上面的按钮//这里我是用图片按钮
    auto closeItem = MenuItemImage::create(
                                           "CloseNormal.png",
                                           "CloseSelected.png",
                                           CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));

    closeItem->setPosition(Vec2(origin.x + visibleSize.width - closeItem->getContentSize().width/2 ,
                                origin.y + closeItem->getContentSize().height/2));
	closeItem->setTag(EXIT);
	auto startItem = MenuItemImage::create(
		                                      "begining.png",
		                                      "begining.png",
		                                      CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));
	startItem->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2 - 100));
	startItem->setTag(START);

	auto helpItem = MenuItemImage::create(
		                                       "help.png",
		                                       "help.png",
		                                     CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));
	helpItem->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2 - 200));
	helpItem->setTag(HELP);

	//POI ： 这里是创建菜单，并把上面弄好的按钮 安装上去
	// create menu, it's an autorelease object
	auto menu = Menu::create(closeItem,startItem,helpItem, NULL);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, 1);   //优先级是1//POI ：把菜单弄在Scene上（值得注意这里this就是属于类HelloWorld）

	//POI ： 继续在场景Scene上面弄点文字
	auto Authorlabel = Label::create("@HeandNe_zengrihui", "Arial", 15);
	Authorlabel->setPosition(Point(0,0));
	Authorlabel->setAnchorPoint(Point::ANCHOR_BOTTOM_LEFT);
	this->addChild(Authorlabel, 1);
	
	auto Title_word = Label::create("~SNAKE~", "Arial", 100);
	Title_word->setPosition(Point(visibleSize.width / 2, visibleSize.height / 2 + 100));
	Title_word->setAnchorPoint(Point::ANCHOR_MIDDLE);
	this->addChild(Title_word, 1);
	//POI : 这居然是背景图片
	auto sprite = Sprite::create("title_1.jpg");
    sprite->setPosition(Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));
	sprite->setOpacity(98);
    // add the sprite as a child to this layer
    this->addChild(sprite, 0);                         //优先级是0//
    return true;
}
//这里这里是菜单的调用的函数（根据按钮上面所代表的Tag，选择对应的函数）
void HelloWorld::menuCloseCallback(Ref* pSender)
{
	//在下面switch选择中，导演就是负责切换场景或者结束场景
	switch (((Node*)pSender)->getTag())
	{
	case START:
		CCLOG("start your game!");
		Director::getInstance()->replaceScene(GameScene_1::createScene());
		break;
	case HELP:
		CCLOG("HELP ME ALONE");
		Director::getInstance()->replaceScene(GameHelp::createScene());
		break;

	case EXIT:
		Director::getInstance()->end();
		break;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
		exit(0);
#endif

	}
	   /*To navigate back to native iOS screen(if present) without quitting the application  ,do not use Director::getInstance()->end() and exit(0) as given above,instead trigger a custom event created in RootViewController.mm as below*/
    
    //EventCustom customEndEvent("game_scene_close_event");
    //_eventDispatcher->dispatchEvent(&customEndEvent);
}
