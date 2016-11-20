#pragma once
#include"HelloWorldScene.h"

#include"BoardCard.h"
#include"Users.h"
//_heroID 0,1
//_cardTypeID 0,1,2,3 ���,����,����,װ��
//_cardOrder ˳��

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
	//�����ܹ�����

	void open();
	void close();
	void turnPage(int delta);
	void scroll(bool direction);//true=�ϻ�,false=�»� order

	void leftEvent(Ref* sender, TouchEventType type);
	void rightEvent(Ref* sender, TouchEventType type);
	//ʹħͼ������

	//void addNewCard();  //����ʹħ
	void displayCard(int num); //չʾʹħ
	void undisplay();
	void initCard();




	//����ͼ��״̬
	void setState(int state);

	//�鿨����
	void initCardArray();
	void createCardSet();
	void removeCard();
	void addCard();
	void chooseCardSet(int num);
public:
	/**@onTouchBegan:��������ʼ
	*/
	bool onTouchBegan(Touch* touch, Event* event);
	/**@onTouchBegan:�������ƶ�
	*/
	void onTouchMoved(Touch* touch, Event* event);
	/**@onTouchBegan:����������
	*/
	void onTouchEnded(Touch* touch, Event* event);

	bool collisionCheck(Point p, Node *node);
public:
	int curPage;
	Sprite *_line;
	Sprite *_scroll;
	Sprite *_scrollline;
	bool _scr;
	//ʹħͼ��
	std::string _describe;
	Sprite* _bgs[3], *_frames[CARD_SIZE];
	Sprite *_cards[CARD_SIZE];
	Button *_upbuttons[CARD_SIZE];


	BoardCard * _cardSel;
	Sprite *_carddetail;
	Label*_desLb;

	Label *_stoneLbs[CARD_SIZE];


	bool _state;  //�жϵ��Ч��
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
	//�뿨���йصĺ���
public:
	void createCardArray(string cardName, int roleID);		//�������飬����cardArrayID
	void addCardArray();						//���ӿ��飬��ӵ����ݿ�
	void changeCardArray();
	bool delCardArray();						//ɾ������
	int getCardArrayNumber();								//������еĿ�������

	bool addCardintoArray(int cardID);		//�ڿ��������ӿ���
	bool delCardofArray( int cardID);		//ɾ������

	bool addUserCard(int ID);								//���ӿ���

	void newSetEvent(Ref*pSender, TouchEventType type);
	void adjustCardSets();
	Button* _newSetBn; //�����¿���
	Button* _completeBn;//��ɿ���
};