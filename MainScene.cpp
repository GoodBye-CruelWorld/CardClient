#include "MainScene.h"
#include"BattleScene.h"
USING_NS_CC;


#define GAME_STATE_NORMAL 0
#define GAME_STATE_PREPARED 1 



// on "init" you need to initialize your instance
void MainLayer::onEnter()
{
	Layer::onEnter();


	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	auto s = Director::getInstance()->getWinSize();

	log("init...");

	//背景图,方框，游戏模式，6个按钮，一个arrow
	_bg = Sprite::create(s_png_login_bg);


	//练习模式按钮
	auto trainingButton = Button::create("button/trainingBn.png");
	trainingButton->setTouchEnabled(true);
	trainingButton->setPosition(Vec2(0, visibleSize.height / 2));
	trainingButton->runAction(MoveTo::create(0.2f, Vec2(visibleSize.width * 1 / 4, visibleSize.height * 3 / 8)));
	trainingButton->addTouchEventListener(this, toucheventselector(MainLayer::startTrainingEvent));
	addChild(trainingButton);

	//冒险模式按钮
	auto adventureButton = Button::create("button/adventureBn.png");
	adventureButton->setTouchEnabled(true);
	adventureButton->setPosition(Vec2(0, visibleSize.height / 2));
	adventureButton->runAction(MoveTo::create(0.2f, Vec2(visibleSize.width / 2, visibleSize.height / 2)));
	adventureButton->addTouchEventListener(this, toucheventselector(MainLayer::adventureEvent));
	addChild(adventureButton);

	//对战模式按钮
	auto versusButton = Button::create("button/versusBn.png");
	versusButton->setTouchEnabled(true);
	versusButton->setPosition(Vec2(0, visibleSize.height / 2));
	versusButton->runAction(MoveTo::create(0.2f, Vec2(visibleSize.width * 3 / 4, visibleSize.height * 3 / 8)));
	versusButton->addTouchEventListener(this, toucheventselector(MainLayer::startVersusEvent));
	addChild(versusButton);
	_socket = new GameSocket();

	_gameState = GAME_STATE_NORMAL;
	scheduleUpdate();
}

void MainLayer::update(float dt)
{
	if (_gameState == GAME_STATE_PREPARED&&_socket->_recv == false)
	{
		log("received msg");
		if (_socket->_recvMsg == "rank1")
		{
			log("rank1");
			_gameState = GAME_STATE_NORMAL;
			startGame(true, true);
		}
		if (_socket->_recvMsg == "rank2")
		{
			log("rank2");
			_gameState = GAME_STATE_NORMAL;
			startGame(true, false);
		}
	}
}
/*
*开始一场对战
*/
void MainLayer::startGame(bool mode, bool firstHand)
{
	this->unscheduleUpdate();
	Scene* s = new MyScene();
	auto l = new MyLayer2();
	l->_mode = mode;
	l->_firstHand = firstHand;
	l->_socket = _socket;
	s->addChild(l);
	//	Director::getInstance()->replaceScene(TransitionFade::create(2, s));
	Director::getInstance()->replaceScene(s);
	s->release();
	l->release();
}
void MainLayer::adventureEvent(Ref*pSender, TouchEventType type)
{
	switch (type)
	{
	case TOUCH_EVENT_ENDED:
	{

		if (!_socket->connect())
		{
			log("connect failed!");
		}
		log("connect success!");
		/*	string a = _userName->getText();
		if (a == "")
		return;
		string b = "username:" + a;
		_socket->sendMsg(b);*/
	}
		break;
	default:
		break;
	}
}

void MainLayer::startTrainingEvent(Ref*pSender, TouchEventType type)
{


	switch (type)
	{
	case TOUCH_EVENT_BEGAN:
		break;
	case TOUCH_EVENT_MOVED:
		break;
	case TOUCH_EVENT_ENDED:
	{
		Scene* s = new MyScene();
		auto l = new MyLayer2();
		l->_mode = 0;
		l->_socket = _socket;
		s->addChild(l);
		//	Director::getInstance()->replaceScene(TransitionFade::create(2, s));
		Director::getInstance()->replaceScene(s);
		s->release();
		l->release();
	}
		break;
	case TOUCH_EVENT_CANCELED:
		break;
	default:
		break;
	}


}

void MainLayer::startVersusEvent(Ref*pSender, TouchEventType type)
{


	switch (type)
	{
	case TOUCH_EVENT_ENDED:
	{
		//this->unscheduleUpdate();
		//发送对战消息
	/*	string a = _userName->getText();
		if (a == "")
			return;
		string b = "username:" + a;
		_socket->sendMsg(b);
		_socket->recvMsg();
		_gameState = GAME_STATE_PREPARED;*/


	}
		break;
	default:
		break;
	}


}


void MainLayer::setGameSocket(GameSocket* socket)
{
	_socket = socket;
}
void MainLayer::setMode(int mode)
{
	_mode= mode;
}