#include "SimpleAudioEngine.h"
#include "ui\CocosGUI.h"
#include "cocos2d.h"
#include "GameScore.h"
#include "AudioEngine.h"

using namespace std;
using namespace CocosDenshion;
USING_NS_CC;

char GameScore::minandscon[6] = { '0', '0', ':', '0', '0', '\0' }; //时间初始化
string GameScore::playername = "HHH"; //名字初始化
int GameScore::playerscore = 250; //分数初始化

Scene* GameScore::createScene()
{
	auto scene = Scene::create();
	auto layer = GameScore::create();
	scene->addChild(layer);
	return scene;
}
bool GameScore::init()
{
	if (!Layer::init())
	{
		return false;
	}

	Size visibleSize = Director::getInstance()->getVisibleSize();

	auto labelTime = Label::createWithSystemFont("Time", "Arial", 25);
	labelTime->setTag(120);
	labelTime->setPosition(Point(visibleSize.width / 2 - 100, visibleSize.height / 2 + 175));
	this->addChild(labelTime, 1);

	auto labelScore = Label::createWithSystemFont("Apple", "Arial", 25);
	labelScore->setTag(121);
	labelScore->setPosition(Point(visibleSize.width / 2 + 100, visibleSize.height / 2 + 175));
	this->addChild(labelScore, 1);

	auto labelName = Label::createWithSystemFont("Name", "Arial", 25);
	labelName->setTag(122);
	labelName->setPosition(Point(visibleSize.width / 4, visibleSize.height / 2 + 175));
	this->addChild(labelName, 1);

	if (250 != playerscore)
	{
		auto labelTitleName = Label::createWithSystemFont("YOUR SCORE", "Arial", 30);
		labelTitleName->setTag(130);
		labelTitleName->setColor(Color3B::YELLOW);
		labelTitleName->setPosition(Point(visibleSize.width - 120, visibleSize.height - 130));
		this->addChild(labelTitleName, 1);

		auto labelTime_T = Label::createWithSystemFont("POI", "Arial", 25);
		labelTime_T->setTag(123);
		labelTime_T->setColor(Color3B::YELLOW);
		labelTime_T->setPosition(Point(visibleSize.width - 120, visibleSize.height - 250));
		this->addChild(labelTime_T, 1);
		auto label_2 = (Label *)this->getChildByTag(123);
		char strtime_2[16];
		sprintf(strtime_2, "%s", minandscon);
		label_2->setString(strtime_2);

		auto labelScore_T = Label::createWithSystemFont("POI", "Arial", 25);
		labelScore_T->setTag(124);
		labelScore_T->setColor(Color3B::YELLOW);
		labelScore_T->setPosition(Point(visibleSize.width - 120, visibleSize.height - 300));
		this->addChild(labelScore_T, 1);
		auto label_1 = (Label *)this->getChildByTag(124);
		char strtime_1[12];
		sprintf(strtime_1, "%d", playerscore);
		label_1->setString(strtime_1);

		if (playername.empty())
			playername = "NOBODY";

		auto labelName_T = Label::createWithSystemFont(playername, "Arial", 25);
		labelName_T->setTag(125);
		labelName_T->setColor(Color3B::YELLOW);
		labelName_T->setPosition(Point(visibleSize.width - 120, visibleSize.height - 200));
		this->addChild(labelName_T, 1);
	}
	auto labelBack = Label::create("Main Menu", "Marker Felt", 30);
	auto itemBack = MenuItemLabel::create(labelBack, CC_CALLBACK_1(GameScore::menuBackToMain, this));
	itemBack->setPosition(Point(visibleSize.width / 2, visibleSize.height / 2 - 250));

	auto menu = Menu::create(itemBack, NULL);
	menu->setPosition(Point(0, 0));
	this->addChild(menu, 1);

	//背景音乐//
	SimpleAudioEngine::getInstance()->playBackgroundMusic(GameRankMusic, true);
	//背景图片//
	auto sprite = Sprite::create(GameRankPicture);
	sprite->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
	sprite->setOpacity(95);
	this->addChild(sprite, 0);

	//写入当前用户的游戏记录并且排序//
	WriteIntoFile("RANK.txt", "RANKCO.txt");
	ReadFromFile(name, mark, time, "RANK.txt");
	Dowmrank(name, mark, time);
	WriteIntoFile(name, mark, time, "RANK.txt");

	//客户端初试或链接服务器失败，则启动本地记录//
	if (false == ClientInit())  
	{
		ShowRank(name, mark, time);
		auto labelTitle = Label::createWithSystemFont("Player Rank (local)", "Arial", 50);
		labelTitle->setTag(199);
		labelTitle->setPosition(Point(visibleSize.width / 2, visibleSize.height - 55));
		this->addChild(labelTitle, 1);
	}
	else
	{
		ReadFromFile(name, mark, time, "C_RealPlayer.txt");
		Dowmrank(name, mark, time);
		ShowRank(name, mark, time);
		auto labelTitle = Label::createWithSystemFont("Player Rank (server)", "Arial", 50);
		labelTitle->setTag(199);
		labelTitle->setPosition(Point(visibleSize.width / 2, visibleSize.height - 55));
		this->addChild(labelTitle, 1);
	}
	return true;
}

//从关卡中获得时间信息//
void GameScore::GetyouScore_INT(const char s_time[4], const int s_score) 
{
	GameScore::minandscon[0] = *(s_time + 0);
	GameScore::minandscon[1] = *(s_time + 1);
	GameScore::minandscon[2] = ':';
	GameScore::minandscon[3] = *(s_time + 2);
	GameScore::minandscon[4] = *(s_time + 3);
	GameScore::minandscon[5] = '\0';
	GameScore::playerscore = s_score;
}

void GameScore::menuBackToMain(cocos2d::Ref* pSender)
{
	auto transition_1 = TransitionFade::create(0.5, HelloWorld::createScene());
	SimpleAudioEngine::getInstance()->playBackgroundMusic(Titlemusic, true);
	Director::getInstance()->replaceScene(transition_1);
}

//将分数从整型变成字符//
void GameScore::IntChangeChar(char * tr)                  
{
	*tr = (playerscore / 100) + '0';
	*(tr + 1) = ((playerscore / 10) - (playerscore / 100) * 10) + '0';
	*(tr + 2) = (playerscore - (playerscore / 10) * 10) + '0';
	*(tr + 3) = '\0';
}

void GameScore::WriteIntoFile(char * filename_1, char * filename_2)
{
	ifstream preFile(filename_1); //读取文件信息//
	char buffer[101];
	if (!preFile.is_open())
	{
		cout << "Error opening file";
		return;
	}
	ofstream preFileCo(filename_2, ios::out);
	if (!preFileCo.is_open())
		return;
	while (!preFile.eof())
	{
		preFile.getline(buffer, 100);
		preFileCo << buffer << "\n";

	}
	preFileCo.close();
	preFile.close();

	ifstream preFileCo_2(filename_2);
	if (!preFileCo_2.is_open())
		return;
	char buffer_2[101];

	//app是追加，out是重新创建并写入到文件//
	ofstream File_player(filename_1, ios::out);
	if (!File_player.is_open())
		return;
	char c_score[4];
	IntChangeChar(c_score);
	File_player << playername << "   " << minandscon << "     " << c_score << "\n";
	while (!preFileCo_2.eof())
	{
		preFileCo_2.getline(buffer_2, 100);
		File_player << buffer_2 << "\n";

	}
	preFileCo_2.close();
	File_player.close();
}

void  GameScore::WriteIntoFile(char N[][12], char M[][4], char T[][6], char * filename)
{
	ofstream File_A(filename, ios::out);
	if (!File_A.is_open())
		return;
	int i;
	for (i = 0; (i < Max_rank) && (0 != M[i][0]); i++)
		File_A << N[i] << "   " << T[i] << "     " << M[i] << "\n";
	File_A.close();
}

//从文件中分离出名字（char）和分数（char） 和时间（char）//
void GameScore::Getsome(char * all, char * lett, char * num, char * tim)   
{
	int i, k, p;
	p = 0;
	for (i = 0, k = 0; *(all + i) != 0; i++)
	{
		if (' ' != *(all + i))
		{
			*(lett + p++) = *(all + i);
		}
		else
			k += 1;
		if (k >= 3)
			break;
	}
	*(lett + p) = '\0';

	for (k = 0, p = 0; *(all + i) != 0; i++)
	{
		if (' ' != *(all + i))
		{
			*(tim + p++) = *(all + i);
		}
		else
			k += 1;
		if (k >= 5)
			break;
	}
	*(tim + p) = '\0';

	for (p = 0; (p < 3) && (*(all + i) != 0); i++)
	{
		if (' ' != *(all + i))
		{
			if (*(all + i) >= '0' && *(all + i) <= '9')
			{
				*(num + p++) = *(all + i);
			}
		}

	}
	*(num + p) = '\0';
}

//从文件中读取游戏记录//
void GameScore::ReadFromFile(char N[][12], char M[][4], char T[][6], char * filename)     
{
	ifstream examplein(filename);  //读取文件信息//
	char buffer[256];
	int i;
	if (!examplein.is_open())
	{
		cout << "Error opening file";
		return;
	}
	for (i = 0; (!examplein.eof()) && (i < Max_rank); i++)
	{
		examplein.getline(buffer, 100);
		Getsome(buffer, N[i], M[i], T[i]);
	}

	if (i <= Max_rank)
	{
		while (i < Max_rank)
		{
			N[i][0] = '\0';
			M[i][0] = '\0';
			T[i][0] = '\0';
			i++;
		}
	}
}

void GameScore::ShowRank(char N[][12], char M[][4], char T[][6])
{
	Size visibleSize = Director::getInstance()->getVisibleSize();
	int i;
	for (i = 0; (i < Show_rank) && (0 != M[i][0]); i++)
	{
		auto labelTime_T = Label::createWithSystemFont(T[i], "Arial", 25);
		labelTime_T->setTag(126 + i);
		labelTime_T->setPosition(Point(visibleSize.width / 2 - 100, visibleSize.height / 2 + 125 - 50 * i));
		if (i == 0)
			labelTime_T->setColor(Color3B::RED);
		this->addChild(labelTime_T, 1);

		auto labelScore_T = Label::createWithSystemFont(M[i], "Arial", 25);
		labelScore_T->setTag(127 + i);
		labelScore_T->setPosition(Point(visibleSize.width / 2 + 100, visibleSize.height / 2 + 125 - 50 * i));
		if (i == 0)
			labelScore_T->setColor(Color3B::RED);
		this->addChild(labelScore_T, 1);

		auto labelName_T = Label::createWithSystemFont(N[i], "Arial", 25);
		labelName_T->setTag(128 + i);
		labelName_T->setPosition(Point(visibleSize.width / 4, visibleSize.height / 2 + 125 - 50 * i));
		if (i == 0)
			labelName_T->setColor(Color3B::RED);
		this->addChild(labelName_T, 1);
	}

}

//字符数字转成整型//
void GameScore::charChangeint(char M[][4], int mark[])                     
{
	int i;
	int k = 0;
	for (i = 0; (i<Max_rank) && (0 != M[i][0]); i++)
		mark[i] = (M[i][0] - '0') * 100 + (M[i][1] - '0') * 10 + (M[i][2] - '0');
	if (i < Max_rank)
	while (i < Max_rank)
		mark[i++] = 0;
}

//将玩家排名降序排列//
void GameScore::Dowmrank(char N[][12], char M[][4], char T[][6])
{
	char t_1[12], t_2[4], t_3[6];
	int mark[Max_rank];
	int i, j;
	int temp;
	charChangeint(M, mark);

	for (j = 0; j < Max_rank - 1; j++)
	{
		for (i = 0; i < Max_rank - 1 - j; i++)
		{
			if (mark[i] < mark[i + 1])
			{

				strcpy_s(t_1, N[i + 1]);
				strcpy_s(N[i + 1], N[i]);
				strcpy_s(N[i], t_1);

				strcpy_s(t_2, M[i + 1]);
				strcpy_s(M[i + 1], M[i]);
				strcpy_s(M[i], t_2);

				strcpy_s(t_3, T[i + 1]);
				strcpy_s(T[i + 1], T[i]);
				strcpy_s(T[i], t_3);

				temp = mark[i];
				mark[i] = mark[i + 1];
				mark[i + 1] = temp;
			}
		}
	}
}
//客户端初试化//
bool GameScore::ClientInit()
{
	WORD sockVersion = MAKEWORD(2, 2);
	WSADATA data;
	if (WSAStartup(sockVersion, &data) != 0)
	{
		return false; //无法建立则退出//
	}

	SOCKET sclient = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sclient == INVALID_SOCKET)
	{
		return false;//无法建立则退出//
	}

	sockaddr_in serAddr;
	serAddr.sin_family = AF_INET;
	serAddr.sin_port = htons(8000);//端口号为8000//

	//127.x.x.x,不管是都少，最后还是发送127.0.0.1。其他的都是地址存在理论上//
	//将IP地址的点分十进制转换成二进制，并写入Socket//
	inet_pton(AF_INET, "127.0.0.1", (void*)&serAddr.sin_addr.S_un.S_addr);
	if (connect(sclient, (sockaddr *)&serAddr, sizeof(serAddr)) == SOCKET_ERROR) //检查是否连接上服务器//
	{
		closesocket(sclient);
		return false;
	}
	else
	{
		if (250 != playerscore)
		{
			char Gamedate_1[40] = { '\0' };
			const char * nameplayer = playername.c_str();
			strcpy_s(Gamedate_1, nameplayer);
			strcat_s(Gamedate_1, "   ");
			strcat_s(Gamedate_1, minandscon);

			char c_score[4];
			IntChangeChar(c_score);
			strcat_s(Gamedate_1, "     ");
			strcat_s(Gamedate_1, c_score);

			send(sclient, Gamedate_1, strlen(Gamedate_1), 0);
		}
	}
	//下面是接受排名文件//
	unsigned long long file_size = 0;
	recv(sclient, (char*)&file_size, sizeof(unsigned long long) + 1, NULL);

	DWORD dwNumberOfBytesRecv = 0;
	DWORD dwCountOfBytesRecv = 0;
	char Buffer_2[1024];
	HANDLE hFile;
	hFile = CreateFile(_T("C_RealPlayer.txt"), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	do
	{
		//前面是只有::是表面全局作用域
		dwNumberOfBytesRecv = ::recv(sclient, Buffer_2, sizeof(Buffer_2), 0);
		::WriteFile(hFile, Buffer_2, dwNumberOfBytesRecv, &dwNumberOfBytesRecv, NULL);  
		dwCountOfBytesRecv += dwNumberOfBytesRecv;
	} while (file_size - dwCountOfBytesRecv);

	CloseHandle(hFile);

}


