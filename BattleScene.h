#pragma once
#include"HelloWorldScene.h"



#include"GameBoard.h"
#include"Tool2D.h"
#include"Battle.h"
#include"Command.h"
#include"Users.h"
#define CL(__className__) [](){ return __className__::create();}

class MyScene :public cocos2d::Scene
{



};


class MyLayer2 :public Layer
{
public:
	virtual void onEnter() override;
	void backToMainEvent(Ref*pSender, TouchEventType type); //返回主界面
	void backToMain();
	CREATE_FUNC(MyLayer2);
	GameSocket *_socket;
	bool _mode;
	User *_user;
	bool _boss;
	bool _firstHand;
private:
	GameBoard *_gameBoard;
	BattleTool2D *_tool;
	CBattle *_battles[3];
	Command *_cmd;
	int _battleID[2];
	bool _battleState[2];
};


