#include"ShopScene.h"





void ShopLayer::onEnter()
{
	Layer::onEnter();

	auto fs = Director::getInstance()->getWinSize();

	//初始化卡牌数组
	CardArray a1 = { 0, { 9, 3, 2, 2 } };
	CardArray a2 = { 1, { 11, 6, 2, 0 } };
	CardArray a3 = { 10, { 9, 0, 0, 0 } };
	_cardArray.push_back(a1);
	_cardArray.push_back(a2);
	_cardArray.push_back(a3);
	
	//定义抽牌按钮
	_drawBtn = Button::create(s_png_main_train, s_png_main_train_bn);
	_drawBtn->setTouchEnabled(true);
	_drawBtn->setPosition(Vec2(fs.width*(0.2), fs.height*(0.5)));
	addChild(_drawBtn);
	_drawBtn->addTouchEventListener(this, toucheventselector(ShopLayer::startDrawEvent));

	_user = new User("test1");
	//_user->setData("test1");
	_user->setMoney(10000);
	_moneyLabel = Label::create("", "fonts/Marker Felt.ttf", 50);
	_moneyLabel->setPosition(Vec2(fs.width*(0.2), fs.height*(0.8)));
	setMoneyLabel(_user->getMoney());
	addChild(_moneyLabel);
	
}

void ShopLayer::startDrawEvent(Ref*pSender, TouchEventType type)
{


	switch (type)
	{
	case TOUCH_EVENT_ENDED:
	{
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

	int x, y, z;
	while (true)
	{
		//英雄随机
		x = rand() % _cardArray.size();
		//卡牌种类随机
		y = rand() % 4;
		if (_cardArray.at(x).CardNumber[y] != 0)
		{
			z = rand() % _cardArray.at(x).CardNumber[y];
			break;
		}

	}

	int _cardID = _cardArray.at(x).RoleID * 10000 + y * 1000 + z;
	CCard _card = CCard(_cardID);
	BoardCard *_boardCard = BoardCard::create(_card);
	_boardCard->setPosition(Vec2(fs.width*0.5, fs.height*0.5));
	addChild(_boardCard);
	_boardCard->turnSide();

	_user->setMoney(_user->getMoney() - 500);
	setMoneyLabel(_user->getMoney());
}

void ShopLayer::setMoneyLabel(int Money)
{
	char s[10];
	sprintf_s(s, "%d", Money);
	string str = s;
	_moneyLabel->setString(str);
}