
#include"BattleScene.h"
#include"MainScene.h"
#include "renderer/CCRenderer.h"
#include "renderer/CCCustomCommand.h"
#include "renderer/CCGroupCommand.h"


//
////
////将层按顺序设置，并定义返回，重载，前进的回调函数
////测试用
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


	_gameBoard = GameBoard::getInstance();
	_gameBoard->setPosition(Vec2(s.width / 2, s.height / 2));
	addChild(_gameBoard);




	int id[30];
	for (int i = 0; i < 30; i++)
		id[i] = i%3;

	bool firstHand[2];
	firstHand[0] = false;
	firstHand[1] = true;
	if (_firstHand)
	{
		firstHand[0] = true;
		firstHand[1] = false;
	}

	for (int i = 0; i < 2; i++)
	{
		_battleID[i] = 0;
		_battleState[i] = false;
		_battles[i] = new CBattle(_gameBoard, &_battleID[i], &_battleState[i], id, i,_socket,_mode,firstHand[i]);
		this->addChild(_battles[i]);	
	}


	_battles[0]->setEnemy(_battles[1]);
	_battles[1]->setEnemy(_battles[0]);
	_battles[0]->GameStart();
	_battles[1]->GameStart();
	_tool = new BattleTool2D(_gameBoard, _battles[0], _battles[1], & _battleID[0], &_battleState[0]);
	addChild(_tool);

	auto backBn = Button::create(s_png_battle_exit);
	backBn->setPosition(Vec2(s.width*0.9,s.height*0.1));
	backBn->addTouchEventListener(this, toucheventselector(MyLayer2::backToMainEvent));
	addChild(backBn);


}

void MyLayer2::backToMain(){
	Scene* s = new Scene();
	auto l = new MainLayer();
	l->setGameSocket(_socket);
	s->addChild(l);
	Director::getInstance()->replaceScene(s);
	s->release();
	l->release();
}

void MyLayer2::backToMainEvent(Ref*pSender, TouchEventType type) //返回主界面
{
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