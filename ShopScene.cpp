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

	int x, y, z;//x=英雄,y=卡牌种类,z=卡牌序号
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


void ShopLayer::cummonAnime(int id)
{
	//_state = false;


	//auto bg = Sprite::create("bg/dark.png");
	////bg->setPosition(_bgs[0]->getContentSize() / 2);
	//this->addChild(bg, 10);
	//bg->setOpacity(255 * 0.75);

	//char c[20];
	//sprintf(c, "ME/%02d.png", id);
	//auto card = Sprite::create(c);
	//card->setPosition(bg->getContentSize() / 2);
	//bg->addChild(card);
	//card->setScale(0);
	//card->runAction(Sequence::create(
	//	DelayTime::create(0.2f),
	//	Spawn::create(ScaleTo::create(2, 1), RotateBy::create(2, Vec3(0, 3600, 0)), NULL),
	//	NULL));


	////粒子效果
	//auto particle = ParticleSystemQuad::create("particles/Get.plist");
	//particle->setPosition(bg->getContentSize() / 2);
	//particle->setSpeedVar(0.01);
	//particle->setStartSize(0.6);
	//bg->addChild(particle);


	////卡牌值
	////名字
	//auto _laName = Tool::createEnglishLabel();
	////_laName->setString(_BoardCards[id]->_name);
	//_laName->setScale(0.5);
	//_laName->setPosition(Vec2(195, 316 - 32));


	////创建攻击label

	////sprintf(c, "%d", _BoardCards[id]->_oAttack);
	//auto _laAttack = Tool::createEnglishLabel();
	//_laAttack->setString(c);
	//_laAttack->setScale(0.5);
	//_laAttack->setPosition(Vec2(186, 34));

	////创建生命label
	//char l[5];
	////sprintf(l, "%d", _BoardCards[id]->_oLife);
	//auto _laLife = Tool::createEnglishLabel();
	//_laLife->setString(l);
	//_laLife->setPosition(Vec2(30, 34));
	//_laLife->setScale(0.5);
	//card->addChild(_laAttack);
	//card->addChild(_laLife);
	//card->addChild(_laName);


	//auto label = Tool::createTitleLabel(75);
	//Tool::setLabelString(label, "cummon1");
	//label->setPosition(bg->getContentSize() / 2 + Size(0, 200));
	//bg->addChild(label);
	//label->setVisible(false);
	//label->setScale(1.2);
	//label->setPositionZ(40);
	//label->runAction(Sequence::create(
	//	DelayTime::create(0.2),
	//	CallFunc::create(CC_CALLBACK_0(Node::setVisible, label, true)),
	//	ScaleTo::create(0.1f, 0.8f),
	//	MoveBy::create(0.1f, Vec3(0, 0, -40)),
	//	NULL));


	//bg->runAction(Sequence::create(
	//	DelayTime::create(2.2),
	//	DelayTime::create(3),
	//	CallFunc::create(CC_CALLBACK_0(Node::removeFromParent, bg)),
	//	CallFunc::create(CC_CALLBACK_0(Book::setState, this, true)),
	//	NULL));

}