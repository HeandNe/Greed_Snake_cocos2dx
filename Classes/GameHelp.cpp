#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"
#include "ui\CocosGUI.h"
#include "cocos2d.h"
#include"GameHelp.h"
#include "AudioEngine.h"

using namespace std;
USING_NS_CC;

Scene* GameHelp::createScene()
{
	auto scene = Scene::create();
	auto layer = GameHelp::create();
	scene->addChild(layer);
	return scene;
}
bool GameHelp::init()
{
    if (!Layer::init())
    {
        return false;
    }
     
    Size visibleSize = Director::getInstance()->getVisibleSize();
	
	auto Nextduty = Sprite::create("Next.png");
	Nextduty->setAnchorPoint(Point::ANCHOR_MIDDLE);
	Nextduty->setPosition(Point(visibleSize.width / 2, visibleSize.height / 2 + 150));
	this->addChild(Nextduty, 1);

	auto next_word = Label::create("       Click it\ngo to next scene", "Arial", 20);
	next_word->setPosition(Point(visibleSize.width / 2 , visibleSize.height / 2 + 70));
	next_word->setAnchorPoint(Point::ANCHOR_MIDDLE);
	this->addChild(next_word, 1);

	auto foodNode = Sprite::create("apple.png");
	foodNode->setAnchorPoint(Point::ANCHOR_MIDDLE);
	foodNode->setPosition(Point(visibleSize.width / 2 -200,visibleSize.height /2 +30));
	this->addChild(foodNode, 1);

	auto food_word = Label::create(" Eat the apple\nLet snake stronger", "Arial", 20);
	food_word->setPosition(Point(visibleSize.width / 2 - 200, visibleSize.height / 2 - 30));
	food_word->setAnchorPoint(Point::ANCHOR_MIDDLE);
	this->addChild(food_word, 1);

	auto Trap = Sprite::create("Trap.png");
	Trap->setAnchorPoint(Point::ANCHOR_MIDDLE);
	Trap->setPosition(Point(visibleSize.width / 2, visibleSize.height / 2 -90));
	this->addChild(Trap, 1);

	auto trap_word = Label::create("Crash in it\nYou will die", "Arial", 20);
	trap_word->setPosition(Point(visibleSize.width / 2, visibleSize.height / 2 - 130));
	trap_word->setAnchorPoint(Point::ANCHOR_MIDDLE);
	this->addChild(trap_word, 1);

	auto Door = Sprite::create("door.png");
	Door->setAnchorPoint(Point::ANCHOR_MIDDLE);
	Door->setPosition(Point(visibleSize.width / 2 + 200, visibleSize.height / 2 + 30));
	this->addChild(Door, 1);

	auto door_word = Label::create(" Get into it\nWhere you go??", "Arial", 20);
	door_word->setPosition(Point(visibleSize.width / 2 + 200, visibleSize.height / 2 - 30));
	door_word->setAnchorPoint(Point::ANCHOR_MIDDLE);
	this->addChild(door_word, 1);

    auto labelBack = Label::create("Main Menu", "Marker Felt", 30);
    auto itemBack = MenuItemLabel::create(labelBack, CC_CALLBACK_1(GameHelp::menuBackToMain, this));
    itemBack->setPosition(Point(visibleSize.width / 2, visibleSize.height / 2 - 200 ));
 
    auto menu = Menu::create(itemBack, NULL);
    menu->setPosition(Point(0, 0));
    this->addChild(menu,1);

	auto sprite = Sprite::create("help.jpg");
	sprite->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2 ));
	sprite->setOpacity(95);
	this->addChild(sprite, 0);
	
    return true;
}

void GameHelp::menuBackToMain(cocos2d::Ref* pSender)
{
	Director::getInstance()->replaceScene(HelloWorld::createScene());
}

