#pragma once
#include"HelloWorldScene.h"



#include"GameBoard.h"
#include"Tool2D.h"
#include"Battle.h"
#define CL(__className__) [](){ return __className__::create();}

class MyScene :public cocos2d::Scene
{



};


class MyLayer2 :public Layer
{
public:
	virtual void onEnter() override;
	void backToMainEvent(Ref*pSender, TouchEventType type); //����������
	void backToMain();
	CREATE_FUNC(MyLayer2);
	GameSocket *_socket;
	bool _mode;
	bool _firstHand;
private:
	GameBoard *_gameBoard;
	BattleTool2D *_tool;
	CBattle *_battles[2];

	int _battleID[2];
	bool _battleState[2];
};
