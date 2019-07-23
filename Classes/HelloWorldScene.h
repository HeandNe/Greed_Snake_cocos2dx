#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"

#define  Titlemusic "Titlebgm.mp3"
#define  Wintermusic "Winbgm.mp3"
#define  Deathmusic "Deadbgm.mp3"
#define  Gamemusic "Gamebgm.mp3"
#define  GameRankMusic "Rankbgm.mp3"
#define  GamePicture "GamePP.png"
#define  GameRankPicture "Rank.png"
//static int TotalTime[6];

class HelloWorld : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();
    void menuCloseCallback(cocos2d::Ref* pSender);
    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);
};

//±Í«©//
typedef enum{
	START = 10,
	HELP,
	EXIT,
	STIME
}TAG_MENU;


typedef enum {
	UP = 1,
	DOWN,
	LEFT,
	RIGHT
}DIR_DEF;

class SnakeNode :public cocos2d::Ref
{
public:
	int row;
	int col;
	int dir;
	SnakeNode* preNode;
	cocos2d::Sprite* nodeSprite;
};





#endif // __HELLOWORLD_SCENE_H__
