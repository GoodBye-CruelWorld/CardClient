#include"BookScene.h"
#include"Tool.h"

void Book::onEnter()
{
	_initME = false;
	_state = true;
	//初始化翻页功能
	curPage = 0;
	_scr = true;//图鉴处于上翻状态
	_new = false;


	char c[20];
	for (int i = 0; i < 3; i++)
	{
		sprintf(c, "book/book%d.png", i + 1);
		_bgs[i] = Sprite::create(c);
		addChild(_bgs[i]);
		_bgs[i]->setVisible(!i);
	}
	auto s = Director::getInstance()->getWinSize();

	auto leftButton = Button::create("book/arrow2.png");
	leftButton->setPosition(Vec2(10, 164) - Vec2(311, 164));
	leftButton->addTouchEventListener(this, toucheventselector(Book::leftEvent));
	addChild(leftButton);
	auto rightButton = Button::create("book/arrow.png");
	rightButton->setPosition(Vec2(602, 164) - Vec2(311, 164));
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
	//初始化使魔图鉴
	_selectedME = -1;
	_stoneNeeds[0] = 1;
	_stoneNeeds[1] = 5;
	_stoneNeeds[2] = 10;
	_stoneNeeds[3] = 15;
	for (int i = 0; i < 35; i++)
	{
		_frames[i] = Sprite::create("book/frame.png");
		_frames[i]->setPosition(160 + i % 2 * 285, 222 - i / 2 * 85);
		_bgs[0]->addChild(_frames[i]);

		_frames[i]->setCascadeOpacityEnabled(true);
		if (i>5)
			_frames[i]->setOpacity(0);

		auto card = CCard(i);
		_BoardCards[i] = BoardCard::create(card);
		//_BoardCards[i] = Sprite::create(_BoardCards[i]->_path_icon);
		//	_BoardCards[i]->setPosition(160 + i % 2 * 300, 222 - i / 2 * 85);
		//	_bg->addChild(_BoardCards[i]);
		_BoardCards[i]->setPosition(36, 42);
		_frames[i]->addChild(_BoardCards[i]);


		_upbuttons[i] = Button::create("button/levelUp2.png");

		_upbuttons[i]->setScale(1);
		_upbuttons[i]->setPosition(Vec2(200, 32));
		_upbuttons[i]->setEnabled(false);
		_upbuttons[i]->setBright(false);
		_upbuttons[i]->setTouchEnabled(false);
		_frames[i]->addChild(_upbuttons[i]);
	}

	for (int i = 6; i < 8; i++)
	{
		_upbuttons[i]->setVisible(false);
	}

	_desLb = NULL;
	_MEdetail = NULL;
	_MEcard = NULL;



	//初始化道具图鉴

	auto batch = SpriteBatchNode::create("bag/grid.png");
	for (int i = 0; i < 34; i++)
	{
		_frames2[i] = Sprite::createWithTexture(batch->getTexture());
		_frames2[i]->setPosition(55 + 57 * (i % 10), 240 - 64 * (i / 10));
		//_frames2[i]->setVisible(false);
		_frames2[i]->setTag(-1);
		_bgs[1]->addChild(_frames2[i]);
	}

	auto batch2 = SpriteBatchNode::create("choose/10.png");
	for (int i = 0; i < 34; i++)
	{
		auto unknown = Sprite::createWithTexture(batch2->getTexture());
		unknown->setPosition(22.5, 26);
		//_frames2[i]->setVisible(false);
		unknown->setTag(10);
		_frames2[i]->addChild(unknown);
	}

	for (int i = 0; i < 20; i++)
	{
		sprintf(c, "item/%02d.png", i);
		_items[i] = Sprite::create(c);
		_items[i]->setPosition(22.5, 26);
		//_frames2[i]->setVisible(false);
		_items[i]->setTag(11);
		_items[i]->setVisible(false);
		//_items[i]->setVisible(true);
		_items[i]->setScale(0.7);
		_frames2[i]->addChild(_items[i]);
		_own[i] = false;

	}

	//addItem(2);
	//初始化反应式图鉴
	auto batch3 = SpriteBatchNode::create("book/frame.png");
	for (int i = 0; i <20; i++)
	{
		_frames3[i] = Sprite::createWithTexture(batch3->getTexture());
		_frames3[i]->setPosition(160 + i % 2 * 300, 222 - i / 2 * 85);
		//_frames2[i]->setVisible(false);
		_frames3[i]->setTag(-1);
		_bgs[2]->addChild(_frames3[i]);
	}
	for (int i = 0; i < 20; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			auto unknown = Sprite::createWithTexture(batch2->getTexture());
			unknown->setPosition(80 + 45 * j, 40);
			//_frames2[i]->setVisible(false);
			unknown->setTag(10 + j);
			_frames3[i]->addChild(unknown);
		}
	}
	for (int i = 6; i < 20; i++)
	{
		_frames3[i]->setVisible(false);
	}
	for (int i = 0; i < 12; i++)
	{
		sprintf(c, "reaction%02d", i);
		_reactions[i] = Tool::createTitleLabel();
		Tool::setLabelString(_reactions[i], c);
		_reactions[i]->setPosition(125, 40);
		//_frames2[i]->setVisible(false);
		_reactions[i]->setTag(13);
		_reactions[i]->setVisible(false);
		_reactions[i]->setScale(0.6);
		_frames3[i]->addChild(_reactions[i]);
		_own3[i] = false;

	}

}

void Book::addStone(int MEID, int num)
{
	//_BoardCards[MEID]->_stone += num;
	//char c[20];
	//sprintf(c, "stone%d", MEID);
	//UserDefault::sharedUserDefault()->setIntegerForKey(c, _BoardCards[MEID]->_stone);
	//UserDefault::sharedUserDefault()->flush();

	//int aa = _BoardCards[MEID]->_quality;
	//sprintf(c, "%d / %d", _BoardCards[MEID]->_stone, _stoneNeeds[aa]);
	//_stoneLbs[MEID]->setString(c);
	//if (_BoardCards[MEID]->_stone - _stoneNeeds[aa] >= 0)
	//{

	//	_upbuttons[MEID]->setBright(true);

	//}
}

void Book::displayME(int num)
{
	if (!_state)
		return;

	for (int i = 0; i < 8; i++)
	{

		//_frames[i]->setPosition3D(Vec3(_frames[i]->getPosition3D() + Vec3(0, 0, 30)));
		_frames[i]->runAction(Sequence::create(
			DelayTime::create(0.1 + i / 20.0),
			ScaleTo::create(0.1f, 0.f),
			MoveBy::create(0.1f, Vec3(0, 0, -30)),
			NULL));
	}


	_selectedME = num;
	auto s = Director::getInstance()->getWinSize();

	//卡牌
	_MEcard = Sprite::create();
	_MEcard->setVisible(false);
	_MEcard->setScale(1.2);
	_MEcard->setPosition3D(Vec3(112, 145, 40));
	_MEcard->runAction(Sequence::create(
		DelayTime::create(0.4),
		CallFunc::create(CC_CALLBACK_0(Node::setVisible, _MEcard, true)),
		ScaleTo::create(0.1f, 0.8f),
		MoveBy::create(0.1f, Vec3(0, 0, -40)),
		NULL));

	//卡牌值
	//名字
	auto _laName = Tool::createEnglishLabel();
	//_laName->setString(/*_BoardCards[num]->_name*/);
	_laName->setScale(0.5);
	_laName->setPosition(Vec2(195, 316 - 32));


	//创建攻击label
	char c[20];
	//sprintf(c, "%d", _BoardCards[num]->_attack);
	auto _laAttack = Tool::createEnglishLabel();
	_laAttack->setString(c);
	_laAttack->setScale(0.5);
	_laAttack->setPosition(Vec2(186, 34));

	//创建生命label

	//sprintf(c, "%d", _BoardCards[num]->_life);
	auto _laLife = Tool::createEnglishLabel();
	_laLife->setString(c);
	_laLife->setPosition(Vec2(30, 34));
	_laLife->setScale(0.5);
	_MEcard->addChild(_laAttack);
	_MEcard->addChild(_laLife);
	_MEcard->addChild(_laName);


	_line->runAction(Sequence::create(
		DelayTime::create(0.6),
		MoveBy::create(0.3f, Vec3(-75, 0, 0)),
		NULL));


	for (int i = 0; i < 4; i++)
	{
		auto bnLabel = Tool::createTitleLabel();
		sprintf(c, "MEBn%d", i);
		Tool::setLabelString(bnLabel, c);
		bnLabel->setPosition(120, 45);
		bnLabel->setScale(0.8);
		_MEButtons[i] = Button::create("book/frame.png");
		_MEButtons[i]->setPosition(Vec2(75, 230 - 50 * i));
		_MEButtons[i]->setVisible(false);
		_MEButtons[i]->setScale(0.4);
		_MEButtons[i]->runAction(Sequence::create(
			DelayTime::create(0.9 + i*0.05f),
			CallFunc::create(CC_CALLBACK_0(Node::setVisible, _MEButtons[i], true)),
			MoveBy::create(0.1f, Vec3(200, 0, 0)),
			NULL));
		_MEButtons[i]->addChild(bnLabel);

		_bgs[0]->addChild(_MEButtons[i], 0, 10 + i);
	}
	_MEButtons[0]->addTouchEventListener(this, toucheventselector(Book::introEvent));
	_MEButtons[1]->addTouchEventListener(this, toucheventselector(Book::attriEvent));
	_MEButtons[2]->addTouchEventListener(this, toucheventselector(Book::skillEvent));
	_MEButtons[3]->addTouchEventListener(this, toucheventselector(Book::returnEvent));
	_MEButtons[0]->setColor(Color3B::GREEN);


	_MEdetail = Sprite::create("book/desA.png");
	_MEdetail->setPosition3D(Vec3(554 - 75, 144, 40));
	_MEdetail->setVisible(false);
	_MEdetail->setScale(1.2);
	_MEdetail->runAction(Sequence::create(
		DelayTime::create(1.f),
		CallFunc::create(CC_CALLBACK_0(Node::setVisible, _MEdetail, true)),
		ScaleTo::create(0.1f, 1.f),
		MoveBy::create(0.1f, Vec3(0, 0, -40)),
		NULL));
	_bgs[0]->addChild(_MEdetail, 0, 14);
	_bgs[0]->addChild(_MEcard, 0, 15);


	MEIntro();



}


void Book::showText(int num)
{
	//for (int i = 0; i < 3; i++)
	//{
	//	_texts[i]->setVisible(false);
	//}
	//_texts[num]->setVisible(true);
}

void  Book::upMEQuality(int id)
{
	/*if (_BoardCards[id]->_quality == 3)
		return;
	char c[20];
	sprintf(c, "quality%d", id);
	UserDefault::sharedUserDefault()->setIntegerForKey(c, _BoardCards[id]->_quality + 1);
	UserDefault::sharedUserDefault()->flush();


	if (_BoardCards[id]->_quality == 0)
	{
		cummonAnime(id);


	}



	_BoardCards[id]->_stone -= _stoneNeeds[_BoardCards[id]->_quality];
	_stars[id][_BoardCards[id]->_quality]->setVisible(true);
	_BoardCards[id]->_quality++;
	_BoardCards[id]->updateAttribute();

	sprintf(c, "%d / %d", _BoardCards[id]->_stone, _stoneNeeds[_BoardCards[id]->_quality]);
	_stoneLbs[id]->setString(c);

	int aa = _BoardCards[id]->_quality;


	if (_BoardCards[id]->_stone - _stoneNeeds[aa] < 0)
	{

		_upbuttons[id]->setBright(false);
	}
	if (aa == 3)
		_upbuttons[id]->setBright(false);*/
}

void Book::undisplay()
{
	//for (int i = 0; i < 3; i++)
	//{
	//	if (_texts[i] != NULL)
	//	{
	//		_texts[i]->setVisible(false);
	//		_texts[i] = NULL;
	//	}
	//}
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

	_selectedME = -1;

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
	_new = false;
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


Sprite*Book::createMEPhote(int i)
{
	//if (i < 0)
	//	return NULL;
	//char c[20];
	//sprintf(c, "choose/%02d.png", i);
	//auto phote = Sprite::create(c);

	//for (int j = 0; j < this->_BoardCards[i]->_quality; j++)
	//{
	//	auto starSpr = Sprite::create("book/star.png");
	//	switch (this->_BoardCards[i]->_quality)
	//	{
	//	case 1:
	//		starSpr->setPosition(4 + 30 + 14 * j, 18);
	//		break;
	//	case 2:
	//		starSpr->setPosition(4 + 24 + 14 * j, 18);
	//		break;
	//	case 3:
	//		starSpr->setPosition(4 + 12 + 14 * j, 18);
	//		break;
	//	default:
	//		break;
	//	}
	//	starSpr->setScale(0.8);
	//	phote->addChild(starSpr);
	//}

	//sprintf(c, "LV%d", this->_BoardCards[i]->_level);
	//auto label = Tool::createEnglishLabel();
	//label->setString(c);
	//label->enableShadow(Color4B(58, 35, 10, 255), Size(0, -2));
	//label->setScale(0.3);
	//label->setPosition(34, 50 + 12);
	//phote->addChild(label);

	return NULL;// phote;
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




void Book::introEvent(Ref* sender, TouchEventType type)
{
	switch (type)
	{
	case TOUCH_EVENT_ENDED:
		for (int i = 0; i < 4; i++)
			_MEButtons[i]->setColor(Color3B::WHITE);
		_MEButtons[0]->setColor(Color3B::GREEN);
		MEIntro();
		break;
	default:
		break;
	}

}
void Book::attriEvent(Ref* sender, TouchEventType type)
{
	switch (type)
	{
	case TOUCH_EVENT_ENDED:
		for (int i = 0; i < 4; i++)
			_MEButtons[i]->setColor(Color3B::WHITE);
		_MEButtons[1]->setColor(Color3B::GREEN);
		_guide[0] = true;
		MEAttri();
		break;
	default:
		break;
	}
}
void Book::skillEvent(Ref* sender, TouchEventType type)
{
	switch (type)
	{
	case TOUCH_EVENT_ENDED:
		for (int i = 0; i < 4; i++)
			_MEButtons[i]->setColor(Color3B::WHITE);
		_MEButtons[2]->setColor(Color3B::GREEN);
		_guide[1] = true;
		MESkill();
		break;
	default:
		break;
	}
}
void Book::returnEvent(Ref* sender, TouchEventType type)
{
	switch (type)
	{
	case TOUCH_EVENT_ENDED:
		for (int i = 0; i < 4; i++)
			_MEButtons[i]->setColor(Color3B::WHITE);
		_MEButtons[3]->setColor(Color3B::GREEN);
		undisplay();
		break;
	default:
		break;
	}
}


void Book::MEIntro()
{
	for (int i = 10; i < 21; i++)
	{
		if (_MEdetail->getChildByTag(i))
		{
			_MEdetail->getChildByTag(i)->removeFromParent();
		}
	}
	char c[40];
	int num = _selectedME;
	auto nameLb = Tool::createTitleLabel();

	sprintf(c, "ME%02d", num);
	Tool::setLabelString(nameLb, c);
	nameLb->setPosition(555 - 468, 210);
	_MEdetail->addChild(nameLb, 2, 11);


	auto wordLb = Tool::createTextLabel();

	sprintf(c, "MEWord%d", num);
	Tool::setLabelString(wordLb, c);
	wordLb->setPosition(555 - 468, 160);
	wordLb->setDimensions(360, 120);
	wordLb->setScale(0.35);
	_MEdetail->addChild(wordLb, 2, 12);

	auto desLb = Tool::createTextLabel();

	sprintf(c, "MEDescribe%d", num);
	Tool::setLabelString(desLb, c);
	desLb->setPosition(555 - 468, 75);
	desLb->setDimensions(360, 400);
	desLb->setScale(0.35);
	_MEdetail->addChild(desLb, 2, 13);
}
void Book::MEAttri()
{
	//for (int i = 10; i < 21; i++)
	//{
	//	if (_MEdetail->getChildByTag(i))
	//	{
	//		_MEdetail->getChildByTag(i)->removeFromParent();
	//	}
	//}
	//char c[40];
	//int num = _selectedME;


	//Label *lbs[11];
	////名称
	//lbs[0] = Tool::createTextLabel();
	//sprintf(c, "ME%02d", num);
	//Tool::setLabelString(lbs[0], c);
	//lbs[0]->setPosition(555 - 468, 210);
	//_MEdetail->addChild(lbs[0], 2, 10);


	////化学符号
	//lbs[1] = Tool::createTextLabel();
	//Tool::setLabelString(lbs[1], "sign");
	//sprintf(c, "%d", _BoardCards[num]->_attack);
	//lbs[1]->setString(lbs[1]->getString() + c);
	//lbs[1]->setPosition(555 - 468, 190);
	//_MEdetail->addChild(lbs[1], 2, 11);


	////标签
	//lbs[2] = Tool::createTextLabel();
	//sprintf(c, "tag%d", num);
	//Tool::setLabelString(lbs[2], c);
	//lbs[2]->setPosition(555 - 468, 170);
	//_MEdetail->addChild(lbs[2], 2, 12);

	////攻击
	//lbs[3] = Tool::createTextLabel();
	//Tool::setLabelString(lbs[3], "attack");
	//sprintf(c, "%d", _BoardCards[num]->_attack);
	//lbs[3]->setString(lbs[3]->getString() + c);
	//lbs[3]->setPosition(555 - 468, 150);
	//_MEdetail->addChild(lbs[3], 2, 13);

	////生命
	//lbs[4] = Tool::createTextLabel();
	//Tool::setLabelString(lbs[4], "life");
	//sprintf(c, "%d", _BoardCards[num]->_life);
	//lbs[4]->setString(lbs[4]->getString() + c);
	//lbs[4]->setPosition(555 - 468, 130);
	//_MEdetail->addChild(lbs[4], 2, 14);

	////等级
	//lbs[5] = Tool::createTextLabel();
	//Tool::setLabelString(lbs[5], "level");
	//sprintf(c, "%d", _BoardCards[num]->_level);
	//lbs[5]->setString(lbs[5]->getString() + c);
	//lbs[5]->setPosition(555 - 468, 110);
	//_MEdetail->addChild(lbs[5], 2, 15);

	//lbs[5]->setHorizontalAlignment(kCCTextAlignmentLeft);
	////品质
	//lbs[6] = Tool::createTextLabel();
	//Tool::setLabelString(lbs[6], "quantity");
	//sprintf(c, "%d", _BoardCards[num]->_quality);
	//lbs[6]->setString(lbs[6]->getString() + c);
	//lbs[6]->setPosition(555 - 468, 90);
	//_MEdetail->addChild(lbs[6], 2, 16);

	////技能
	//lbs[7] = Tool::createTextLabel();
	//Tool::setLabelString(lbs[7], "skill");
	//lbs[7]->setPosition(555 - 468, 70);
	//_MEdetail->addChild(lbs[7], 2, 17);


	//for (int i = 0; i < 8; i++)
	//{
	//	lbs[i]->setAnchorPoint(Point(0, 0));
	//	lbs[i]->setHorizontalAlignment(TextHAlignment::LEFT);
	//	lbs[i]->setScale(0.4);
	//	lbs[i]->setPosition(30, 210 - 20 * i);
	//}


	//Label*skillLbs[3];
	//for (int i = 0; i < 3; i++)
	//{
	//	skillLbs[i] = Tool::createTextLabel();
	//	sprintf(c, "Skill%d%i", num, i);
	//	Tool::setLabelString(skillLbs[i], c);
	//	skillLbs[i]->setPosition(20 + 35 * i + 50, 70);
	//	skillLbs[i]->setScale(0.4);
	//	skillLbs[i]->setAnchorPoint(Point(0, 0));
	//	_MEdetail->addChild(skillLbs[i], 2, 18 + i);

	//	skillLbs[i]->setColor(Color3B::GRAY);
	//}
	//for (int i = 0; i < _BoardCards[num]->_quality; i++)
	//{
	//	skillLbs[i]->setColor(Color3B::GREEN);
	//}


}
void Book::MESkill()
{
	//for (int i = 10; i < 21; i++)
	//{
	//	if (_MEdetail->getChildByTag(i))
	//	{
	//		_MEdetail->getChildByTag(i)->removeFromParent();
	//	}

	//}
	//char c[40];
	//int num = _selectedME;


	//Label*skillNameLb[3];
	//Label*skillDesLb[3];
	////名称
	//for (int i = 0; i < 3; i++)
	//{
	//	skillNameLb[i] = Tool::createTextLabel();
	//	sprintf(c, "Skill%d%i", num, i);
	//	Tool::setLabelString(skillNameLb[i], c);
	//	skillNameLb[i]->setPosition(565 - 468, 215 - 60 * i);
	//	_MEdetail->addChild(skillNameLb[i], 2, 10 + i);
	//	skillNameLb[i]->setScale(0.4);

	//	skillDesLb[i] = Tool::createTextLabel();
	//	sprintf(c, "SkillDes%d%i", num, i);
	//	Tool::setLabelString(skillDesLb[i], c);
	//	skillDesLb[i]->setPosition(565 - 468, 170 - 60 * i);
	//	skillDesLb[i]->setDimensions(500, 200);
	//	_MEdetail->addChild(skillDesLb[i], 2, 13 + i);
	//	skillDesLb[i]->setScale(0.3);
	//	skillNameLb[i]->setColor(Color3B::GRAY);
	//	skillDesLb[i]->setColor(Color3B::GRAY);
	//}

	//for (int i = 0; i < _BoardCards[num]->_quality; i++)
	//{
	//	skillNameLb[i]->setColor(Color3B(127, 71, 4));
	//	skillDesLb[i]->setColor(Color3B(127, 71, 4));
	//}
}



void Book::cummonAnime(int id)
{
	_state = false;


	auto bg = Sprite::create("bg/dark.png");
	//bg->setPosition(_bgs[0]->getContentSize() / 2);
	this->addChild(bg, 10);
	bg->setOpacity(255 * 0.75);

	char c[20];
	sprintf(c, "ME/%02d.png", id);
	auto card = Sprite::create(c);
	card->setPosition(bg->getContentSize() / 2);
	bg->addChild(card);
	card->setScale(0);
	card->runAction(Sequence::create(
		DelayTime::create(0.2f),
		Spawn::create(ScaleTo::create(2, 1), RotateBy::create(2, Vec3(0, 3600, 0)), NULL),
		NULL));


	//粒子效果
	auto particle = ParticleSystemQuad::create("particles/Get.plist");
	particle->setPosition(bg->getContentSize() / 2);
	particle->setSpeedVar(0.01);
	particle->setStartSize(0.6);
	bg->addChild(particle);


	//卡牌值
	//名字
	auto _laName = Tool::createEnglishLabel();
	//_laName->setString(_BoardCards[id]->_name);
	_laName->setScale(0.5);
	_laName->setPosition(Vec2(195, 316 - 32));


	//创建攻击label

	//sprintf(c, "%d", _BoardCards[id]->_oAttack);
	auto _laAttack = Tool::createEnglishLabel();
	_laAttack->setString(c);
	_laAttack->setScale(0.5);
	_laAttack->setPosition(Vec2(186, 34));

	//创建生命label
	char l[5];
	//sprintf(l, "%d", _BoardCards[id]->_oLife);
	auto _laLife = Tool::createEnglishLabel();
	_laLife->setString(l);
	_laLife->setPosition(Vec2(30, 34));
	_laLife->setScale(0.5);
	card->addChild(_laAttack);
	card->addChild(_laLife);
	card->addChild(_laName);


	auto label = Tool::createTitleLabel(75);
	Tool::setLabelString(label, "cummon1");
	label->setPosition(bg->getContentSize() / 2 + Size(0, 200));
	bg->addChild(label);
	label->setVisible(false);
	label->setScale(1.2);
	label->setPositionZ(40);
	label->runAction(Sequence::create(
		DelayTime::create(0.2),
		CallFunc::create(CC_CALLBACK_0(Node::setVisible, label, true)),
		ScaleTo::create(0.1f, 0.8f),
		MoveBy::create(0.1f, Vec3(0, 0, -40)),
		NULL));


	bg->runAction(Sequence::create(
		DelayTime::create(2.2),
		DelayTime::create(3),
		CallFunc::create(CC_CALLBACK_0(Node::removeFromParent, bg)),
		CallFunc::create(CC_CALLBACK_0(Book::setState, this, true)),
		NULL));

}


//设置图鉴状态
void Book::setState(int state)
{
	_state = state;
}


void Book::scroll(bool direction)//true=上滑,false=下滑
{
	if (curPage != 0 || _selectedME != -1)
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


void Book::initME()
{
	if (_initME)
		return;
	_initME = true;
	char c[20];
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 3; j++)
		{

			_stars[i][j] = Sprite::create("book/star.png");

			//_stars[i][j]->setPosition(132 + i % 2 * 300+j*20, 212 - i / 2 * 85);
			//_bg->addChild(_stars[i][j]);
			_stars[i][j]->setPosition(98 + j * 20, 32);
			_frames[i]->addChild(_stars[i][j]);
		}

		auto nameLb = Tool::createTitleLabel();
		sprintf(c, "ME%02d", i);
		Tool::setLabelString(nameLb, c);
		nameLb->setScale(0.35);
		nameLb->setPosition(110, 52);
		_frames[i]->addChild(nameLb);

		_stoneLbs[i] = Tool::createEnglishLabel();

		//sprintf(c, "%d / %d", _BoardCards[i]->_stone, _stoneNeeds[_BoardCards[i]->_quality]);
		_stoneLbs[i]->setString(c);
		_stoneLbs[i]->setScale(0.4);
		_stoneLbs[i]->setPosition(200, 52);
		_frames[i]->addChild(_stoneLbs[i]);

		/*if (_BoardCards[i]->_stone - _stoneNeeds[_BoardCards[i]->_quality] >= 0)
		{

			_upbuttons[i]->setBright(true);
		}
		if (_BoardCards[i]->_quality == 3)
			_upbuttons[i]->setBright(false);*/
	}
}