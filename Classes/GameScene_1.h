#include "cocos2d.h"
#include "HelloWorldScene.h"

USING_NS_CC;

class GameScene_1 : public cocos2d::Layer
{
public:

	int snake_score;
	int game_target;

	virtual bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
	void logic(float t);
	void GamePause();                           //暂停函数//
	void GameResume();                         //继续函数//

	static cocos2d::Scene* createScene();
	virtual bool init();
	void NextScene(cocos2d::Ref* pSender);     //下一关的场景//
	void ReScene(cocos2d::Ref* pSender);
	void menuBackToMain(cocos2d::Ref*pSender);

	void Death();
	void Gamestop(cocos2d::Ref* pSender);
	void Gamecontinue(cocos2d::Ref* pSender);
	void GameNextScene();

	void TimeCounter(float dt);
	int nSecond;
	int nMinute;

	Label* gameover;

	Label* regame;
	MenuItemLabel* regameItem;

	Menu* menu_2;
	MenuItemImage*  NextItem;

	Label* stop;
	MenuItemLabel* stopItem;

	Label* rebegin;
	MenuItemLabel* rebeginItem;

	Label* labelBack;
	MenuItemLabel* itemBack;

	CREATE_FUNC(GameScene_1);

protected:
	SnakeNode* sHead;
	SnakeNode* sFood;
	cocos2d::Vector<SnakeNode*> allbody;      //Vector容器实在是太棒了//太好用了

};

