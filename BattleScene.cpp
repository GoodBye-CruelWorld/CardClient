
#include"BattleScene.h"
#include"MainScene.h"
#include "renderer/CCRenderer.h"
#include "renderer/CCCustomCommand.h"
#include "renderer/CCGroupCommand.h"


//
////
////���㰴˳�����ã������巵�أ����أ�ǰ���Ļص�����
////������
////
static std::function<Layer*()> createFunctions[] =
{
	//CL(MyLayer1),
	CL(MyLayer2)
};
//
static int sceneIdx = 0;//order of scene
#define MAX_LAYER (sizeof(createFunctions)/sizeof(createFunctions[0]));
//
static Layer*next()
{
	sceneIdx++;
	sceneIdx = sceneIdx%MAX_LAYER;

	auto layer = (createFunctions[sceneIdx])();
	return layer;
}

static Layer*restart()
{
	auto layer = (createFunctions[sceneIdx])();
	return layer;
}

static Layer*back()
{
	sceneIdx--;
	int total = MAX_LAYER;
	sceneIdx = (sceneIdx + total) % MAX_LAYER;

	auto layer = (createFunctions[sceneIdx])();
	return layer;
}


void MyLayer2::onEnter()
{
	Layer::onEnter();
	auto s = Director::getInstance()->getWinSize();
	
	//test
	//SimpleAudioEngine::getInstance()->playBackgroundMusic("bgm/battle.mp3",true);
	//SimpleAudioEngine::getInstance()->playEffect("bgm/battle.mp3",true);
	

	/*��ʼ������*/
	_gameBoard = GameBoard::getNewInstance();
	_gameBoard->setPosition(Vec2(s.width / 2, s.height / 2));
	addChild(_gameBoard);
	/*��ʼ���������*/

	/*��ʼ���ƿ�*/

	int armorId[30];
	int warriorId[30];
	for (int i = 0; i < 30; i++)
	{
		armorId[i] = rand() % 8 + 10000;
		warriorId[i] = rand() % 8;
	}
	armorId[29] = 0;
	armorId[29] = 0;
	armorId[29] = 0;
	armorId[29] = 0;
	armorId[29] = 0;
	warriorId[29] = 2000;
	warriorId[28] = 2001;
	warriorId[27] = 3000;
	warriorId[26] = 3001;
	warriorId[25] = 1002;
	/*��ʼ���ƿ����*/

	/*�Ⱥ���˳��*/
	bool firstHand[2];
	firstHand[0] = false;
	firstHand[1] = true;
	if (_firstHand)
	{
		firstHand[0] = true;
		firstHand[1] = false;
	}

	/*��ʼ��Battle*/
	for (int i = 0; i < 2; i++)
	{
		if (i==0)
			_battles[i] = new CBattle(_gameBoard,warriorId, i,_socket,_mode,firstHand[i]);
		else
			_battles[i] = new CBattle(_gameBoard, armorId, i, _socket, _mode, firstHand[i]);
		this->addChild(_battles[i]);	
	}

	_battles[2] = new CBattle(_gameBoard, warriorId, 2, _socket, _mode, firstHand[1]);
	_battles[2]->setEnemy(_battles[1]);
	_battles[2]->setWild(_battles[0]);
	_battles[2]->addWild();
	this->addChild(_battles[2]);
	_battles[0]->setEnemy(_battles[1]);
	_battles[1]->setEnemy(_battles[0]);
	_battles[0]->setWild(_battles[2]);
	_battles[1]->setWild(_battles[2]);
	_battles[0]->gameStart();
	_battles[1]->gameStart();

	
	/*CCard card;
	card.cardCreate(10001);
	card.set_pos(0);
	_battles[2]->_cardPool[POOL_BATTLE].push_back(card); */
	
	//_gameBoard->addCard(card, POOL_MONSTER, 0, 0, 0);

	/*��ʼ��Command*/

	Command::getInstance()->setBattles(_battles[0], _battles[1]);

	/*��ʼ��tool2D*/
	_tool = new BattleTool2D(_gameBoard, _battles[0], _battles[1],_battles[2]);
	addChild(_tool);

	/*���ذ�ť*/
	auto backBn = Button::create(s_png_battle_exit);
	backBn->setPosition(Vec2(s.width*0.9,s.height*0.1));
	backBn->addTouchEventListener(this, toucheventselector(MyLayer2::backToMainEvent));
	addChild(backBn);


}

void MyLayer2::backToMain(){
	Scene* s = new Scene();
	auto l = new MainLayer();
	l->setGameSocket(_socket);
	l->setMode(_mode);
	s->addChild(l);
	Director::getInstance()->replaceScene(s);
	s->release();
	l->release();
}

void MyLayer2::backToMainEvent(Ref*pSender, TouchEventType type) //����������
{
	SimpleAudioEngine::getInstance()->stopBackgroundMusic();
	switch (type)
	{
	case TOUCH_EVENT_ENDED:
	{
		backToMain();
	}
		break;
	default:
		break;
	}
}