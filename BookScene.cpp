#include"BookScene.h"
#include"Tool.h"
#include"BoardCardBuilder.h"

int Book::convertToCardID(int num)
{
	return _curRole * 10000 + _curType * 1000 + num;
}

void Book::onEnter()
{
	Layer::onEnter();
	auto winsize=Director::getInstance()->getWinSize();
	_initCard = false;
	_state = true;
	//初始化翻页功能
	curPage = 0;
	_scr = true;//图鉴处于上翻状态
	_curSize = 3;
	_curNum = 0;
	_curType = 0;
	_curRole = 0;
	_curSet = 1;
	initCardArray();
	initCard();
		
	char c[40];
	for (int i = 0; i < 3; i++)
	{
		sprintf(c, "book/book%d.png", i + 1);
		_bgs[i] = Sprite::create(c);
		_bgs[i]->setPosition(winsize / 2);
		addChild(_bgs[i]);
		_bgs[i]->setVisible(!i);
	}
	auto s = Director::getInstance()->getWinSize();

	auto leftButton = Button::create("book/arrow2.png");
	leftButton->setPosition(Vec2(10, 164) - Vec2(311, 164)+winsize/2);
	leftButton->addTouchEventListener(this, toucheventselector(Book::leftEvent));
	addChild(leftButton);
	auto rightButton = Button::create("book/arrow.png");
	rightButton->setPosition(Vec2(602, 164) - Vec2(311, 164) + winsize / 2);
	rightButton->addTouchEventListener(this, toucheventselector(Book::rightEvent));
	addChild(rightButton);
	rightButton->runAction(RepeatForever::create(Sequence::create(
		MoveBy::create(0.4f, Vec2(-3, 0)),
		MoveBy::create(0.4f, Vec2(3, 0)),
		NULL)));
	leftButton->runAction(RepeatForever::create(Sequence::create(
		MoveBy::create(0.4f, Vec2(3, 0)),
		MoveBy::create(0.4f, Vec2(-3, 0)),
		NULL)));

	_line = Sprite::create("book/line.png");
	_line->setPosition(Vec2(300, 134) - Vec2(311, 158));
	addChild(_line);

	_scroll = Sprite::create("book/scroll.png");
	_scroll->setPosition(582, 250);
	_bgs[0]->addChild(_scroll);


	_scrollline = Sprite::create("book/line.png");
	_scrollline->setPosition(582, 120);
	_scrollline->setVisible(false);
	_bgs[0]->addChild(_scrollline);

	_cardSetBar = Sprite::create("book/cardbar.png");
	_cardSetBar->setPosition(s.width*0.9, s.height / 2);

	addChild(_cardSetBar);
	//初始化使魔图鉴
	_selectedCard = -1;
	auto framebatch = SpriteBatchNode::create("book/frame.png");

	for (int i = 0; i <_cardArray.at(_curRole).CardNumber[_curType]; i++)
	{
		_frames[i] = Sprite::createWithTexture(framebatch->getTexture());
		_frames[i]->setPosition(160 + i % 2 * 285, 222 - i / 2 * 85);
		_bgs[0]->addChild(_frames[i]);

		_frames[i]->setCascadeOpacityEnabled(true);
		if (i>5)
			_frames[i]->setOpacity(0);

		CCard card = CCard(convertToCardID(i));
		//卡牌头像
		_cards[i] = BoardCardBuilder::buildCardPhote(card.get_cardID());
		_cards[i]->setPosition(40, 42);
		_frames[i]->addChild(_cards[i]);

		//卡牌值

		//添加名称label
		auto _laName = Tool::createTitleLabel();
		Tool::setLabelString(_laName, card.get_cardName());
		_laName->setScale(0.8);
		_laName->setPosition(200,60);
		_frames[i]->addChild(_laName);
		//添加费用label
		sprintf(c, "%d", card.get_cost());
		auto _laCost = Tool::createEnglishLabel();
		_laCost->setString(c);
		_laCost->setPosition(120, 30);
		_frames[i]->addChild(_laCost);
		//添加攻击label
		sprintf(c, "%d", card.get_attack());//取最终
		auto _laAttack = Tool::createEnglishLabel();
		_laAttack->setString(c);
		_laAttack->setPosition(170, 30);
		_frames[i]->addChild(_laAttack);
		//添加生命label
		sprintf(c, "%d", card.get_health());//取实际
		auto _laHealth = Tool::createEnglishLabel();
		_laHealth->setString(c);
		_laHealth->setPosition(220, 30);
		_frames[i]->addChild(_laHealth);




		_upbuttons[i] = Button::create("button/levelUp2.png");
		_upbuttons[i]->setPosition(Vec2(200, 32));
		_upbuttons[i]->setEnabled(false);
		_upbuttons[i]->setBright(false);
		_upbuttons[i]->setTouchEnabled(false);
		_frames[i]->addChild(_upbuttons[i]);
	}

	for (int i = 6; i < _curSize; i++)
	{
		_upbuttons[i]->setVisible(false);
	}




	for (int i = 0; i < _curSet; i++)
	{
		auto set = Sprite::create("book/cardset0.png");
		set->setPosition(Vec2(94, 700 - i * 45));
		_cardSetBar->addChild(set);
		_cardSets.pushBack(set);
	}

	_newSetBn = Button::create("book/newset.png");
	_newSetBn->setTouchEnabled(true);
	_newSetBn->setPosition(Vec2(94, 700- _curSet * 45));
	_cardSetBar->addChild(_newSetBn);
	_newSetBn->addTouchEventListener(this, toucheventselector(Book::newSetEvent));



	auto touchListener = EventListenerTouchOneByOne::create();//创建单点触摸事件监听器
	touchListener->onTouchBegan = CC_CALLBACK_2(Book::onTouchBegan, this);//触摸开始
	touchListener->onTouchMoved = CC_CALLBACK_2(Book::onTouchMoved, this);//触摸中移动
	touchListener->onTouchEnded = CC_CALLBACK_2(Book::onTouchEnded, this);//触摸结束
	_eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);//注册分发器

	_desLb = NULL;
	_carddetail = NULL;

}


void Book::displayCard(int num)
{
	if (!_state)
		return;
	if (num<0 || num>_curSize)
		return;

	for (int i = 0; i < _curSize; i++)
	{

		//_frames[i]->setPosition3D(Vec3(_frames[i]->getPosition3D() + Vec3(0, 0, 30)));
		_frames[i]->runAction(Sequence::create(
			DelayTime::create(0.1 + i / 20.0),
			ScaleTo::create(0.1f, 0.f),
			MoveBy::create(0.1f, Vec3(0, 0, -30)),
			NULL));
	}


	_selectedCard = num;
	auto s = Director::getInstance()->getWinSize();

	//卡牌
	_cardSel =BoardCard::create(CCard(num));
	_cardSel->setVisible(false);
	_cardSel->setScale(1.2);
	_cardSel->setPosition3D(Vec3(112, 145, 40));
	_cardSel->runAction(Sequence::create(
		DelayTime::create(0.4),
		CallFunc::create(CC_CALLBACK_0(Node::setVisible, _cardSel, true)),
		ScaleTo::create(0.1f, 0.8f),
		MoveBy::create(0.1f, Vec3(0, 0, -40)),
		NULL));

	//分割线
	_line->runAction(Sequence::create(
		DelayTime::create(0.6),
		MoveBy::create(0.3f, Vec3(-75, 0, 0)),
		NULL));

	_carddetail = Sprite::create("book/desA.png");
	_carddetail->setPosition3D(Vec3(554 - 75, 144, 40));
	_carddetail->setVisible(false);
	_carddetail->setScale(1.2);
	_carddetail->runAction(Sequence::create(
		DelayTime::create(1.f),
		CallFunc::create(CC_CALLBACK_0(Node::setVisible, _carddetail, true)),
		ScaleTo::create(0.1f, 1.f),
		MoveBy::create(0.1f, Vec3(0, 0, -40)),
		NULL));
	_bgs[0]->addChild(_carddetail, 0, 14);
	_bgs[0]->addChild(_cardSel, 0, 15);

	char c[100];
	auto desLb = Tool::createTextLabel();
	sprintf(c, "Desc%d", convertToCardID(num));
	Tool::setLabelString(desLb, c);
	desLb->setPosition(555 - 468, 75);
	desLb->setDimensions(360, 400);
	desLb->setScale(0.35);
	_carddetail->addChild(desLb, 2, 13);





}



void Book::undisplay()
{
	for (int i = 10; i < 16; i++)
	{
		auto node = _bgs[0]->getChildByTag(i);
		if (node != NULL)
		{
			node->runAction(Sequence::create(
				ScaleTo::create(0.1f, 0.f),
				CallFunc::create(CC_CALLBACK_0(Node::removeFromParent, node)),
				NULL));
		}
	}
	_line->runAction(Sequence::create(
		MoveBy::create(0.2f, Vec3(75, 0, 0)),
		NULL));

	_selectedCard = -1;

	for (int i = 0; i < 8; i++)
	{
		int offset = 0;
		if (!_scr)
		{
			offset = 85;
		}
		_frames[i]->setPosition3D(Vec3(160 + i % 2 * 285, 222 - i / 2 * 85 + offset, 0) + Vec3(0, 0, 30));
		_frames[i]->runAction(Sequence::create(
			DelayTime::create(0.2 + i / 20.0),
			ScaleTo::create(0.1f, 1.f),
			MoveBy::create(0.1f, Vec3(0, 0, -30)),
			NULL));

	}

	auto node = _bgs[1]->getChildByTag(10);
	if (node != NULL)
		node->removeFromParent();



	if (_bgs[2]->getChildByTag(10) != NULL)
		_bgs[2]->getChildByTag(10)->removeFromParent();

}


void Book::open()
{

	_line->setPosition(Vec2(300, 134) - Vec2(311, 158));
	this->setVisible(true);
	this->setScale(0.f);
	this->runAction(Sequence::create(
		ScaleTo::create(0.1f, 1.f),
		NULL));
	for (int i = 0; i < 8; i++)
	{
		_frames[i]->setScale(0.f);
		_frames[i]->setPosition3D(Vec3(_frames[i]->getPosition3D() + Vec3(0, 0, 30)));
		_frames[i]->runAction(Sequence::create(
			DelayTime::create(0.1 + i / 20.0),
			ScaleTo::create(0.1f, 1.f),
			MoveBy::create(0.1f, Vec3(0, 0, -30)),
			NULL));
	}

}

void Book::close()
{
	if (this->isVisible())
	{
		undisplay();
		this->runAction(Sequence::create(
			ScaleTo::create(0.1f, 0.f),
			CallFunc::create(CC_CALLBACK_0(Node::setVisible, this, false)),
			NULL));
	}
}


void Book::turnPage(int delta)
{
	for (int i = 0; i < 3; i++)
		_bgs[i]->setVisible(false);
	curPage = (curPage + 3 + delta) % 3;
	_bgs[curPage]->setVisible(true);
	if (curPage == 1)
		_line->setVisible(false);
	else
		_line->setVisible(true);

}


void Book::leftEvent(Ref* sender, TouchEventType type)
{
	switch (type)
	{
	case TOUCH_EVENT_ENDED:
		turnPage(-1);
		break;
	default:
		break;
	}
}
void Book::rightEvent(Ref* sender, TouchEventType type)
{
	switch (type)
	{
	case TOUCH_EVENT_ENDED:
		turnPage(1);
		break;
	default:
		break;
	}
}








//设置图鉴状态
void Book::setState(int state)
{
	_state = state;
}





bool Book::onTouchBegan(Touch* touch, Event* event)
{
	return true;
}

void Book::onTouchMoved(Touch* touch, Event* event)
{
}
void Book::onTouchEnded(Touch* touch, Event* event)
{
	Point tp = touch->getLocation();
	//加牌
	for (int i = 0; i < _curSize; i++)
	{
		if (collisionCheck(tp, _frames[i]))
		{

			auto id = convertToCardID(i);
			if (_state)
				addCardintoArray(id);
			else
				displayCard(id);
		}
	}
	//删牌
	for (int i = 0; i < _curSize; i++)
	{
		/*if (collisionCheck(tp, _frames[i]))
		{
			auto id = convertToCardID(i);

			delCardofArray( id);
		}*/
	}

}


void Book::scroll(bool direction)//true=上滑,false=下滑
{
	if (curPage != 0 || _selectedCard != -1)
		return;
	_scr = direction;
	if (direction)
	{
		_scroll->setPosition(582, 250);
		for (int i = 0; i < 8; i++)
		{
			_frames[i]->runAction(MoveTo::create(0.2f, Vec2(160 + i % 2 * 285, 222 - i / 2 * 85)));
			if (i < 2)
			{
				_frames[i]->runAction(FadeIn::create(0.2));
				_upbuttons[i]->setVisible(true);
			}
			if (i > 5)
			{
				_frames[i]->runAction(FadeOut::create(0.2));
				_upbuttons[i]->setVisible(false);
			}
		}
	}
	else
	{
		_scroll->setPosition(582, 40);
		for (int i = 0; i < 8; i++)
		{

			_frames[i]->runAction(MoveTo::create(0.2f, Vec2(160 + i % 2 * 285, 222 - i / 2 * 85 + 85)));
			if (i < 2)
			{
				_frames[i]->runAction(FadeOut::create(0.2));
				_upbuttons[i]->setVisible(false);
			}
			if (i > 5)
			{
				_frames[i]->runAction(FadeIn::create(0.2));
				_upbuttons[i]->setVisible(true);
			}
		}
	}
}


void Book::initCard()
{


}
bool Book::collisionCheck(Point p, Node *node)
{
	auto s = Director::getInstance()->getWinSize();
	auto a = node->getPosition();
	auto scale = node->getScale();
	Node*parent = node->getParent();
	while (parent != this)
	{
		a += parent->getPosition() - parent->getContentSize() / 2;
		parent = parent->getParent();
	}


	auto b = node->getContentSize() / 2 * node->getScale();

	if (p.x<(a.x + b.width) && p.x>(a.x - b.width) && p.y<(a.y + b.height) && p.y>(a.y - b.height))
		return true;
	else
		return false;
}

void Book::initCardArray()
{
	CardArrayInBook a1 = { 0, { 9, 3, 2, 2 } };
	CardArrayInBook a2 = { 1, { 11, 6, 2, 0 } };
	CardArrayInBook a3 = { 10, { 9, 0, 0, 0 } };
	_cardArray.push_back(a1);
	_cardArray.push_back(a2);
	_cardArray.push_back(a3);
}


void Book::createCardArray(string cardName, int roleID)//创建卡组，返回cardArrayID
{
	//_user->createCardArray(cardName, roleID);
	char c[20];
	sprintf(c, "book/cardset%d.png", roleID);
	auto set = Sprite::create(c);
	set->setPosition(Vec2(94, 700 - _cardSets.size() * 100));
	_cardSetBar->addChild(set);
	_cardSets.pushBack(set);
	_selCardSet = _cardSets.size() - 1;
	changeCardArray();
}
void Book::addCardArray()						//增加卡组，添加到数据库
{
	//_user->addCardArray(_selCardSet);
}
bool Book::delCardArray()						//删除卡组
{
//	_user->delCardArray(_selCardSet);
	_cardSets.at(_selCardSet)->removeFromParent();
	adjustCardSets();
	return true;
}
int Book::getCardArrayNumber()								//获得所有的卡组数量
{
	return 1;
	return _user->getCardArrayNumber();
}

bool Book::addCardintoArray(int cardID)		//在卡组中增加卡牌
{
	Sprite *card = BoardCardBuilder::buildCardInSet(cardID);
	card->setPosition(94, 700-45-_cardsInSet.size()*45);
	_cardsInSet.pushBack(card);
	_cardSetBar->addChild(card);
	return true;
}
bool Book::delCardofArray(int cardID)		//删除卡牌
{
	return NULL;
}
void Book::changeCardArray()
{
	_cardSets.at(_selCardSet)->runAction(MoveTo::create(0.1f, Vec2(94, 700)));
	for (int i = 0; i < _cardSets.size(); i++)
	{
		if (i != _selCardSet)
			_cardSets.at(i)->setVisible(false);
	}
	_newSetBn->setVisible(false);	
}

bool Book::addUserCard(int ID)								//增加卡牌
{
	return false;
}


void Book::newSetEvent(Ref*pSender, TouchEventType type)
{
	switch (type)
	{
	case TOUCH_EVENT_ENDED:
	{
		createCardArray("hello",_curRole);

	}
		break;
	default:
		break;
	}
}


void Book::adjustCardSets()
{

}