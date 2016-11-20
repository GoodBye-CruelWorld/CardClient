#include "MainScene.h"
#include"BattleScene.h"
USING_NS_CC;


#define GAME_STATE_NORMAL 0
#define GAME_STATE_PREPARED 1 



// on "init" you need to initialize your instance
void MainLayer::onEnter()
{
	Layer::onEnter();



	auto s = Director::getInstance()->getWinSize();

	log("init...");

	//元素：背景图,方框，游戏模式，6个按钮，一个arrow
	_bg = Sprite::create(s_png_main_bg);
	_bg->setPosition(s / 2);
	addChild(_bg);

	_frame = Sprite::create(s_png_main_frame);
	_frame->setPosition(s.width / 2, s.height *0.48);
	_frame->setScaleX(0.84);
	addChild(_frame);

	auto fs = _frame->getContentSize();

	_modeImg = Sprite::create(s_png_main_mode);
	_modeImg->setPosition(fs.width / 2, fs.height*0.87);
	_frame->addChild(_modeImg);

	_arrow = Sprite::create(s_png_main_arrow);
	_arrow->setPosition(fs.width / 2, fs.height*0.87);
	_frame->addChild(_arrow);

	const char *buttonPath[12];
	buttonPath[0] = s_png_main_train;
	buttonPath[1] = s_png_main_train_bn;
	buttonPath[2] = s_png_main_adventure;
	buttonPath[3] = s_png_main_adventure_bn;
	buttonPath[4] = s_png_main_versus;
	buttonPath[5] = s_png_main_versus_bn;
	buttonPath[6] = s_png_main_shop;
	buttonPath[7] = s_png_main_shop_bn;
	buttonPath[8] = s_png_main_book;
	buttonPath[9] = s_png_main_book_bn;
	buttonPath[10] = s_png_main_option;
	buttonPath[11] = s_png_main_option_bn;
	for (int i = 0; i < 6; i++)
	{
		_buttons[i] = Button::create(buttonPath[i * 2], buttonPath[i * 2 + 1]);
		_buttons[i]->setTouchEnabled(true);
		if (i<3)
			_buttons[i]->setPosition(Vec2(fs.width*(0.6 + i*0.1), fs.height*(0.77 - i*0.07)));
		else
			_buttons[i]->setPosition(Vec2(fs.width / 2, fs.height*(0.7 - i*0.1)));

		_frame->addChild(_buttons[i]);
	}
	_buttons[0]->addTouchEventListener(this, toucheventselector(MainLayer::startTrainingEvent));
	_buttons[1]->addTouchEventListener(this, toucheventselector(MainLayer::startAdventureEvent));
	_buttons[2]->addTouchEventListener(this, toucheventselector(MainLayer::startVersusEvent));
	_buttons[3]->addTouchEventListener(this, toucheventselector(MainLayer::startShopEvent));
	_buttons[4]->addTouchEventListener(this, toucheventselector(MainLayer::startBookEvent));
	_buttons[5]->addTouchEventListener(this, toucheventselector(MainLayer::startOptionEvent));

	//元素加载完毕



	_gameState = GAME_STATE_NORMAL;
	scheduleUpdate();
}

void MainLayer::update(float dt)
{
	if (_gameState == GAME_STATE_PREPARED&&_socket->_recv == false)
	{
		log("received msg");
		auto msg = _socket->getMsg();
		if (msg== "rank1")
		{
			log("rank1");
			_gameState = GAME_STATE_NORMAL;
			startGame(true, true);
		}
		if (msg == "rank2")
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
	Director::getInstance()->replaceScene(s);
	s->release();
	l->release();
}

//开始冒险模式

void MainLayer::startAdventureEvent(Ref*pSender, TouchEventType type)
{
	switch (type)
	{
	case TOUCH_EVENT_ENDED:
	{
		
		Scene* s = new MyScene();
		auto l = new MyLayer2();
		l->_mode = 0;
		l->_socket = _socket;
		s->addChild(l);
		Director::getInstance()->replaceScene(TransitionFade::create(1.0f, s));

		s->release();
		l->release();
	}
		break;
	default:
		break;
	}
}
//开始练习
void MainLayer::startTrainingEvent(Ref*pSender, TouchEventType type)
{


	switch (type)
	{
	case TOUCH_EVENT_ENDED:
	{
		Scene* s = new MyScene();
		auto l = new MyLayer2();
		l->_mode = 0;
		l->_socket = _socket;
		s->addChild(l);
		Director::getInstance()->replaceScene(TransitionFade::create(1.0f, s));

		s->release();
		l->release();
	}
		break;
	default:
		break;
	}


}
//开始对战
void MainLayer::startVersusEvent(Ref*pSender, TouchEventType type)
{


	switch (type)
	{
	case TOUCH_EVENT_ENDED:
	{
		//发送对战消息
		//string a = _userName->getText();
		//if (a == "")
		//	return;
		std::string b = "username:" + _account;
		_socket->sendMsg(b);
		//string b = "usernameok" ;
		//_socket->sendMsg(b);
		_socket->recvMsg();
		_gameState = GAME_STATE_PREPARED;


	}
		break;
	default:
		break;
	}


}

void MainLayer::startShopEvent(Ref*pSender, TouchEventType type)
{


	switch (type)
	{
	case TOUCH_EVENT_ENDED:
	{
		Scene* s = new Scene();
		auto l = new ShopLayer();
		l->_mode = 0;
		l->_socket = _socket;
		s->addChild(l);
		Director::getInstance()->replaceScene(TransitionFade::create(1.0f, s));

		s->release();
		l->release();
	}
		break;
	default:
		break;
	}
}

void MainLayer::startBookEvent(Ref*pSender, TouchEventType type)
{


	switch (type)
	{
	case TOUCH_EVENT_ENDED:
	{



	}
		break;
	default:
		break;
	}


}

void MainLayer::startOptionEvent(Ref*pSender, TouchEventType type)
{


	switch (type)
	{
	case TOUCH_EVENT_ENDED:
	{


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
	_mode = mode;
}


void MainLayer::setAccount(std::string acc)
{
	_account = acc;
}