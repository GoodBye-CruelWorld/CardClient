#include"ShopScene.h"
#include"Tool.h"
#include"MainScene.h"



void ShopLayer::onEnter()
{
	Layer::onEnter();

	auto s = Director::getInstance()->getWinSize();

	//初始化卡牌数组
	CardArray a1 = { 0, { 9, 3, 2, 2 } };
	CardArray a2 = { 1, { 11, 6, 2, 0 } };
	CardArray a3 = { 10, { 9, 0, 0, 0 } };
	_cardArray.push_back(a1);
	_cardArray.push_back(a2);
	_cardArray.push_back(a3);
	

	//定义背景
	auto bg = Sprite::create("book/bg0.jpg");
	bg->setPosition(s / 2);
	addChild(bg);

	//黑色蒙版
	auto dark= Sprite::create("book/dark.png");
	dark->setPosition(s / 2);
	addChild(dark);

	//定义卷轴
	auto div = Sprite::create("book/div.png");
	div->setPosition(s / 2);
	div->setScale(0.7);
	addChild(div);

	auto divSize = div->getContentSize();
	//定义宝箱
	_box = Sprite::create("book/box.png");
	_box->setPosition(Vec2(divSize.width / 2, divSize.height*0.7));
	div->addChild(_box);


	//定义抽牌按钮
	_drawBtn = Button::create("book/buy.png");
	_drawBtn->setTouchEnabled(true);
	_drawBtn->setPosition(Vec2(divSize.width / 2, divSize.height*0.3));
	div->addChild(_drawBtn);
	_drawBtn->addTouchEventListener(this, toucheventselector(ShopLayer::startDrawEvent));

	//backbutton
	auto back = Button::create("book/back.png");
	_drawBtn->setTouchEnabled(true);
	back->setPosition(Vec2(s.width *0.08, s.height*0.95));
	back->setScaleX(0.8);
	addChild(back);
	back->addTouchEventListener(this, toucheventselector(ShopLayer::backEvent));

	auto money = Sprite::create("book/money.png");
	money->setPosition(Vec2(s.width*(0.86), s.height*(0.05)));

	addChild(money);

	//money
	_user = new User("test1");
	//_user->setData("test1");

	_moneyLabel = Tool::createEnglishLabel();
	_moneyLabel->setScale(1);
	setMoneyLabel(_user->getMoney());
	_moneyLabel->setColor(Color3B(194, 84, 26));
	_moneyLabel->setPosition(Vec2(s.width*(0.95), s.height*(0.05)));

	addChild(_moneyLabel);
	
}

void ShopLayer::startDrawEvent(Ref*pSender, TouchEventType type)
{


	switch (type)
	{
	case TOUCH_EVENT_ENDED:
	{
		SimpleAudioEngine::getInstance()->playEffect("bgm/Select.mp3");
		DrawCard();
	}
	break;
	default:
		break;
	}
}

void ShopLayer::DrawCard()
{
	if (_user->getMoney() < 500)
		return;
	auto fs = Director::getInstance()->getWinSize();

	int x, y, z;//x=英雄,y=卡牌种类,z=卡牌序号
	while (true)
	{
		//英雄随机
		int rand = CCRANDOM_0_1()*100;
		x =rand% _cardArray.size();
		//卡牌种类随机
		y = rand % 4;
		if (_cardArray.at(x).CardNumber[y] != 0)
		{
			z = rand % _cardArray.at(x).CardNumber[y];
			break;
		}

	}

	int cardID = _cardArray.at(x).RoleID * 10000 + y * 1000 + z;
	
	cummonAnime(cardID);
	_user->addUserCard(cardID);
	_user->setMoney(_user->getMoney() - 500);
	setMoneyLabel(_user->getMoney());
}

void ShopLayer::setMoneyLabel(int Money)
{
	char s[10];
	sprintf(s, "%d", Money);
	string str = s;
	_moneyLabel->setString(str);
}


void ShopLayer::cummonAnime(int id)
{

	auto s = Director::getInstance()->getWinSize();
	_box->runAction(ScaleTo::create(0.1f, 0));
	_drawBtn->runAction(ScaleTo::create(0.1f, 0));
	CCard _card = CCard(id);
	BoardCard *_boardCard = BoardCard::create(_card);
	_boardCard->setPosition(Vec2(s.width*0.5, s.height*0.7));
	addChild(_boardCard);
	//_boardCard->turnSide();
	_boardCard->setScale(3);
	_state = false;




	//粒子效果
	auto particle = ParticleSystemQuad::create("particles/Get.plist");
	particle->setPosition(Vec2(s.width*0.5, s.height*0.7));
	particle->setSpeedVar(0.01);
	particle->setStartSize(0.6);
	addChild(particle,99);







	auto action = RotateBy::create(1.5f, Vec3(0, 3600, 0));
	auto easeaction = EaseOut::create(action,2);

	_boardCard->runAction(Sequence::create(
		DelayTime::create(1),
		
		easeaction,
		CallFunc::create(CC_CALLBACK_0(BoardCard::turnSide, _boardCard, 0.25)),

		DelayTime::create(0.5),
		ScaleTo::create(0.3, 3.5),
	
		ScaleTo::create(0.3, 3),
		DelayTime::create(2),
		Spawn::create(MoveBy::create(0.5, Vec3(0, 0, 100)),ScaleTo::create(0.5,3),RotateBy::create(0.5,Vec3(0,-90,0)), FadeOut::create(1),NULL),
		
		NULL));

	this->runAction(Sequence::create(
		DelayTime::create(2.2),
		CallFunc::create(CC_CALLBACK_0(ShopLayer::addParticle, this, _boardCard)),
		DelayTime::create(6.5),
		CallFunc::create(CC_CALLBACK_0(Node::removeFromParent, _boardCard)),
		CallFunc::create(CC_CALLBACK_0(Node::removeFromParent, particle)),
		//CallFunc::create(CC_CALLBACK_0(Node::runAction, _box, ScaleTo::create(0.1f, 1))),
		CallFunc::create(CC_CALLBACK_0(ShopLayer::setState, this, true)),
		NULL));

	_box->runAction(Sequence::create(
		DelayTime::create(5.2),
		ScaleTo::create(0.1f, 1),
		NULL));
	_drawBtn->runAction(Sequence::create(
		DelayTime::create(5.2),
		ScaleTo::create(0.1f, 1),
		NULL));

}


void ShopLayer::setState(bool state)
{
	_state = state;
}


void ShopLayer::backEvent(Ref*pSender, TouchEventType type)
{

	SimpleAudioEngine::getInstance()->stopBackgroundMusic();
	switch (type)
	{
	case TOUCH_EVENT_ENDED:
	{
		SimpleAudioEngine::getInstance()->playEffect("bgm/Select.mp3");
		Scene* s = new Scene();
		auto l = new MainLayer();
		l->setGameSocket(_socket);
		l->_user = _user;
		s->addChild(l);
		Director::getInstance()->replaceScene(TransitionFade::create(0.5f, s));
		s->release();
		l->release();
	}
		break;
	default:
		break;
	}

}


void ShopLayer::addParticle(Node*node){
	auto par = ParticleSystemQuad::create("particles/getcard.plist");
	
	par->setPosition(node->getPosition()-Vec2(0,10));
	addChild(par);







}