#include "HelloWorldScene.h"

USING_NS_CC;

#include"MainScene.h";


#define GAME_STATE_NORMAL 0
#define GAME_STATE_PREPARED 1 

Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    





	auto s = Director::getInstance()->getWinSize();

	log("init...");
	_bg = Sprite::create(s_png_login_bg);
	_bg->setPosition(s / 2);
	addChild(_bg);

	_frame = Sprite::create(s_png_login_frame);
	_frame->setPosition(s.width / 2, s.height*0.6);
	addChild(_frame);

	_title = Sprite::create(s_png_login_title);
	_title->setPosition(s.width / 2, s.height*0.68);
	_frame->addChild(_title);

	_submit = Button::create(s_png_login_button);
	_submit->setPosition(Vec2(s.width / 2, s.height*0.49));
	_submit->setTouchEnabled(true);
	_submit->addTouchEventListener(this, toucheventselector(HelloWorld::loginEvent));
	_frame->addChild(_submit);


	_submitText = Sprite::create(s_png_login_submit);
	_submitText->setPosition(51.5, 18.5);

	_submit->addChild(_submitText);
	//注册
	_register = Button::create(s_png_login_register);
	_register->setPosition(Vec2(s.width *0.47, s.height*0.45));
	_frame->addChild(_register);
	//exit
	_exit = Button::create(s_png_login_exit);
	_exit->setPosition(Vec2(s.width *0.53, s.height*0.45));
	_frame->addChild(_exit);
	
	//创建登录框
	auto editBoxSize = Size(180, 35);

	//用户名框
	_userName = cocos2d::extension::EditBox::create(editBoxSize, cocos2d::extension::Scale9Sprite::create(s_png_login_input));
	_userName->setPosition(Point(s.width *0.5, s.height * 0.63));
	_userName->setFontSize(15);
	_userName->setFontColor(Color3B::BLACK);
	_userName->setPlaceHolder("  Username");
	_userName->setPlaceholderFontColor(Color3B::GRAY);
	_userName->setMaxLength(8);
	_userName->setReturnType(cocos2d::extension::EditBox::KeyboardReturnType::DONE);
	_frame->addChild(_userName);
	//密码框
	_userPwd = cocos2d::extension::EditBox::create(editBoxSize, cocos2d::extension::Scale9Sprite::create(s_png_login_input));
	_userPwd->setPosition(Point(s.width *0.5,s.height * 0.57));
	_userPwd->setFontSize(15);
	_userPwd->setFontColor(Color3B::BLACK);
	_userPwd->setPlaceHolder("   Password");
	_userPwd->setPlaceholderFontColor(Color3B::GRAY);
	_userPwd->setMaxLength(8);
	_userPwd->setReturnType(cocos2d::extension::EditBox::KeyboardReturnType::DONE);
	//_userPwd->setDelegate(this);
	_frame->addChild(_userPwd);
	




	_gameState = GAME_STATE_NORMAL;
	_socket = new GameSocket();
	//scheduleUpdate();
    return true;
}

//void HelloWorld::update(float dt)
//{
//	if (_gameState == GAME_STATE_PREPARED&&_socket->_recv== false)
//	{
//		log("received msg");
//		if (_socket->_recvMsg == "rank1")
//		{
//			log("rank1");
//			_gameState = GAME_STATE_NORMAL;
//			startGame(true,true);
//		}
//		if (_socket->_recvMsg == "rank2")
//		{
//			log("rank2");
//			_gameState = GAME_STATE_NORMAL;
//			startGame(true, false);
//		}
//	}
//}
/*
*开始一场对战
*/
//void HelloWorld::startGame(bool mode, bool firstHand)
//{
//	this->unscheduleUpdate();
//	Scene* s = new MyScene();
//	auto l = new MyLayer2();
//	l->_mode = mode;
//	l->_firstHand = firstHand;
//	l->_socket = _socket;
//	s->addChild(l);
//	//	Director::getInstance()->replaceScene(TransitionFade::create(2, s));
//	Director::getInstance()->replaceScene(s);
//	s->release();
//	l->release();
//}
void HelloWorld::loginEvent(Ref*pSender, TouchEventType type)
{
		switch (type)
		{
		case TOUCH_EVENT_ENDED:
		{
		
			if (!_socket->connect())
			{
				log("connect failed!");
				//return;
			}
			log("connect success!");
			Scene* s = new Scene();
			auto l = new MainLayer();
			l->setGameSocket(_socket);
			s->addChild(l);
			Director::getInstance()->replaceScene(s);
			s->release();
			l->release();
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

//void HelloWorld::startTrainingEvent(Ref*pSender, TouchEventType type)
//{
//	
//		
//	switch (type)
//	{
//	case TOUCH_EVENT_BEGAN:
//		break;
//	case TOUCH_EVENT_MOVED:
//		break;
//	case TOUCH_EVENT_ENDED:
//	{
//		Scene* s = new MyScene();
//		auto l = new MyLayer2();
//		l->_mode = 0;
//		l->_socket = _socket;
//		s->addChild(l);
//		//	Director::getInstance()->replaceScene(TransitionFade::create(2, s));
//		Director::getInstance()->replaceScene(s);
//		s->release();
//		l->release();
//	}
//			break;
//		case TOUCH_EVENT_CANCELED:
//			break;
//		default:
//			break;
//		}
//
//	
//}
//
//void HelloWorld::startVersusEvent(Ref*pSender, TouchEventType type)
//{
//	
//		
//		switch (type)
//		{
//		case TOUCH_EVENT_ENDED:
//		{
//			//this->unscheduleUpdate();
//			//发送对战消息
//			string a = _userName->getText();
//			if (a == "")
//				return;
//			string b = "username:" + a;
//			_socket->sendMsg(b);
//			_socket->recvMsg();
//			_gameState = GAME_STATE_PREPARED;
//		
//		
//		}
//			break;
//		default:
//			break;
//		}
//	
//	
//}



void HelloWorld::editBoxEditingDidBegin(cocos2d::extension::EditBox* editBox)
{
	log("editBox %p DidBegin.", editBox);
}

void HelloWorld::editBoxEditingDidEnd(cocos2d::extension::EditBox* editBox)
{
	log("editBox %p DidEnd.", editBox);
}

void HelloWorld::editBoxTextChanged(cocos2d::extension::EditBox* editBox, const std::string &text)
{
	log("editBox %p TextChanged, text: %s", editBox, text.c_str());
}

void HelloWorld::editBoxReturn(cocos2d::extension::EditBox *editBox)
{
	log("editBox %p was returned.", editBox);
}