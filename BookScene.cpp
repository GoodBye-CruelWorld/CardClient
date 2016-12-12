#include"BookScene.h"
#include"Tool.h"
#include"BoardCardBuilder.h"
#include"MainScene.h"
int Book::convertToCardID(int num)
{
	return _curRole * 10000 + _curType * 1000 + num;
}

void Book::onEnter()
{
	Layer::onEnter();
	auto winsize=Director::getInstance()->getWinSize();
	_initCard = false;
	_state = false;
	//初始化翻页功能
	curPage = 0;

	_delCard = false;
	_delSet = false;
	_scr = true;//图鉴处于上翻状态
	_curSize = 3;
	_curNum = 0;
	_curType = 0;
	_curRole = 0;
	_curSet = _user->getCardArrayNumber();
	initCardArray();
	initCard();
		
	char c[40];
	for (int i = 0; i < 3; i++)
	{
		sprintf(c, "book/book%d.png", i + 1);
		_bgs[i] = Sprite::create(c);
		_bgs[i]->setPosition(winsize / 2-Size(80,0));
		addChild(_bgs[i]);
		_bgs[i]->setRotation(-1);
		_bgs[i]->setVisible(!i);
	}
	auto s = Director::getInstance()->getWinSize();

	auto leftButton = Button::create("book/arrow2.png");
	leftButton->setPosition(Vec2(10, 164) - Vec2(421, 180)+winsize/2);
	leftButton->addTouchEventListener(this, toucheventselector(Book::leftEvent));
	addChild(leftButton);
	auto rightButton = Button::create("book/arrow.png");
	rightButton->setPosition(Vec2(602, 164) - Vec2(301, 149) + winsize / 2);
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

	/*_scroll = Sprite::create("book/scroll.png");
	_scroll->setPosition(582, 250);
	_bgs[0]->addChild(_scroll);*/


	_scrollline = Sprite::create("book/line.png");
	_scrollline->setPosition(582, 120);
	_scrollline->setVisible(false);
	_bgs[0]->addChild(_scrollline);

	_cardSetBar = Sprite::create("book/cardbar.png");
	_cardSetBar->setPosition(s.width*0.9, s.height / 2);

	addChild(_cardSetBar);
	//图鉴
	_selectedCard = -1;
	auto framebatch = SpriteBatchNode::create("book/frame.png");
	_curSize = _cardArray.at(_curRole).CardNumber[_curType];
	for (int i = 0; i < 20; i++)
	{
		_frames[i] = Sprite::createWithTexture(framebatch->getTexture());
		_frames[i]->setPosition(250 + i % 2 * 295, 352 - i / 2 * 85);
		_frames[i]->setScale(1.2);
		_bgs[0]->addChild(_frames[i]);
		_frames[i]->setCascadeOpacityEnabled(true);
		if (i>7)
			_frames[i]->setOpacity(0);
	}

	showPage();



	for (int i = 0; i < _curSet; i++)
	{
		auto setF = Sprite::create("book/frame2.png");
		setF->setPosition(Vec2(94, 670 - i * 40));
		_cardSetBar->addChild(setF);

		auto set = Sprite::create("book/cardset1.png");
		set->setPosition(set->getContentSize()/2);
	
		setF->addChild(set);
		_cardSets.pushBack(setF);
	}

	_newSetBn = Button::create("book/newset3.png");
	_newSetBn->setTouchEnabled(true);
	_newSetBn->setPosition(Vec2(94,670- _curSet * 40));
	_cardSetBar->addChild(_newSetBn);
	_newSetBn->addTouchEventListener(this, toucheventselector(Book::newSetEvent));


	_finishSet = Button::create("book/finish3.png");
	_finishSet->setTouchEnabled(true);
	_finishSet->setPosition(Vec2(94, 55));
	_finishSet->setScale(0.8);
	_cardSetBar->addChild(_finishSet);
	_finishSet->addTouchEventListener(this, toucheventselector(Book::finishSetEvent));
	_finishSet->setVisible(false);
	auto touchListener = EventListenerTouchOneByOne::create();//创建单点触摸事件监听器
	touchListener->onTouchBegan = CC_CALLBACK_2(Book::onTouchBegan, this);//触摸开始
	touchListener->onTouchMoved = CC_CALLBACK_2(Book::onTouchMoved, this);//触摸中移动
	touchListener->onTouchEnded = CC_CALLBACK_2(Book::onTouchEnded, this);//触摸结束
	_eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);//注册分发器

	_desLb = NULL;
	_carddetail = NULL;


	//backbutton
	auto back = Button::create("book/back.png");
	
	back->setPosition(Vec2(s.width *0.08, s.height*0.95));
	back->setScaleX(0.8);
	addChild(back);
	back->addTouchEventListener(this, toucheventselector(Book::backEvent));
}


void Book::displayCard(int num)
{
	if (_state)
		return;
	if (num<0 || num>_curSize)
		return;

	for (int i = 0; i < _curSize; i++)
	{

		_frames[i]->runAction(Sequence::create(
			DelayTime::create(0.1 + i / 20.0),
			ScaleTo::create(0.1f, 0.f),
			MoveBy::create(0.1f, Vec3(0, 0, -30)),
			NULL));
	}


	_selectedCard = num;
	auto s = Director::getInstance()->getWinSize();

	//卡牌
	auto a = CCard(num);
	_cardSel =BoardCard::create(a);
	_cardSel->setVisible(false);
	_cardSel->setScale(6.5);
	_cardSel->setPosition3D(Vec3(212, 380, 40));
	_cardSel->runAction(Sequence::create(
		DelayTime::create(0.4),
		CallFunc::create(CC_CALLBACK_0(Node::setVisible, _cardSel, true)),
		ScaleTo::create(0.1f, 3.f),
		MoveBy::create(0.1f, Vec3(0, 0, -40)),
		NULL));
	//_cardSel->setScale(3);
	_cardSel->turnSide(0.01);
	//分割线
	_line->runAction(Sequence::create(
		DelayTime::create(0.6),
		MoveBy::create(0.3f, Vec3(-75, 0, 0)),
		NULL));

	_carddetail = Sprite::create("book/detail.png");
	_carddetail->setPosition3D(Vec3(554 - 75-80, 184, 40));
	_carddetail->setVisible(false);
	_carddetail->setScale(0.7);
	_carddetail->runAction(Sequence::create(
		DelayTime::create(1.f),
		CallFunc::create(CC_CALLBACK_0(Node::setVisible, _carddetail, true)),
		ScaleTo::create(0.1f, 1.f),
		MoveBy::create(0.1f, Vec3(0, 0, -40)),
		NULL));
	_bgs[0]->addChild(_carddetail, 0, 14);
	_bgs[0]->addChild(_cardSel, 0, 15);

	char c[100];

	auto nameLb = Tool::createTitleLabel();
	sprintf(c, "Name%d", convertToCardID(num));
	Tool::setLabelString(nameLb, c);
	nameLb->setPosition(555 - 468+400, 475);
	nameLb->setDimensions(360, 400);
	nameLb->setScale(1);
	_carddetail->addChild(nameLb, 2, 14);

	auto desLb = Tool::createTitleLabel();
	sprintf(c, "Desc%d", convertToCardID(num));
	Tool::setLabelString(desLb, c);
	desLb->setPosition(555 - 468+190, 185);
	desLb->setDimensions(360, 400);
	desLb->setScale(1);
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
	clearCurPage();
	curPage = (curPage + 6 + delta) % 6;
	if (curPage < 3)
		_curRole = 0;
	else
		_curRole = 1;

	if (curPage % 3 == 0)
		_curType = 0;
	else if (curPage % 3 == 1)
		_curType = 1;
	else
		_curType = 2;
	_curSize = _cardArray.at(_curRole).CardNumber[_curType];
	showPage();

}
void Book::clearCurPage(){
	for (int i = 0; i < _curSize; i++)
	{
		_frames[i]->removeChildByTag(11);
		_frames[i]->removeChildByTag(12);
		_frames[i]->removeChildByTag(13);
		_frames[i]->removeChildByTag(14);
		_frames[i]->removeChildByTag(15);
		_frames[i]->setVisible(false);
	}
}
void Book::showPage(){
	char c[20];

	
	for (int i = 0; i <_curSize; i++)
	{

		_frames[i]->setVisible(true);
		CCard card = CCard(convertToCardID(i));
		//卡牌头像
		_cards[i] = BoardCardBuilder::buildCardPhote(card.get_cardID());
		_cards[i]->setPosition(40, 42);
		_frames[i]->addChild(_cards[i],1,11);

		//卡牌值

		//添加名称label
		auto _laName = Tool::createTitleLabel();
		Tool::setLabelString(_laName, card.get_cardName());
		_laName->setScale(0.8);
		_laName->setPosition(200, 60);
		_frames[i]->addChild(_laName,1,12);
		//添加费用label
		sprintf(c, "%d", card.get_cost());
		auto _laCost = Tool::createEnglishLabel();
		_laCost->setString(c);
		_laCost->setPosition(120, 30);
		_frames[i]->addChild(_laCost,1,13);
		//添加攻击label
		sprintf(c, "%d", card.get_attack());//取最终
		auto _laAttack = Tool::createEnglishLabel();
		_laAttack->setString(c);
		_laAttack->setPosition(170, 30);
		_frames[i]->addChild(_laAttack,1,14);
		//添加生命label
		sprintf(c, "%d", card.get_health());//取实际
		auto _laHealth = Tool::createEnglishLabel();
		_laHealth->setString(c);
		_laHealth->setPosition(220, 30);
		_frames[i]->addChild(_laHealth,1,15);
	}
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
	SimpleAudioEngine::getInstance()->playEffect("bgm/Select.mp3");
	Point tp = touch->getLocation();
	if (_state)
		for (int i = 0; i <_cardsInSet.size(); i++)
		{
		if (collisionCheck(tp, _cardsInSet.at(i)))
		{
			//auto id = convertToCardID(i);
			//_delCard = i;
			delCardofArray(i);
			_delCard = true;
		}
		}
	else
		for (int i = 0; i <_cardSets.size(); i++)
		{
		if (collisionCheck(tp, _cardSets.at(i)))
		{
			_selCardSet = i;
			_delSet = true;
		}
		}

	return true;
}

void Book::onTouchMoved(Touch* touch, Event* event)
{
}
void Book::onTouchEnded(Touch* touch, Event* event)
{

	Point tp = touch->getLocation();
	//加牌或看牌
	for (int i = 0; i < _curSize; i++)
	{
		if (collisionCheck(tp, _frames[i]))
		{

			auto id = convertToCardID(i);
			if (_state)
				addCardintoArray(id,true);
			else
				displayCard(id);
			return;
		}
	}
	//删牌或看卡组
	if (_state)
		for (int i = 0; i <_cardsInSet.size(); i++)
		{
			if (collisionCheck(tp, _cardsInSet.at(i)))
			{
				//auto id = convertToCardID(i);
				delCardofArray( i);
				return;
			}
		}
	else
		for (int i = 0; i <_cardSets.size(); i++)
		{
			if (collisionCheck(tp, _cardSets.at(i)))
			{
				_selCardSet = i;
				changeCardArray();
				return;
			}
		}

	if (_delSet)
	{
		delCardArray();
	}
	_delSet = false;
	_delCard = false;
	

	

	//删卡组

}


void Book::scroll(bool direction)//true=上滑,false=下滑
{
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
	_user->createCardArray(cardName, roleID);
	_newSet = true;
	char c[20];

	auto setF = Sprite::create("book/frame2.png");	
	setF->setPosition(Vec2(94, 670 - _cardSets.size() * 40));
	_cardSetBar->addChild(setF);

	sprintf(c, "book/cardset%d.png", roleID);
	auto set = Sprite::create(c);
	set->setPosition(setF->getContentSize()/2);
	setF->addChild(set);
	_cardSets.pushBack(setF);
	_selCardSet = _cardSets.size() - 1;
	changeCardArray();
	_curSize++;
}
void Book::addCardArray()						//增加卡组，添加到数据库
{
	if (_newSet)
		_user->addCardArray(_selCardSet);
	_newSet = false;
}
bool Book::delCardArray()						//删除卡组
{


	_user->delCardArray(_selCardSet);

	_cardSets.at(_selCardSet)->removeFromParent();
	_cardSets.erase(_selCardSet);
	adjustCardSets();
	return true;
}
int Book::getCardArrayNumber()								//获得所有的卡组数量
{
	//return _curSize;
	return _user->getCardArrayNumber();
}

bool Book::addCardintoArray(int cardID,bool addToSql)		//在卡组中增加卡牌
{
	if (addToSql)
		_user->addCardintoArray(_selCardSet, cardID);
	log(cardID);
	auto setF = Sprite::create("book/frame2.png");
	setF->setPosition(94, 670 - 45 - _cardsInSet.size() * 40);
	_cardSetBar->addChild(setF);

	Sprite *card = BoardCardBuilder::buildCardInSet(cardID);
	card->setPosition(setF->getContentSize()/2);

	setF->addChild(card);
	_cardsInSet.pushBack(setF);

	return true;
}
bool Book::delCardofArray(int cardID)		//删除卡牌
{
	return NULL;
}
void Book::changeCardArray()
{
	_state = true;
	_finishSet->setVisible(true);
	_cardSets.at(_selCardSet)->runAction(MoveTo::create(0.1f, Vec2(94, 670)));
	for (int i = 0; i < _cardSets.size(); i++)
	{
		if (i != _selCardSet)
			_cardSets.at(i)->setVisible(false);
	}
	_newSetBn->setVisible(false);

	auto cards=_user->getCardformArray(_selCardSet);

	for (int i = 0; i < cards.size(); i++)
	{
		int num = cards[i] / 10000000;
		auto id = cards[i] % 10000000;
		for (int j = 0; j < num;j++)
			addCardintoArray(id);
	}
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
		createCardArray("hello1",_curRole);
		
	}
		break;
	default:
		break;
	}
}


void Book::finishSetEvent(Ref*pSender, TouchEventType type)
{
	switch (type)
	{
	case TOUCH_EVENT_ENDED:
	{
		finishSet();

	}
		break;
	default:
		break;
	}
}

void Book::finishSet(){
	addCardArray();
	_finishSet->setVisible(false);
	for (int i = 0; i < _cardsInSet.size(); i++)
	{
		_cardsInSet.at(i)->removeFromParent();
	}
	_cardsInSet.clear();
	adjustCardSets();
	_newSetBn->setVisible(true);
	_state = false;

}


void Book::adjustCardSets()
{
	for (int i = 0; i < _cardSets.size(); i++)
	{
		_cardSets.at(i)->setVisible(true);
		_cardSets.at(i)->setPosition(Vec2(94, 670-i* 40));
	}
	_newSetBn->setPosition(Vec2(94, 670-_cardSets.size() * 40));
}




void Book::backEvent(Ref*pSender, TouchEventType type)
{

	SimpleAudioEngine::getInstance()->stopBackgroundMusic();
	switch (type)
	{
	case TOUCH_EVENT_ENDED:
	{
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
