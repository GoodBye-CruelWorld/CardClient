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

	void backEvent(Ref*pSender, TouchEventType type);//返回
	//动画
	void cummonAnime(int id);
	void setState(bool state);

	void addParticle(Node*node);
	GameSocket *_socket;
	int _mode;
	User *_user;
private:
	
	int _state;
	Sprite *_box;
	Label * _moneyLabel;
	Sprite *_bg;
	Sprite *_frame;
	Sprite *_modeImg;
	Button *_drawBtn;
	Sprite *_arrow;



	//卡牌数组，储存每个职业每个种类的卡的数量
	vector<CardArray> _cardArray;

	void setMoneyLabel(int Money);
	void DrawCard();//抽牌
};

