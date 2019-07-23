#include "stdlib.h"
#include "stdio.h"
#include "iostream"
#include "string"
#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"
#include "ui\CocosGUI.h"
#include "cocos2d.h"
#include "GameScene_3.h"
#include "GameScore.h"
#include "AudioEngine.h"

using namespace std;
using namespace CocosDenshion;
USING_NS_CC;

#define DUTY  100         //任务分数//
#define SPEED 0.1      //蛇的速度//游戏难度的评定标准之一//
#define Union_lit 32   //最小单位（像素为32*32）

Scene* GameScene_3::createScene()
{
	auto scene = Scene::create();
	auto layer = GameScene_3::create();
	scene->addChild(layer,0);
	return scene;
}

bool GameScene_3::init()
{
    if (!Layer::init())
    {
        return false;
    }
 
    Size visibleSize = Director::getInstance()->getVisibleSize();
	flag_2 = 0;

    //蛇和食物的初始化建立//
	sHead = new SnakeNode();
	sHead->row = (visibleSize.height / 2 - 150) / Union_lit + 1;
	sHead->col = (visibleSize.width / 2 - 200) / Union_lit + 1;
	sHead->preNode = NULL;

	sFood = new SnakeNode();
	sFood->row = rand() % 20; //这些20和30是游戏分辨率除以蛇和食物贴图的分辨率所得
	sFood->col = rand() % 30; 

	trap_row_1 = 12;
	trap_col_1 = rand() % 16;

	trap_row_2 = rand() % 20;
	trap_col_2 = 26;

	door_row_1 = (visibleSize.height - 120) / Union_lit;
	door_col_1 = (visibleSize.width / 2 + 120) / Union_lit;
	door_row_2 = (visibleSize.height / 2 - 150) / Union_lit;
	door_col_2 = (visibleSize.width / 2 - 200) / Union_lit;
   //食物不能和陷阱一处//
	Gamedonwor(&sFood->row, &sFood->col, &trap_col_1, &trap_row_1, &trap_row_2, &trap_col_2);
	
	auto snakeNode = Sprite::create("snakehead.png");
	snakeNode->setAnchorPoint(Point::ANCHOR_BOTTOM_LEFT);
	snakeNode->setPosition(Point(sHead->col * Union_lit, sHead->row * Union_lit));
	snakeNode->setVisible(true); //食物隐藏的“开关”//这是为游戏暂停做铺垫的
	sHead->nodeSprite = snakeNode; 
	this->addChild(snakeNode,1);

	auto foodNode = Sprite::create("apple.png");
	foodNode->setAnchorPoint(Point::ANCHOR_BOTTOM_LEFT);
	foodNode->setPosition(Point(sFood->col * Union_lit, sFood->row * Union_lit));
	foodNode->setVisible(true);    //食物隐藏的“开关”//
	sFood->nodeSprite = foodNode;
	this->addChild(foodNode,3);
    // 分数板//
	snake_score = 0;
	auto labelScore = Label::createWithSystemFont("Score: 0", "宋体", 25);
	labelScore->setTag(110);
	labelScore->setPosition(Point(visibleSize.width - 100, visibleSize.height-50));
	this->addChild(labelScore,1);
	 //分数任务//
	game_target = DUTY;
	auto TargetScore = Label::createWithSystemFont("POI", "宋体", 25);
	TargetScore->setTag(111);
	TargetScore->setPosition(Point(visibleSize.width - 100, visibleSize.height - 70));
	this->addChild(TargetScore, 1);

	char strtarget[20];
	sprintf(strtarget, "TARGET: %d", game_target);    
	auto label = (Label *)this->getChildByTag(111);
	label->setString(strtarget);
    //陷阱设置//
	Trap_1 = Sprite::create("Trap.png");
	Trap_1->setPosition(Vec2(trap_col_1 * Union_lit, trap_row_1 * Union_lit));
	Trap_1->setVisible(true);
	this->addChild(Trap_1,2);

	Trap_2 = Sprite::create("Trap.png");
	Trap_2->setPosition(Vec2(trap_col_2 * Union_lit, trap_row_2 * Union_lit));
	Trap_2->setVisible(true);
	this->addChild(Trap_2, 2);

	Door_1 = Sprite::create("door.png");
	Door_1->setPosition(Vec2(door_col_1 * Union_lit, door_row_1 * Union_lit));
	Door_1->setVisible(true);
	this->addChild(Door_1, 2);

	Door_2 = Sprite::create("door.png");
	Door_2->setPosition(Vec2(door_col_2 * Union_lit, door_row_2 * Union_lit));
	Door_2->setVisible(true);
	this->addChild(Door_2, 2);
    //回到菜单场景//
	labelBack = Label::create("Main Menu", "Arial", 30);
	labelBack->setVisible(false);
	itemBack = MenuItemLabel::create(labelBack, CC_CALLBACK_1(GameScene_3::menuBackToMain, this)); 
	itemBack->setPosition(Point(visibleSize.width / 2, visibleSize.height / 2 - 110));
	itemBack->setAnchorPoint(Point::ANCHOR_MIDDLE_BOTTOM);
	itemBack->setEnabled(false);
	auto menu_1 = Menu::create(itemBack, NULL);
	menu_1->setPosition(Point(0, 0));
	this->addChild(menu_1, 1);

	 NextItem = MenuItemImage::create(
		"Next.png",
		"Next.png",
		CC_CALLBACK_1(GameScene_3::NextScene, this));
	 NextItem->setVisible(false);
	 NextItem->setEnabled(false);
	 menu_2 = Menu::create(NextItem, NULL);
     menu_2->setPosition(Vec2(visibleSize.width / 2 , visibleSize.height / 2 - 200 ));
	 this->addChild(menu_2, 1);
	 //暂停按钮//
	 stop = Label::create("Pause", "Arial", 20);
	 stop->setVisible(true);
	 stop->setOpacity(70);
	 stopItem = MenuItemLabel::create(stop, CC_CALLBACK_1(GameScene_3::Gamestop, this));
	 stopItem->setPosition(Point(visibleSize.width - 10, 20));
	 stopItem->setAnchorPoint(Point::ANCHOR_BOTTOM_RIGHT);
	 stopItem->setTag(114);
	 stopItem->setEnabled(true);
	 auto menu_4 = Menu::create(stopItem, NULL);
	 menu_4->setPosition(Point(0, 0));
	 this->addChild(menu_4, 1);
	 //继续按钮//
	 rebegin = Label::create("Continue", "Arial", 100);              
	 rebegin->setVisible(false);
	 rebeginItem = MenuItemLabel::create(rebegin, CC_CALLBACK_1(GameScene_3::Gamecontinue, this));
	 rebeginItem->setPosition(Point(visibleSize.width / 2, visibleSize.height / 2));
	 rebeginItem->setAnchorPoint(Point::ANCHOR_MIDDLE_BOTTOM);
	 rebeginItem->setTag(115);
	 rebeginItem->setEnabled(false);
	 auto menu_5 = Menu::create(rebeginItem, NULL);
	 menu_5->setPosition(Point(0, 0));
	 this->addChild(menu_5, 1);

     //背景音乐//
	 SimpleAudioEngine::getInstance()->playBackgroundMusic(Gamemusic, true);
     //背景图片//
	 auto sprite = Sprite::create(GamePicture);
	 sprite->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
	 sprite->setOpacity(90);
	 this->addChild(sprite, 0);

	 nSecond = 0;
	 nMinute = 0;
	 auto labelTime = Label::createWithSystemFont("Time 00:00", "Arial", 25);
	 labelTime->setTag(118);
	 labelTime->setPosition(Point(visibleSize.width - 100, visibleSize.height - 28));
	 this->addChild(labelTime, 1);

     //触点聆听者的初始化//
	auto touchListener = EventListenerTouchOneByOne::create();
	touchListener->setSwallowTouches(true);
	touchListener->onTouchBegan = CC_CALLBACK_2(GameScene_3::onTouchBegan, this);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener, this);

	this->schedule(SEL_SCHEDULE(&GameScene_3::TimeCounter), 1);
	this->schedule(SEL_SCHEDULE(&GameScene_3::logic), SPEED);
    return true;
}
 
void GameScene_3::menuBackToMain( cocos2d::Ref* pSender ) //回到菜单场景//
{
	auto transition_1 = TransitionFade::create(0.5, HelloWorld::createScene()); 
	SimpleAudioEngine::getInstance()->playBackgroundMusic(Titlemusic, true);
	Director::getInstance()->replaceScene(transition_1);
}

void GameScene_3::NextScene(cocos2d::Ref* pSender) //下一关//
{
	//关闭输入//
	textName->detachWithIME();
	//获取输入//只有当关闭了输入才能获取输入//
	string getname = textName->getString();
	GameScore::playername = getname.substr(0,7);
	auto transition_3 = TransitionFade::create(0.5,GameScore::createScene());  
	Director::getInstance()->replaceScene(transition_3);
}


 //避免食物和陷阱位置重复//
void GameScene_3::Gamedonwor(int * Fr, int * Fc, int * Tc_1, int * Tr_1, int * Tr_2, int * Tc_2)
{ 
	//陷阱与陷阱之间不重复//
	while ((*Tc_2 == *Tc_1 && * Tr_2 == *Tr_1) && (*Tc_2 - 1 == *Tc_1 - 1 && *Tr_2 - 1 == *Tr_1 - 1) && (*Tc_2 == *Tc_1 &&  *Tr_2 - 1 == *Tr_1 - 1) || (*Tc_2 - 1 == *Tc_1 - 1 && * Tr_2 == *Tr_1))
	{
		*Tr_1 = 12;
		*Tc_1 = rand() % 16;
	}
	//食物与两个陷阱不重复//
	while ((*Fc == *Tc_1 && * Fr == *Tr_1) || (*Fc == *Tc_1 - 1 && * Fr == *Tr_1 - 1) || (*Fc == *Tc_1 && * Fr == *Tr_1 - 1) || (*Fc == *Tc_1 - 1 && * Fr == *Tr_1))
	{
		*Fr = rand() % 20;
		*Fc = rand() % 30;
	}
	while ((*Fc == *Tc_2 && * Fr == *Tr_2) || (*Fc == *Tc_2 - 1 && * Fr == *Tr_2 - 1) || (*Fc == *Tc_2 && * Fr == *Tr_2 - 1) || (*Fc == *Tc_2 - 1 && * Fr == *Tr_2))
	{
		*Fr = rand() % 20;
		*Fc = rand() % 30;
	}
}

void GameScene_3::GamePause() //游戏暂停和隐身//
{
	sHead->nodeSprite->setVisible(false);
	int i;
	for (i = 0; i < allbody.size(); i++)
		allbody.at(i)->nodeSprite->setVisible(false);
	this->pause();
	sFood->nodeSprite->setVisible(false);
	Trap_1->setVisible(false);
	Trap_2->setVisible(false);
	Door_1->setVisible(false);
	Door_2->setVisible(false);
}

void GameScene_3::GameResume()
{

	sHead->nodeSprite->setVisible(true);
	int i;
	for (i = 0; i < allbody.size(); i++)
		allbody.at(i)->nodeSprite->setVisible(true);
	sFood->nodeSprite->setVisible(true);
	Trap_1->setVisible(true);
	Trap_2->setVisible(true);
	Door_1->setVisible(true);
	Door_2->setVisible(true);
	this->resume();
}

void GameScene_3::Gamestop(cocos2d::Ref* pSender)
{
	GamePause();
	stop->setVisible(false);
	stopItem->setEnabled(false);
	rebegin->setVisible(true);
	rebeginItem->setEnabled(true);
	labelBack->setVisible(true);
	itemBack->setEnabled(true);

}

void GameScene_3::Gamecontinue(cocos2d::Ref* pSender)
{
	rebegin->setVisible(false);
	rebeginItem->setEnabled(false);
	labelBack->setVisible(false);
	itemBack->setEnabled(false);
	stop->setVisible(true);
	stopItem->setEnabled(true);	
	GameResume();
}

void GameScene_3::GameNextScene()
{

	GamePause();
	stop->setVisible(false);
	stopItem->setEnabled(false);
	NextItem->setVisible(true);
	NextItem->setEnabled(true);
	Trap_1->setVisible(false);
	Trap_2->setVisible(false);
	Door_1->setVisible(false);
	Door_2->setVisible(false);
	
	char str[4];
	str[0] = '0' + (nMinute / 10);
	str[1] = '0' + (nMinute % 10);
	str[2] = '0' + (nSecond / 10);
	str[3] = '0' + (nSecond % 10);
	
	GameScore::GetyouScore_INT(str, snake_score);
	SimpleAudioEngine::getInstance()->preloadBackgroundMusic(Wintermusic);
	SimpleAudioEngine::getInstance()->playBackgroundMusic(Wintermusic, false);

}

void GameScene_3::GameEnd()
{
	GameNextScene();
	Size visibleSize = Director::getInstance()->getVisibleSize();
    textName = TextFieldTTF::textFieldWithPlaceHolder("Please tell me your name", "Helvetica", 24);
	textName->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
	this->addChild(textName, 1);
	//开启输入
	textName->attachWithIME();
	//当点击下一场景的按钮时便停止输入//
}

bool GameScene_3::onTouchBegan(Touch* touch, Event* event)   
{
	Point touchPoint = touch->getLocation();
	int touchRow = ((int)touchPoint.y) / 32;   //图片像素是32*32//
	int touchCol = ((int)touchPoint.x) / 32;
	//蛇头和蛇身//高大于宽
	if (abs(touchRow - sHead->row) > abs(touchCol - sHead->col))  
	{
		if (touchRow > sHead->row)
		{

			if (DIR_DEF::UP == sHead->dir || DIR_DEF::DOWN == sHead->dir)
			{
				if (touchCol > sHead->col)
				{

					sHead->dir = DIR_DEF::RIGHT;
				}
				else
				{
					sHead->dir = DIR_DEF::LEFT;
				}
			}
			else
				sHead->dir = DIR_DEF::UP;
		}
		else
		{
			if (DIR_DEF::DOWN == sHead->dir || DIR_DEF::UP == sHead->dir)
			{
				if (touchCol > sHead->col)
				{

					sHead->dir = DIR_DEF::RIGHT;
				}
				else
				{
					sHead->dir = DIR_DEF::LEFT;
				}
			}
			else
				sHead->dir = DIR_DEF::DOWN;

		}
	}

	else      //蛇头和蛇身//宽大于高
	{
		if (touchCol > sHead->col)
		{

			if (DIR_DEF::RIGHT == sHead->dir || DIR_DEF::LEFT == sHead->dir)
			{
				if (touchRow > sHead->row)
				{

					sHead->dir = DIR_DEF::UP;
				}
				else
				{
					sHead->dir = DIR_DEF::DOWN;
				}
			}
			else
				sHead->dir = DIR_DEF::RIGHT;

		}
		else
		{
			if (DIR_DEF::LEFT == sHead->dir || DIR_DEF::RIGHT == sHead->dir)
			{
				if (touchRow > sHead->row)
				{

					sHead->dir = DIR_DEF::UP;
				}
				else
				{
					sHead->dir = DIR_DEF::DOWN;
				}
			}
			else
				sHead->dir = DIR_DEF::LEFT;
		}

	}
	return true;
}

void GameScene_3::TimeCounter(float dt)
{
	nSecond++;
	if (nSecond == 60)
	{
		nSecond = 0;
		nMinute++;
		if (nMinute == 60)
		{
			nMinute = 99;
			nSecond = 99;
		}
	}
	char str[6];
	str[0] = '0' + (nMinute / 10);
	str[1] = '0' + (nMinute % 10);
	str[2] = ':';
	str[3] = '0' + (nSecond / 10);
	str[4] = '0' + (nSecond % 10);
	str[5] = '\0';
	auto label = (Label *)this->getChildByTag(118);
	char strtime[12];
	sprintf(strtime, "Time %s", str);
	label->setString(strtime);

}

//游戏逻辑（难点）//
void GameScene_3::logic(float t)
{
	for (int i = allbody.size() - 1; i >= 0; i--) 
	{
		auto sn = allbody.at(i);
		if (allbody.empty())
		{

			sn->dir = sHead->dir;
			sn->row = sHead->row;
			sn->col = sHead->col;
		}
		else
		{
			sn->col = sn->preNode->col;
			sn->row = sn->preNode->row;
			sn->dir = sn->preNode->dir;
		}
		sn->nodeSprite->setPosition(Point(sn->col * Union_lit, sn->row * Union_lit));
	}

	switch (sHead->dir)       //头部方向设置//
	{
	case DIR_DEF::UP:
		sHead->row++;
		//头部旋转，至少不是死头//
		sHead->nodeSprite->setAnchorPoint(Point::ANCHOR_TOP_RIGHT);
		sHead->nodeSprite->setRotation(180);
		break;
	case DIR_DEF::DOWN:
		sHead->row--;

		sHead->nodeSprite->setAnchorPoint(Point::ANCHOR_BOTTOM_LEFT);  ///////
		sHead->nodeSprite->setRotation(0);
		
		break;
	case DIR_DEF::LEFT:
		sHead->col--;
		sHead->nodeSprite->setAnchorPoint(Point::ANCHOR_BOTTOM_RIGHT);
		sHead->nodeSprite->setRotation(90);
	
		break;
	case DIR_DEF::RIGHT:
		sHead->col++;
		sHead->nodeSprite->setAnchorPoint(Point::ANCHOR_TOP_LEFT);
		sHead->nodeSprite->setRotation(-90);
		
		break;
	}
	                         //边界穿越//
	if (sHead->col >= 30)
	{
		sHead->col = 0;
	}
	else if (sHead->col < 0)
	{
		sHead->col = 30;
	}

	if (sHead->row >= 20)
	{
		sHead->row = 0;
	}
	else if (sHead->row < 0)
	{
		sHead->row = 20;
	}

	//重新定位//
	sHead->nodeSprite->setPosition(Point(sHead->col * Union_lit, sHead->row * Union_lit)); 

	if (sHead->col == sFood->col && sHead->row == sFood->row)
	{     
		//进食音响//
		SimpleAudioEngine::getInstance()->playEffect("Yumi.mp3");
		//食物再生//
		sFood->row = rand() % 20;
		sFood->col = rand() % 30;
		 //调用食物不可与陷阱重复函数//
		Gamedonwor(&sFood->row, &sFood->col, &trap_col_1, &trap_row_1, &trap_row_2, &trap_col_2);
		
		sFood->nodeSprite->setPosition(Point(sFood->col * Union_lit, sFood->row * Union_lit));
         //分数增加//
		this->snake_score += 1;
		auto label = (Label *)this->getChildByTag(110);
		char strscore[20];
		sprintf(strscore, "Score: %d", snake_score);
		label->setString(strscore);
		//长身体了//
		auto snakeNode = new SnakeNode();
		if (allbody.empty())
		{
			snakeNode->preNode = sHead;                    

		}
		else
		{
			snakeNode->preNode = allbody.back();
		}
		switch (snakeNode->preNode->dir)
		{
		case DIR_DEF::UP:
			snakeNode->row = snakeNode->preNode->row - 1;
			snakeNode->col = snakeNode->preNode->col;
			break;
		case DIR_DEF::DOWN:
			snakeNode->row = snakeNode->preNode->row + 1;
			snakeNode->col = snakeNode->preNode->col;
			break;
		case DIR_DEF::LEFT:
			snakeNode->row = snakeNode->preNode->row;
			snakeNode->col = snakeNode->preNode->col + 1;
			break;
		case DIR_DEF::RIGHT:
			snakeNode->row = snakeNode->preNode->row;
			snakeNode->col = snakeNode->preNode->col - 1;
			break;
		}

		auto nodeSprite = Sprite::create("body.png");
		nodeSprite->setPosition(Point(snakeNode->col * Union_lit, snakeNode->row * Union_lit));
		nodeSprite->setAnchorPoint(Point::ANCHOR_BOTTOM_LEFT);
		nodeSprite->setVisible(true);  //蛇身隐藏按钮//
		snakeNode->nodeSprite = nodeSprite;
		this->addChild(nodeSprite, 1);

		allbody.pushBack(snakeNode);
	}

	//传送代码//必须放在吃到食物代码后面，食物的优先级比传送门高//
	//从上门（门一）到下门（门二）
	if (0 == flag_2)
	{
		if (((sHead->col == door_col_1  && sHead->row == door_row_1) || (sHead->col == door_col_1 - 1 && sHead->row == door_row_1 - 1) || (sHead->col == door_col_1 && sHead->row == door_row_1 - 1) || (sHead->col == door_col_1 - 1 && sHead->row == door_row_1)))
		{
			sHead->col = door_col_2;
			sHead->row = door_row_2;
			flag_2 = 1;

		}
		else if (((sHead->col == door_col_2  && sHead->row == door_row_2) || (sHead->col == door_col_2 - 1 && sHead->row == door_row_2 - 1) || (sHead->col == door_col_2 && sHead->row == door_row_2 - 1) || (sHead->col == door_col_2 - 1 && sHead->row == door_row_2)))
		{
			sHead->col = door_col_1;
			sHead->row = door_row_1;
			flag_2 = 1;
		}
	}
	else
		flag_2 = 0;
	//贪吃蛇重新定位//
	sHead->nodeSprite->setPosition(Point(sHead->col * Union_lit, sHead->row * Union_lit)); 
	if (snake_score == game_target) //分数达到//
		   GameEnd();

	//死亡判定之自己碰撞//
	if (allbody.size() > 1) 
	{
		int k;
		bool flag_1(false);
		for (k = 0; k < allbody.size(); k++)
		{
			if (sHead->col == allbody.at(k)->col && sHead->row == allbody.at(k)->row)
			{
				flag_1 = true;
				break;
			}
		}
		if (flag_1)
	       GameEnd();	
	}
	//碰到障碍——死//
	if (((sHead->col == trap_col_1  && sHead->row == trap_row_1) || (sHead->col == trap_col_1 - 1 && sHead->row == trap_row_1 - 1) || (sHead->col == trap_col_1 && sHead->row == trap_row_1 - 1) || (sHead->col == trap_col_1 - 1 && sHead->row == trap_row_1))    ||    ((sHead->col == trap_col_2  && sHead->row == trap_row_2) || (sHead->col == trap_col_2 - 1 && sHead->row == trap_row_2 - 1) || (sHead->col == trap_col_2 && sHead->row == trap_row_2 - 1) || (sHead->col == trap_col_2 - 1 && sHead->row == trap_row_2)))
		GameEnd();
}
