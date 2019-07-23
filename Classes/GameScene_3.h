#include"stdlib.h"
#include"iostream"
#include"string"
#include "cocos2d.h"
#include "HelloWorldScene.h"

USING_NS_CC;
using namespace std;
class GameScene_3 : public cocos2d::Layer
{
public:
	
	int snake_score;    
	int game_target;
	int trap_row_1;
	int trap_col_1;
	int trap_row_2;
	int trap_col_2;

	int door_row_1;
	int door_col_1;
	int door_row_2;
	int door_col_2;

	int flag_2;

	virtual bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
	void logic(float t);	
	void GamePause();     //暂停函数//
	void GameResume();

	static cocos2d::Scene* createScene();
	virtual bool init();
	//小函数//
	void NextScene(cocos2d::Ref* pSender);     //下一关的场景//
	void menuBackToMain(cocos2d::Ref*pSender);
	//大函数//
	void GameEnd();
	void Gamestop(cocos2d::Ref* pSender);
	void Gamecontinue(cocos2d::Ref* pSender);
	void GameNextScene();
	void Gamedonwor(int * Fr, int * Fc, int * Tc_1, int * Tr_1, int * Tr_2, int * Tc_2);

	void TimeCounter(float dt);
	int nSecond;
	int nMinute;
    
	Menu* menu_2;
	MenuItemImage*  NextItem;

	Label* stop;
	MenuItemLabel* stopItem;

	Label* rebegin;
	MenuItemLabel* rebeginItem;

	Label* labelBack;
	MenuItemLabel* itemBack;

	Sprite * Trap_1; 
	Sprite * Trap_2;

	Sprite * Door_1;
	Sprite * Door_2;

	TextFieldTTF *textName;

	CREATE_FUNC(GameScene_3);
	
protected:
	SnakeNode* sHead;
	SnakeNode* sFood;
	cocos2d::Vector<SnakeNode*> allbody;  
	string *m_pInputText;//获取输入的文字
	
};


