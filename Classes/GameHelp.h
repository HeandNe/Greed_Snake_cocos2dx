#include "cocos2d.h"

class GameHelp : public cocos2d::Layer
{
public:
	static cocos2d::Scene* createScene();
	virtual bool init();
	void menuBackToMain(cocos2d::Ref*pSender);
	CREATE_FUNC(GameHelp);
};