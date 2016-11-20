#pragma once
#include"HelloWorldScene.h"

#include"BoardCard.h"
#include"Users.h"
//_heroID 0,1
//_cardTypeID 0,1,2,3 ���,����,����,װ��
//_cardOrder ˳��

class Book :public Layer
{
public:
	virtual void onEnter() override;
private:
	GameSocket *_socket;
	int _mode;
public:
	//�����ܹ�����

	void open();
	void close();
	void turnPage(int delta);
	void scroll(bool direction);//true=�ϻ�,false=�»�
	void upMEQuality(int id);
	void leftEvent(Ref* sender, TouchEventType type);
	void rightEvent(Ref* sender, TouchEventType type);
	//ʹħͼ������

	void addNewME();  //����ʹħ
	void displayME(int num); //չʾʹħ
	void undisplay();
	void describeEvent();
	void showText(int num);
	void addStone(int MEID, int num);
	Sprite*createMEPhote(int num);
	void introEvent(Ref* sender, TouchEventType type);
	void attriEvent(Ref* sender, TouchEventType type);
	void skillEvent(Ref* sender, TouchEventType type);
	void returnEvent(Ref* sender, TouchEventType type);


	void initME();


	void MEIntro();
	void MEAttri();
	void MESkill();

	//����ͼ��״̬
	void setState(int state);

	//����
	void cummonAnime(int id);

public:
	int curPage;
	Sprite *_line;
	Sprite *_scroll;
	Sprite *_scrollline;
	bool _scr;
	//ʹħͼ��
	std::string _describe;
	Sprite* _bgs[3], *_frames[8], *_frames2[34], *_frames3[20];
	BoardCard *_BoardCards[8];
	Button *_upbuttons[8];
	Sprite* _MEs[8];
	Sprite* _stars[8][3];
	int _selectedME;
	Sprite*_MEcard;
	Sprite *_MEdetail;
	Label*_desLb;
	Button* _MEButtons[8];
	Label *_stoneLbs[8];
	int _stoneNeeds[4];

	//����ͼ��
	Sprite*_items[34];
	Label*_reactions[20];
	bool _own[34];
	bool _own3[20];
	bool _guide[2];
	bool _state;
	bool _new;
	bool _initME;
};