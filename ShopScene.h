#pragma once
#include"HelloWorldScene.h"
#include"card.h"
#include"boardcard.h"
#include"Users.h"

#include<vector>
using namespace std;

//CardNumber��˳��ֱ�Ϊ��ӣ�������������װ��
struct CardArray
{
	int RoleID;
	int CardNumber[4];
};

class ShopLayer :public Layer
{
public:
	virtual void onEnter() override;
	//ģʽѡ��
public:
	//start game
	
	void startDrawEvent(Ref*pSender, TouchEventType type);//����


	//����
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
	//�������飬����ÿ��ְҵÿ������Ŀ�������
	vector<CardArray> _cardArray;

	void setMoneyLabel(int Money);
	void DrawCard();//����
};