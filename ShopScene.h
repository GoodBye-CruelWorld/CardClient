#pragma once
#include"HelloWorldScene.h"
#include"card.h"
#include"boardcard.h"
#include"Users.h"

#include<vector>
using namespace std;

//CardNumber按顺序分别为随从，法术，武器，装备
struct CardArray
{
	int RoleID;
	int CardNumber[4];
};

class ShopLayer :public Layer
{
public:
	virtual void onEnter() override;
	//模式选择
public:
	//start game
	
	void startDrawEvent(Ref*pSender, TouchEventType type);//抽牌


	//动画
	void cummonAnime(int id);



	GameSocket *_socket;
	int _mode;
private:
	
	int _gameState;
	
	Label * _moneyLabel;
	Sprite *_bg;
	Sprite *_frame;
	Sprite *_modeImg;
	Button *_drawBtn;
	Sprite *_arrow;


	User *_user;
	//卡牌数组，储存每个职业每个种类的卡的数量
	vector<CardArray> _cardArray;

	void setMoneyLabel(int Money);
	void DrawCard();//抽牌
};