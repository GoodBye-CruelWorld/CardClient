#pragma once
#include"HelloWorldScene.h"

#include"BoardCard.h"
#include"Users.h"
//_heroID 0,1
//_cardTypeID 0,1,2,3 随从,法术,武器,装备
//_cardOrder 顺序

struct CardArrayInBook
{
	int RoleID;
	int CardNumber[4];
};

#define CARD_SIZE 3
class Book :public Layer
{
public:
	virtual void onEnter() override;

	GameSocket *_socket;
	int _mode;
public:
	//基本架构功能

	void open();
	void close();
	void turnPage(int delta);
	void scroll(bool direction);//true=上滑,false=下滑 order

	void leftEvent(Ref* sender, TouchEventType type);
	void rightEvent(Ref* sender, TouchEventType type);
	//使魔图鉴功能

	//void addNewCard();  //增加使魔
	void displayCard(int num); //展示使魔
	void undisplay();
	void initCard();




	//设置图鉴状态
	void setState(int state);

	//组卡功能
	void initCardArray();
	void createCardSet();
	void removeCard();
	void addCard();
	void chooseCardSet(int num);
public:
	/**@onTouchBegan:当触摸开始
	*/
	bool onTouchBegan(Touch* touch, Event* event);
	/**@onTouchBegan:当触摸移动
	*/
	void onTouchMoved(Touch* touch, Event* event);
	/**@onTouchBegan:当触摸结束
	*/
	void onTouchEnded(Touch* touch, Event* event);

	bool collisionCheck(Point p, Node *node);
public:
	int curPage;
	Sprite *_line;
	Sprite *_scroll;
	Sprite *_scrollline;
	bool _scr;
	//使魔图鉴
	std::string _describe;
	Sprite* _bgs[3], *_frames[CARD_SIZE];
	Sprite *_cards[CARD_SIZE];
	Button *_upbuttons[CARD_SIZE];


	BoardCard * _cardSel;
	Sprite *_carddetail;
	Label*_desLb;

	Label *_stoneLbs[CARD_SIZE];


	bool _state;  //判断点击效果
	bool _initCard;
private:
	int convertToCardID(int num);
	std::vector<CCard> _cardsData;
	int _curSize;
	int _selectedCard;
	int _selCardSet;
	User *_user;
	Button *_createSet;
	Button *_changeSet;
	vector<CardArrayInBook> _cardArray;
	Sprite* _cardSetBar;
	Vector<Sprite*> _cardSets;
	Vector<Sprite*> _cardsInSet;
	int _curRole;
	int _curNum;
	int _curType;
	int _curSet;
	//与卡组有关的函数
public:
	void createCardArray(string cardName, int roleID);		//创建卡组，返回cardArrayID
	void addCardArray();						//增加卡组，添加到数据库
	void changeCardArray();
	bool delCardArray();						//删除卡组
	int getCardArrayNumber();								//获得所有的卡组数量

	bool addCardintoArray(int cardID);		//在卡组中增加卡牌
	bool delCardofArray( int cardID);		//删除卡牌

	bool addUserCard(int ID);								//增加卡牌

	void newSetEvent(Ref*pSender, TouchEventType type);
	void adjustCardSets();
	Button* _newSetBn; //创建新卡组
	Button* _completeBn;//完成卡组
};