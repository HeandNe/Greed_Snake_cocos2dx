#include "stdlib.h"
#include "stdio.h"
#include "iostream"
#include "string"
#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"
#include "ui\CocosGUI.h"
#include "cocos2d.h"
#include"GameScene_1.h"
#include"GameScene_2.h"
#include "AudioEngine.h"


using namespace std;
using namespace CocosDenshion;
USING_NS_CC;
#define DUTY 2        //�������//
#define SPEED 0.2    //�ߵ��ٶ�//��Ϸ�Ѷȵ�������׼֮һ//

Scene* GameScene_1::createScene()
{
	auto scene = Scene::create();
	auto layer = GameScene_1::create();
	scene->addChild(layer, 0);
	return scene;
}
bool GameScene_1::init()
{
	if (!Layer::init())
	{
		return false;
	}

	Size visibleSize = Director::getInstance()->getVisibleSize();
	

	//�ߺ�ʳ��ĳ�ʼ������//
	sHead = new SnakeNode();
	sHead->row = rand() % 20; //���������λ//
	sHead->col = rand() % 30;
	sHead->preNode = NULL;


	sFood = new SnakeNode();
	//��Щ20��30����Ϸ�ֱ��ʳ����ߺ�ʳ����ͼ�ķֱ�������
	sFood->row = rand() % 20; 
	sFood->col = rand() % 30;

	auto snakeNode = Sprite::create("snakehead.png");
	snakeNode->setAnchorPoint(Point::ANCHOR_BOTTOM_LEFT);
	snakeNode->setPosition(Point(sHead->col * 32, sHead->row * 32));
	snakeNode->setVisible(true); //ʳ�����ع���//����Ϊ��Ϸ��ͣ���̵��
	sHead->nodeSprite = snakeNode;
	this->addChild(snakeNode, 1);

	auto foodNode = Sprite::create("apple.png");
	foodNode->setAnchorPoint(Point::ANCHOR_BOTTOM_LEFT);
	foodNode->setPosition(Point(sFood->col * 32, sFood->row * 32));
	foodNode->setVisible(true);  //ʳ�����عر�//
	sFood->nodeSprite = foodNode;
	this->addChild(foodNode, 1);
	// ������//
	snake_score = 0;
	auto labelScore = Label::createWithSystemFont("Score: 0", "����", 25);
	labelScore->setTag(110);
	labelScore->setPosition(Point(visibleSize.width - 100, visibleSize.height - 50));
	this->addChild(labelScore, 1);
	//��������//
	game_target = DUTY;
	auto TargetScore = Label::createWithSystemFont("POI", "����", 25);
	TargetScore->setTag(111);
	TargetScore->setPosition(Point(visibleSize.width - 100, visibleSize.height - 70));

	this->addChild(TargetScore, 1);

	char strtarget[20];
	sprintf(strtarget, "TARGET: %d", game_target);//��������ı��������//
	auto label = (Label *)this->getChildByTag(111);
	label->setString(strtarget);

    //��Ϸ��������//�м���ͷ�ļ������ı������Ͳ�Ҫ�� ǰ�� �� auto//
	gameover = Label::createWithSystemFont("YOU ARE DEATH", "����", 100);
	gameover->setPosition(Point(visibleSize.width / 2, visibleSize.height / 2 + gameover->getContentSize().height));
	gameover->setVisible(false);
	gameover->setColor(Color3B::RED);
	gameover->setTag(112);
	this->addChild(gameover, 1);

	//��Ч��ʼ��//
	SimpleAudioEngine::getInstance()->playEffect("Yumi.mp3");
	 
	//�ص��˵�����(ͼƬ��ť)//
	labelBack = Label::create("Main Menu", "Arial", 30);
	labelBack->setVisible(false);
	itemBack = MenuItemLabel::create(labelBack, CC_CALLBACK_1(GameScene_1::menuBackToMain, this)); //���ְ�ť//
	itemBack->setPosition(Point(visibleSize.width / 2, visibleSize.height / 2 - 110 ));
	itemBack->setAnchorPoint(Point::ANCHOR_MIDDLE_BOTTOM);
	itemBack->setEnabled(false);
	auto menu_1 = Menu::create(itemBack, NULL);
	menu_1->setPosition(Point(0, 0));
	this->addChild(menu_1, 1);

	//��һ��(ͼƬ��ť)//
	NextItem = MenuItemImage::create(
		"Next.png",
		"Next.png",
		CC_CALLBACK_1(GameScene_1::NextScene, this));
	NextItem->setVisible(false);
	NextItem->setEnabled(false);
	menu_2 = Menu::create(NextItem, NULL);
	menu_2->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
	this->addChild(menu_2, 1);

	//������Ϸ(��ť)//
	regame = Label::create("Come on guy", "Arial", 40);
	regame->setVisible(false);
	regameItem = MenuItemLabel::create(regame, CC_CALLBACK_1(GameScene_1::ReScene, this));
	regameItem->setPosition(Point(visibleSize.width / 2 , visibleSize.height / 2 - 50));
	regameItem->setAnchorPoint(Point::ANCHOR_MIDDLE_BOTTOM);
	regameItem->setTag(113);
	regameItem->setEnabled(false);
	auto menu_3 = Menu::create(regameItem, NULL); //�����������淵�ز˵���//
	menu_3->setPosition(Point(0, 0));
	this->addChild(menu_3, 1);
    //��ͣ��ť//
	stop = Label::create("Pause", "Arial", 20);
	stop->setVisible(true);
	stop->setOpacity(70);
	stopItem = MenuItemLabel::create(stop, CC_CALLBACK_1(GameScene_1::Gamestop, this));
	stopItem->setPosition(Point(visibleSize.width - 10, 10));
	stopItem->setAnchorPoint(Point::ANCHOR_BOTTOM_RIGHT);
	stopItem->setTag(114); 
	stopItem->setEnabled(true);
	auto menu_4 = Menu::create(stopItem, NULL);           
	menu_4->setPosition(Point(0, 0));
	this->addChild(menu_4, 1);

	//������ť//
	rebegin = Label::create("Continue", "Arial", 100);               
	rebegin->setVisible(false);
	rebeginItem = MenuItemLabel::create(rebegin, CC_CALLBACK_1(GameScene_1::Gamecontinue, this));
	rebeginItem->setPosition(Point(visibleSize.width / 2, visibleSize.height / 2));
	rebeginItem->setAnchorPoint(Point::ANCHOR_MIDDLE_BOTTOM);
	rebeginItem->setTag(115);
	rebeginItem->setEnabled(false);
	auto menu_5 = Menu::create(rebeginItem, NULL);
	menu_5->setPosition(Point(0, 0));
	this->addChild(menu_5, 1);

	//��������//
	SimpleAudioEngine::getInstance()->playBackgroundMusic(Gamemusic, true);
	//����ͼƬ//
	auto sprite = Sprite::create(GamePicture);
	sprite->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
	sprite->setOpacity(90);
	this->addChild(sprite, 0);
	//��ʱ��//
	nSecond = 0;
	nMinute = 0;
	auto labelTime = Label::createWithSystemFont("Time 00:00", "Arial", 25);
	labelTime->setTag(118);
	labelTime->setPosition(Point(visibleSize.width - 100, visibleSize.height - 28));
	this->addChild(labelTime, 1);

	//���������ߵĳ�ʼ��//
	auto touchListener = EventListenerTouchOneByOne::create();
	touchListener->setSwallowTouches(true);
	touchListener->onTouchBegan = CC_CALLBACK_2(GameScene_1::onTouchBegan, this);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener, this);

	this->schedule(SEL_SCHEDULE(&GameScene_1::TimeCounter), 1);
	this->schedule(SEL_SCHEDULE(&GameScene_1::logic), SPEED);//ˢ�����ʣ�����logic����ĺ�����ѭ���ٶ�//
	return true;
}

void GameScene_1::menuBackToMain(cocos2d::Ref* pSender) //�ص��˵�����//
{
	auto transition_1 = TransitionFade::create(0.5, HelloWorld::createScene());  //���н������Ч���ӳ�0.5s//
	SimpleAudioEngine::getInstance()->playBackgroundMusic(Titlemusic, true);
	Director::getInstance()->replaceScene(transition_1);
}

void GameScene_1::ReScene(cocos2d::Ref* pSender) //���±��ؿ�//
{
	auto transition_2 = TransitionFade::create(1, GameScene_1::createScene()); 
	Director::getInstance()->replaceScene(transition_2);
}

void GameScene_1::NextScene(cocos2d::Ref* pSender)  //��һ��//
{
	auto transition_3 = TransitionFade::create(0.5, GameScene_2::createScene());
	Director::getInstance()->replaceScene(transition_3);
}


void GameScene_1::GamePause()  //��Ϸ��ͣ������//
{
	sHead->nodeSprite->setVisible(false);
	int i;
	for (i = 0; i < allbody.size(); i++)
		allbody.at(i)->nodeSprite->setVisible(false);
	this->pause();
	sFood->nodeSprite->setVisible(false);
}

void GameScene_1::GameResume()
{

	sHead->nodeSprite->setVisible(true);
	int i;
	for (i = 0; i < allbody.size(); i++)
		allbody.at(i)->nodeSprite->setVisible(true);
	sFood->nodeSprite->setVisible(true);
	this->resume();
}

void GameScene_1::Death()
{
	GamePause();
	gameover->setVisible(true);
	regame->setVisible(true);
	regameItem->setEnabled(true);
	labelBack->setVisible(true);
	itemBack->setEnabled(true);
	stop->setVisible(false);
	stopItem->setEnabled(false);
	SimpleAudioEngine::getInstance()->preloadBackgroundMusic(Deathmusic);
	SimpleAudioEngine::getInstance()->playBackgroundMusic(Deathmusic, false);
}

void GameScene_1::GameNextScene()
{
	GamePause();
	stop->setVisible(false);
	stopItem->setEnabled(false);
	NextItem->setVisible(true);
	NextItem->setEnabled(true);
	SimpleAudioEngine::getInstance()->preloadBackgroundMusic(Wintermusic);
	SimpleAudioEngine::getInstance()->playBackgroundMusic(Wintermusic, false);
}

void GameScene_1::Gamestop(cocos2d::Ref* pSender)
{
	GamePause();
	stop->setVisible(false);
	stopItem->setEnabled(false);
	rebegin->setVisible(true);
	rebeginItem->setEnabled(true);
	labelBack->setVisible(true);
	itemBack->setEnabled(true);
}

void GameScene_1::Gamecontinue(cocos2d::Ref* pSender)
{
	rebegin->setVisible(false);
	rebeginItem->setEnabled(false);
	labelBack->setVisible(false);
	itemBack->setEnabled(false);
	stop->setVisible(true);
	stopItem->setEnabled(true);
	GameResume();
}


bool GameScene_1::onTouchBegan(Touch* touch, Event* event)   //̰�����ƶ��㷨//
{
Point touchPoint = touch->getLocation();
int touchRow = ((int)touchPoint.y) / 32;   //ͼƬ������32*32//
int touchCol = ((int)touchPoint.x) / 32;


	if (abs(touchRow - sHead->row) > abs(touchCol - sHead->col))   //�ߴ��ڿ�
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

	else                                               //����ڸ�
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

void GameScene_1::TimeCounter(float dt)
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


//��Ϸ�߼����ѵ㣩//
void GameScene_1::logic(float t)
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
		sn->nodeSprite->setPosition(Point(sn->col * 32, sn->row * 32));
	}



	switch (sHead->dir)    //ͷ����������//
	{
	case DIR_DEF::UP:
		sHead->row++;
		//ͷ����ת//
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
	//�߽紩Խ//
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


	sHead->nodeSprite->setPosition(Point(sHead->col * 32, sHead->row * 32)); //���¶�λ//

	if (sHead->col == sFood->col && sHead->row == sFood->row)
	{                                                             //��ʳ����//
		SimpleAudioEngine::getInstance()->playEffect("Yumi.mp3");

		//ʳ������//
		sFood->row = rand() % 20;
		sFood->col = rand() % 30;
		sFood->nodeSprite->setPosition(Point(sFood->col * 32, sFood->row * 32));

		//��������//
		this->snake_score += 1;
		auto label = (Label *)this->getChildByTag(110);
		char strscore[20];
		sprintf(strscore, "Score: %d", snake_score);
		label->setString(strscore);


		//��������//
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
		nodeSprite->setPosition(Point(snakeNode->col * 32, snakeNode->row * 32));
		nodeSprite->setAnchorPoint(Point::ANCHOR_BOTTOM_LEFT);
		nodeSprite->setVisible(true);                                //�������ذ�ť//
		snakeNode->nodeSprite = nodeSprite;
		this->addChild(nodeSprite, 1);

		allbody.pushBack(snakeNode);
	}

	sHead->nodeSprite->setPosition(Point(sHead->col * 32, sHead->row * 32)); //���¶�λ//

	if (snake_score == game_target)                  //�����ﵽ,������һ��//
       GameNextScene();

	if (allbody.size()>1)                                //�����ж�֮�Լ���ײ//
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
		{
			Death();
		}
	}



}
