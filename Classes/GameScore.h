#include"stdlib.h"
#include"iostream"
#include"string"
#include "cocos2d.h"
#include "HelloWorldScene.h"
#include <tchar.h>
#include<WINSOCK2.H>  
#include<STDIO.H>  
#include<cstring>  
#include<WS2tcpip.h>
#include <fstream>
#pragma comment(lib, "ws2_32.lib")  

#define Max_rank 50
#define Show_rank 7

USING_NS_CC;
using namespace std;
class GameScore : public cocos2d::Layer
{
public:
	static cocos2d::Scene* createScene();
	virtual bool init();
	static void GetyouScore_INT(const char s_time[4], const int s_score);
	void menuBackToMain(cocos2d::Ref*pSender);

	static char minandscon[6];
	static int playerscore;
	static string playername;

	void WriteIntoFile(char * filename_1, char * filename_2);
	void WriteIntoFile(char N[][12], char M[][4], char T[][6], char * filename);

	void IntChangeChar(char * tr);

	void ReadFromFile(char N[][12], char M[][4], char T[][6], char * filename);
	void Getsome(char * all, char * lett, char * num, char * tim);
	char name[Max_rank][12];
	char mark[Max_rank][4];
	char time[Max_rank][6];

	void ShowRank(char N[][12], char M[][4], char T[][6]);
	void charChangeint(char M[][4], int mark[]);
	void Dowmrank(char N[][12], char M[][4], char T[][6]);

	bool ClientInit();

	CREATE_FUNC(GameScore);
};