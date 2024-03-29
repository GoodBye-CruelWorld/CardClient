#include "Tool2D.h"
#include"audio\include\SimpleAudioEngine.h"
using namespace CocosDenshion;
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/************************************************************初始化*******************************************************/
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BattleTool2D::BattleTool2D(GameBoard * gameboard, CBattle* battleMy, CBattle* battleIt, CBattle*battle2)
{
	//初始化变量
	_sight = 0;
	_isOneTouch = false;
	_roleWord = 0;

	_t_battleID = 0;


	// 初始化指针
	_gameboard = gameboard;
	/*_battleID = battleID;
	_battleState = battleState;*/

	_battleIt = battleIt;
	_battleMy = battleMy;
	_battle2 = battle2;

	_cmd = Command::getInstance();

	//设置鼠标准星
	_mouseSprite = Sprite::create("b.png");
	_mouseSprite->setPosition(Vec2(0,0));
	this->addChild(_mouseSprite, 1);
	_mouseSprite->setVisible(false);

	_cardSel = NULL;

}

void BattleTool2D::onEnter()
{
	Node::onEnter();
	//触摸响应注册
	auto touchListener = EventListenerTouchOneByOne::create();//创建单点触摸事件监听器
	touchListener->onTouchBegan = CC_CALLBACK_2(BattleTool2D::onTouchBegan, this);//触摸开始
	touchListener->onTouchMoved = CC_CALLBACK_2(BattleTool2D::onTouchMoved, this);//触摸中移动
	touchListener->onTouchEnded = CC_CALLBACK_2(BattleTool2D::onTouchEnded, this);//触摸结束
	_eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);//注册分发器

	//设置计时器，实时监听
	//this->scheduleUpdate();
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*********************************************************触摸处理程序****************************************************/
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool BattleTool2D::onTouchBegan(Touch* touch, Event* event)
{
	if (_battleMy->_gameState != GAME_RUN)
		return false;
	_t_battleID = 0;
	SimpleAudioEngine::getInstance()->playEffect("bgm/Select.mp3");
	_mouseDown = true;
	Point tp = touch->getLocation();
	_beginID =judgeTouchPoint(tp, 1);//区域

	switch (_beginID / 100)
	{
	case 1:
		{	//指向手牌类的指针Card
			int Number = _beginID % 10;
			//调用我方手牌类中的单位链表,找到_beginID%10所对应序列号的手牌
			if (true/*判断但前费用和Card指针所需的费用  _battleMy->_cardPool[POOL_HAND].at(Number).getCost()<=Battle.actionPoints*/)
			{
				//获得Card所指手牌类的位置坐标，改变为tp
				//将_mouseSprite图片改成对应的卡牌的图片
				for (int i = 0; i < _battleMy->_cardPool[POOL_HAND].size(); i++)
				{
					auto card = _gameboard->getCard(POOL_HAND,i,0);
					if (collisionCheck(tp, card))
					{
						_cardSel= card;
						_gameboard->selectCard(POOL_HAND,i);
						_sight=1;//显示卡牌随鼠标移动

						break;
					}
				}				
			}
			else
				_beginID = 0;
			break;
		}
	case 2:		
		break;
	case 3:																						//我方随从
		if (true/*(_beginID/10)%10!=0 判断随从能否攻击*/)
		{			
			_mouseSprite->setPosition(tp);
			_mouseSprite->setVisible(true);			
		}
		else
			_beginID = 0;
		break;
	case 4:
		break;
	case 5:
		break;
	case 6:
		break;
	case 7:
		break;
	
	case 8:																					/////////我方技能
		{
			int Number = _beginID % 10;
			auto Skill = _gameboard->getRole(0)->getRoleSkill(Number);
			this->scheduleOnce(schedule_selector(BattleTool2D::disSkilldiscribe), 0.2f);
			break;
		}
	case 9:																					/////////敌方技能
		{
			int Number =_beginID % 10;
			auto Skill=_gameboard->getRole(1)->getRoleSkill(Number);
			Skill->displayDescription();
			break; 
		}
	case 10:																				/////////我方装备							
		if (_beginID%10==3)
		{ 
			auto Equip = _gameboard->getRole(0)->getWeapon();
			if (Equip == NULL)
				_beginID = 0;
			else
				Equip->displayDescription();
		}
		else
		{ 
			auto Equip=_gameboard->getRole(0)->getEquip(_beginID%10);
			if (Equip == NULL)
				_beginID = 0;
			else
				Equip->displayDescription();
		}
		break;
	case 11:																				/////////敌方装备
		if (_beginID % 10 == 3)
		{
			auto Equip=_gameboard->getRole(1)->getWeapon();
			if (Equip == NULL)
				_beginID = 0;
			else
				Equip->displayDescription();
		}
		else
		{
			auto Equip=_gameboard->getRole(1)->getEquip(_beginID%10);
			if (Equip == NULL)
				_beginID = 0;
			else
				Equip->displayDescription();
		}
		break;
	
	default:
		break;
	}
	if (_beginID <= 0) 
		return false;

	
	return true;
}

void  BattleTool2D::onTouchEnded(Touch* touch, Event* event)
{
	Point BoardScreen = Point(1024,768);
	Point tp = touch->getLocation();
	bool judge = false;					//还原judge

	_endID =judgeTouchPoint(tp, 2);
										
	_mouseDown = false;
	 

	////////////////////////////////////////////////////////////////////////////////////////英雄表情的响应
	//判断是否单击一次,并且设置表情
	if (_isOneTouch)
	{
		if (_roleWord == 0)
		{
			if (_beginID / 100 == 6 && _endID / 100 == 6)		//我方表情选择
			{
				auto Role = _gameboard->getRole(0);
				Role->getRolePhote()->setWordChoicesVisible(true);
				_roleWord = 1;
				_beginID = 0;
			}
			if (_beginID / 100 == 7 && _endID / 100 == 7)	//敌方表情选择
			{
				auto Role = _gameboard->getRole(1);
				Role->getRolePhote()->setWordChoicesVisible(true);
				_roleWord = 2;
				_beginID = 0;
			}
		}
		else
		{
			if (_roleWord == 1)			//显示表情
			{
				auto Role = _gameboard->getRole(0);
				Role->getRolePhote()->setWordChoicesVisible(false);
				int number=judgePointIn(tp, Vec2(425, 262), Vec2(587, 262), Vec2(425, 119), Vec2(587, 116));
				if (number)
					Role->getRolePhote()->speakWord(number - 1);
					
				_roleWord = 0;
				_beginID = 0;
			}
			if (_roleWord == 2)			//显示表情
			{
				auto Role = _gameboard->getRole(1);
				Role->getRolePhote()->setWordChoicesVisible(false);
				_roleWord = 0;
				_beginID = 0;
			}			
		}
	}
	_isOneTouch = false;
	
	///////////////////////////////////////////////////////////////////////////////////////英雄技能的使用
	//判断非指向技能的使用
	if ((_beginID / 100 == 8) && (_beginID % 100 / 10 == 0))
	{
		int Number = _beginID % 10;
		auto Skill=_gameboard->getRole(0)->getRoleSkill(Number);
		Skill->hideDescription();
		//使用技能的效果
		Skill->useSkill();	

		_t_battleID = 3 * 1000000 + 7 * 10000 + _beginID % 10 * 1000 ;
	}
	//指向性技能的使用
	if ((_beginID / 100 == 8) && (_beginID % 100 / 10 == 1))
	{
		int Number = _beginID % 10;
		auto Skill = _gameboard->getRole(0)->getRoleSkill(Number);
		Skill->hideDescription();

		//判断技能是否正确指向
		if ((_endID / 100 == 3) || (_endID / 100 == 4) || (_endID / 100 == 5) || (_endID / 100 == 6) || (_endID / 100 == 7))
		{
			int Number = _battleMy->_cardPool[POOL_BATTLE].size();

			int  result = 0;
			//判断能否召唤（指向的位置有随从）
			if (_endID / 100 == 3)
			{
				if (_battleMy->_cardPool[POOL_BATTLE].size() == 0)
				{
					//移除鼠标的准星
					//_mouseSprite的图片还原成准星
					_mouseSprite->setVisible(false);
					_sight = 0;
					return;
				}
				result = -1;
				for (int i = 0; i < _battleMy->_cardPool[POOL_BATTLE].size(); i++)
				{

					if (collisionCheck(Vec2(355 + _endID % 10 * 80, 326), _gameboard->getCard(POOL_BATTLE, i, 0)))
					{
						result = i;
						break;
					}
				}
				if (result == -1)
				{

					//移除鼠标的准星
					//_mouseSprite的图片还原成准星
					_mouseSprite->setVisible(false);
					_sight = 0;
					return;

				}
			}

			if (_endID / 100 == 4)
			{
				result = _endID % 10;
			}

			if (_endID / 100 == 6 || _endID / 100 == 7)
				result = 0;

			//使用技能的动画
			Skill->useSkill();
			_t_battleID = 3 * 1000000 + 8 * 10000 + _beginID % 10 * 1000 + _endID/100 * 10 + result;
		}		
	}



	////////////////////////////////////////////////////////////////////////////////////关于敌方 装备 技能 信息的显示与取消
	//取消装备技能的信息显示
	if ((_beginID / 100 == 9))
	{
		int Number = _beginID % 10;
		auto Skill = _gameboard->getRole(1)->getRoleSkill(Number);
		Skill->hideDescription();	
	}
	if (_beginID / 100 == 11)
	{
		
		int Number = _beginID % 10;
		if (Number==3)				//判断是否是武器，3为武器
		{ 
			auto Equip = _gameboard->getRole(1)->getWeapon();
			Equip->hideDescription();
		}
		else
		{
			auto Equip = _gameboard->getRole(1)->getEquip(_beginID % 10);
			Equip->hideDescription();
		}
	
	}




	///////////////////////////////////////////////////////////////////////////////////我方手牌的使用	
	if (_beginID / 100 == 1)
	{

		//解决卡牌原地单机无法还原的问题
		if (_sight != 2 && _sight != -1)
			_gameboard->setCardOraginState();//还原卡牌位置	
		
		
		//使用非指向性牌
		if (_sight==2)
			if (tp.y>BoardScreen.y/4)															
			{
				//判断_beginID，获得该手牌数据
				auto _cradID = _battleMy->_cardPool[POOL_HAND].at(_beginID % 10).get_cardID();
				switch ((_cradID / 1000) % 10)
				{
				case 0:		//表示是随从
				{
					//判断鼠标抬起的落点是否在我方随从区
					if ((_endID / 100 == 3) && _battleMy->actionPoints >= _battleMy->_cardPool[POOL_HAND].at(_beginID % 10).get_cost())		//在随从区,并且该位置无随从
					{			
						//获得我方随从区的随从个数
						int Number = _battleMy->_cardPool[POOL_BATTLE].size();	

						//判断指定位置是否已经有随从了	
						bool result = false; //用于判断是否有随从的指标
						for (int i = 0; i < Number; i++)							
							if (collisionCheck(Vec2(355 + _endID % 10 * 80, 326), _gameboard->getCard(POOL_BATTLE, i, 0)))
								result = true;
						if (result)
						{
							_gameboard->setCardOraginState();//还原卡牌位置	
							break;
						}

						_battleMy->_cardPool[POOL_HAND].at(_beginID % 10).set_pos(_endID % 10);
						//调用Cbattle类的随从召唤				
						_t_battleID = 1 * 1000000 + 01 * 10000 + _beginID % 10 * 1000 + 100 * (_endID % 10) + 03 * 10 + Number % 10;
					}
					else
						_gameboard->setCardOraginState();//还原卡牌位置	
					break;
				}
				case 1:		//表示为法术
				{
					//判断鼠标抬起的落点是否在手牌区外
					if ((_endID / 100 != 1)  &(_endID != 0)&  _battleMy->actionPoints >= _battleMy->_cardPool[POOL_HAND].at(_beginID % 10).get_cost())		//在随从区,并且该位置无随从
					{
						int Number = _battleMy->_cardPool[POOL_BATTLE].size();


						//调用Cbattle类的随从召唤				
						_t_battleID = 2 * 1000000 + 01 * 10000 + _beginID % 10 * 1000;
					}
					else
						_gameboard->setCardOraginState();//还原卡牌位置	
					break;
				}
				case 2:		//表示为武器，传值判断与法术一样
				{
					//判断鼠标抬起的落点是否在手牌区外
					if ((_endID / 100 != 1)  &(_endID != 0)&  _battleMy->actionPoints >= _battleMy->_cardPool[POOL_HAND].at(_beginID % 10).get_cost())		//在手牌区外
					{
						int Number = _battleMy->_cardPool[POOL_BATTLE].size();

						_gameboard->getRole(0)->addWeapon(_cradID);
						//调用Cbattle类的随从召唤				
						_t_battleID = 2 * 1000000 + 01 * 10000 + _beginID % 10 * 1000;
					}
					else
						_gameboard->setCardOraginState();//还原卡牌位置	
					break;
				}
				case 3:		//表示为装备，传值判断与法术一样
				{
					//判断鼠标抬起的落点是否在手牌区外
					if ((_endID / 100 != 1)  &(_endID != 0)&  _battleMy->actionPoints >= _battleMy->_cardPool[POOL_HAND].at(_beginID % 10).get_cost())		//在手牌区外
					{
						int Number = _battleMy->_cardPool[POOL_BATTLE].size();

						_gameboard->getRole(0)->addWeapon(_cradID);
						//调用Cbattle类的随从召唤				
						_t_battleID = 2 * 1000000 + 01 * 10000 + _beginID % 10 * 1000;
					}
					else
						_gameboard->setCardOraginState();//还原卡牌位置	
					break;
				}
				default:
					break;
				}
				//还原目前手牌的数据或放出手牌	
				_sight = 0;

			}
			else
				_gameboard->setCardOraginState();//还原卡牌位置

		//TODO 按照无指向的随从的使用更改

		//使用指向性的手牌
	
			
		if ((_sight == -1))
			if (((_endID / 100 == 3) || (_endID / 100 == 4) || (_endID / 100 == 5) || (_endID / 100 == 6) || (_endID / 100 == 7)))
			{
				auto _cradID = _battleMy->_cardPool[POOL_HAND].at(_beginID % 10).get_cardID();
				switch ((_cradID / 1000) % 10)
				{
				case 0:		//表示是随从
				{
					int Number = _battleMy->_cardPool[POOL_BATTLE].size();

					int  result = 0;
					//判断能否召唤（指向的位置有随从）,同时判断战吼的指向是否正确
					if (_endID/100==3)
					{
						if (_battleMy->_cardPool[POOL_BATTLE].size() == 0)
							result = -1;
						result = -1;
						for (int i = 0; i < _battleMy->_cardPool[POOL_BATTLE].size(); i++)
						{
						
							if (collisionCheck(Vec2(355 + _endID % 10 * 80, 326), _gameboard->getCard(POOL_BATTLE, i, 0)))
							{ 
								result = i;
								break;
							}						
						}
						if (result == -1)
						{
							break;
						}
					}

					if (_endID / 100 == 4)
					{
						if (_battleIt->_cardPool[POOL_BATTLE].size() == 0)
							break;
						result = _endID%10;
					
					}
					
					if (_endID / 100 == 5)
					{
						result = _endID % 10;
					}
					//获得该指向性战吼的ID
					int spellID = -1;
					for (int i = 0; i < _battleMy->_cardPool[POOL_HAND].at(_beginID % 10).get_spellID().size(); i++)
						if (_battleMy->_cardPool[POOL_HAND].at(_beginID % 10).get_spellID().at(i) / 100 % 10 >= 1)
							spellID = _battleMy->_cardPool[POOL_HAND].at(_beginID % 10).get_spellID().at(i);

					if (!judgeAimed(RETINUE, spellID, _endID / 100 , result))
					{
						break;
					}
					//判断指向性随从的放置位置
					//从左到右找一个没有随从的空位，将该随从放入
					for (int j = 0; j < 5; j++)
					{
						//检测某个位置是否已有随从
						bool s = true;
						for (int i = 0; i < _battleMy->_cardPool[POOL_BATTLE].size(); i++)
						{
							auto card = _gameboard->getCard(POOL_BATTLE,i,0);
							if (collisionCheck(Vec2(355 + j * 80, 326), card))
							{
								s = false;
								break;
							}
						}
						if (s)
						{
							//设置改随从的显示
							_gameboard->getCard(POOL_HAND,_beginID % 10,0)->setPosition(Vec2(-155 + 80 * j, -65));
							//设置随从的实际位置
							_battleMy->_cardPool[POOL_HAND].at(_beginID % 10).set_pos(j);
							//设置BattleID
							_t_battleID = 1 * 1000000 + 01 * 10000 + _beginID % 10 * 1000+j*100 + _endID/100 * 10 +result;

							break;
						}
					}
			
				
					break;
				}
				case 1:		//表示为法术
				{
					//result既判定法术是否指定成功，又储存指定目标的数组位置
					int result = -1;
					//指向性法术的使用
					if (_endID / 100 == 3)
					{
						if (_battleMy->_cardPool[POOL_BATTLE].size() == 0)
							break;
					
						result = -1;
						for (int i = 0; i < _battleMy->_cardPool[POOL_BATTLE].size(); i++)
						{

							if (collisionCheck(Vec2(355 + _endID % 10 * 80, 326), _gameboard->getCard(POOL_BATTLE, i, 0)))
							{
								result = i;
								break;
							}
						}
						if (result == -1)
							break;
					
					}
					//指向敌方随从
					if (_endID / 100 == 4)
					{
						result = _endID % 10;
					}
					//指向英雄
					if (_endID / 100 == 7 || _endID / 100 == 6)
					{
						result = 0;
					}
					//指向野怪区
					if (_endID / 100 == 5)
					{
						result = _endID % 10;
					}
					//如果result=-1表示指向失败
					if (result == -1)
						break;

					//获得法术的SPELLID
					int spellID = -1;
					for (int i = 0; i < _battleMy->_cardPool[POOL_HAND].at(_beginID % 10).get_spellID().size(); i++)
						if (_battleMy->_cardPool[POOL_HAND].at(_beginID % 10).get_spellID().at(i) / 100 % 10 >= 1)
							spellID = _battleMy->_cardPool[POOL_HAND].at(_beginID % 10).get_spellID().at(i);

					if (!judgeAimed(SPELL, spellID, _endID / 100, result))
					{
						break;
					}

					_t_battleID = 2 * 1000000 + 01 * 10000 + _beginID % 10 * 1000 + _endID / 100 * 10 + result;
				

					break;
				}
				default:
					break;
				}
				if (_t_battleID==0)
					_gameboard->setCardOraginState();
			}
			else
			{
				_gameboard->setCardOraginState();
			}
		
	}

	//指向性战吼的手牌使用的特殊情况，手牌未移除手牌区
	if (_sight == 3)
		_gameboard->setCardOraginState();

	////////////////////////////////////////////////////////////////////////////////////////////////随从攻击
	if (_beginID / 100 == 3)
	{
		if ((_endID / 100 == 4) || (_endID / 100 == 5) || (_endID / 100 == 7))
		{

			_t_battleID = 4 * 1000000 + 3 * 10000 + _beginID % 10 * 1000 + _endID / 100 * 10 + _endID % 10;					
		}		
	}

	///////////////////////////////////////////////////////////////////////////////////////////////英雄攻击
	if (_beginID / 100 == 6)
	{
		if ((_endID / 100 == 4) || (_endID / 100 == 5) || (_endID / 100 == 7))
		{
			_t_battleID = 4 * 1000000 + _beginID / 100 * 10000 + _beginID % 10 * 1000 + _endID / 100 * 10 + _endID % 10;
		}
	}

	//////////////////////////////////////////////////////////////////////////////////////////////判断回合结束
	
	if ((_endID / 100 == 12) && (_beginID/100==12))
	{
		_t_battleID = 6 * 1000000;	
	}
	
	
	

	//重新初始化_cardSel
	
	_cardSel = NULL;

	//移除鼠标的准星
	//_mouseSprite的图片还原成准星
	_mouseSprite->setVisible(false);
	_sight = 0;

	//还原
	_beginID = 0;
	_endID = 0;


	//信息交互
	if (_t_battleID != 0)
	{
		_cmd->sendCommand(_t_battleID);
		_t_battleID= 0;
	}

}

void  BattleTool2D::onTouchMoved(Touch* touch, Event* event)
{
	Point tp = touch->getLocation();

	if ((_sight != -1) && (_sight != 2) && (_sight != 3))//-1表示图片正移动中，不进行其他判断,1表示变成准星，3表示指向性战吼手牌的使用，2表示手牌随鼠标移到
	{
		switch (_beginID / 100)
		{
		case 1:
		{
			auto Card = _battleMy->_cardPool[POOL_HAND].at(_beginID % 10);
			if (!judgeCard(&_battleMy->_cardPool[POOL_HAND].at(_beginID % 10), AIM)/*(_beginID%100)/10==0判断是否是 非指向道具和法术、随从（无指向性降临效果）*/)
			{

				_sight = 2;
			}
			else
			{
				_sight = 3;
			}
			break;
		}
		case 3:
			
			//_sight = 1;
			if ((_battleMy->_cardPool[POOL_BATTLE].at(_beginID % 10).get_canAttack()) && (!_battleMy->_cardPool[POOL_BATTLE].at(_beginID % 10).get_isAttack()))
				_sight = 1;	
			else
			{
				//提示不能攻击
				_gameboard->getRole(0)->getRolePhote()->speakWord(1);
				_beginID = 0;				
			}
			break;
		case 6:
			//if ((_beginID % 100) / 10)//英雄可攻击
			if (_gameboard->getRole(0)->_hero.get_canAttack() && !_gameboard->getRole(0)->_hero.get_isAttack())
			{
				_sight = 1;
			}
			else
			{
				//进行提醒英雄不可攻击
				_gameboard->getRole(0)->getRolePhote()->speakWord(1);
				_beginID = 0;
			}

			break;
		case 8:
			if ((_beginID % 100) / 10)//技能指向性
			{
				_sight = 1;				
			}
			break;
		default:
			break;
		}
	}

	if (_sight == 3 && tp.y > Point(1024, 768).x * 1 / 7)
	{
		_sight = 1;
	}

	
	if (_sight == 1)					//设定鼠标正在移动，取消再一次的判定
	{
		_sight = -1;

		_mouseSprite->setPosition(tp);
		_mouseSprite->setVisible(true);
	}
	
	if (_sight==-1)					//_sight==-1 表示鼠标变成准星移动
		_mouseSprite->setPosition(tp);
	if (_sight == 2 || _sight == 3)
		_cardSel->setPosition(tp - Vec2(1024 / 2, 768 / 2));

}



///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**********************************************************私有方法*******************************************************/
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int BattleTool2D::judgeTouchPoint(cocos2d::Point tp, int PartID)
{	//1024*768
	Point BoardScreen=Point(1024,768);
	//BoradScreen=获得棋盘的大小
	//调用


	 
	///////////////////////////////////////////////////////////////////////////////////////////////////////////判断表情的使用
	if (_roleWord!=0)
	{
		_isOneTouch = true;
		this->scheduleOnce(schedule_selector(BattleTool2D::resetOneTouch), 0.5f);
		return   600 + 0 * 10 + 0;	
	}
	else
	///////////////////////////////////////////////////////////////////////////////////////////////////////////我方牌库
	if (false/**/ )
	{		
		
		
		
		
	}
	else
	///////////////////////////////////////////////////////////////////////////////////////////////////////////敌方牌库
	if (false/**/)
	{
	
	
	}
	else
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////我方英雄
	if (judgePointIn(tp, Vec2(512, 160))/*tp判定是否是己方英雄范围*/)
	{
		
		_isOneTouch = true;
		this->scheduleOnce(schedule_selector(BattleTool2D::resetOneTouch), 0.5f);
		
				
		if (_gameboard->getRole(0)->getAttack()!=0)
		{
			return  600 + 1 * 10 + 0;	//1表示可以攻击
		}
		else
			return  600 + 0 * 10 + 0;
	}
	else
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////敌方英雄
	if (judgePointIn(tp, Vec2(512, 615))/*tp判定是否是敌方英雄范围*/)
	{
		
		_isOneTouch = true;
		this->scheduleOnce(schedule_selector(BattleTool2D::resetOneTouch), 0.5f);
		return   700 + 0 * 10 + 0;
				
	}
	else
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////判断结束按钮
	if (collisionCheck(tp, _gameboard->_endTurnButton) && _gameboard->_endTurnButton->isEnabled())
	{		
		return 1200 + 0*10 + 0;
	}
	else
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////野怪区
	if (judgePointIn(tp, Vec2(175, 440), Vec2(245, 440), Vec2(245, 350)) || judgePointIn(tp, Vec2(175, 350))/*tp判定是否是野怪区范围*/)
	{
		int number = judgePointIn(tp, Vec2(175, 440), Vec2(245, 440), Vec2(175, 350)) - 1;
		if (number == -1)
			number = judgePointIn(tp, Vec2(245, 350))?3:-1;
		if (number == -1)
			return 0;

		for (int i = 0; i < _battle2->_cardPool[POOL_BATTLE].size(); i++)
		{
			if (_battle2->_cardPool[POOL_BATTLE].at(i).get_pos() == number)
				return 500 + 0 * 10 + i;
		}
		
		return 0;
		//return   500 + 0 * 10 + number;
	}
	else
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////我方技能
	if (judgePointIn(tp, Vec2(650, 191), Vec2(733, 214), Vec2(811, 207)))
	{
		int Number = 0;
		//判断第几个技能，返回Number
		Number =judgePointIn(tp, Vec2(650, 191), Vec2(733, 214), Vec2(811, 207)) - 1;
		//调用英雄类，获得技能数据
		int SkillID = 0;
		auto Skill=_gameboard->getRole(0)->getRoleSkill(Number);
		SkillID=Skill->getSkillID();

		//判断技能是否指向
		bool  Direct = false;
		
		if (SkillID == 10 || SkillID == 0)
			Direct = true;

		
		

		
		if (Direct)
		{
			return 800 + 1 * 10 + Number;//1表示指向性
		}
		else
			return 800 + 0 * 10 + Number;
	}
	else
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////敌方技能
	if (judgePointIn(tp, Vec2(645, 589), Vec2(726, 567), Vec2(805, 574))/*判定是否是敌方方技能的范围*/)
	{
		int Number = 0;
		//判断第几个技能，返回Number
		Number =judgePointIn(tp, Vec2(645, 589), Vec2(726, 567), Vec2(805, 574));

		if (Number == -1)
			return 0;
		else
			return 900 + 0 * 10 + Number-1;
	}
	else
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////我方装备
	if (judgePointIn(tp, Vec2(377,191), Vec2(294,213), Vec2(215,205))/*判定是否是我方装备的范围*/)
	{
		int Number = 0;

		Number =judgePointIn(tp, Vec2(377, 191), Vec2(294, 213), Vec2(215, 205));
		
		//判断第几个装备，返回Number
		if (Number == -1)
			return 0;
		else
			return 1000 + 0 * 10 + Number;

	}
	else 
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////敌方装备
	if (judgePointIn(tp, Vec2(374, 588), Vec2(295, 567), Vec2(215, 570))/*判定是否是敌方装备的范围*/)
	{
		int Number = 0;

		Number =judgePointIn(tp, Vec2(374, 588), Vec2(295, 567), Vec2(215, 570))-1;
		
		//判断第几个装备，返回Number
		if (Number == -1)
			return 0;
		else
			return 1100 + 0 * 10 + Number;

	}
	else
	////////////////////////////////////////////////////////////////////////////////////////////////////////////我方手牌区判定范围
	if (tp.y < BoardScreen.y * 1 / 4)
	{
		

		
		int Number = -1;
		int Type = 0;
		//指向手牌类的指针Card
		for (int i = 0; i < _battleMy->_cardPool[POOL_HAND].size(); i++)
		{
			auto card = _gameboard->getCard(POOL_HAND,i,0);
			if (collisionCheck(tp, card/*->_frame*/))
			{
				Type = judgeCard(&_battleMy->_cardPool[POOL_HAND].at(i), AIM);
				_cardSel = card;
				return  100 + Type * 10 + i;
				
			}
		}

		return 0;
		//调用我方手牌类中的单位链表，逐一判断
		//如果点中，返回点中的牌的序列号（链表中的第几张）Number,未点中Number=-1


	}
	else
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////我方随从
	if (tp.y < BoardScreen.y * 2 / 4)
	{
		int Number = -1;//number用于判断当前点的是第几个随从
		int Type = 0;
		

		//鼠标按下时判断	
		if (PartID == 1)
		{
			//指向手牌类的指针Card
			for (int i = 0; i < _battleMy->_cardPool[POOL_BATTLE].size(); i++)
			{
				auto card = _gameboard->getCard(POOL_BATTLE,i,0);
				if (collisionCheck(tp, card))
				{
					Number = i;
				}
			}

			if ((Number == -1) && (PartID == 1))	//表示未点中随从
				return 0;

			//判读该处是否有卡牌
			if (/*false!*/_battleMy->_cardPool[POOL_BATTLE].at(Number).get_health() != 0)
			{
				//判断随从能否攻击
				
				return  300 +judgeCard(&_battleMy->_cardPool[POOL_BATTLE].at(Number), ATTACK) * 10 + Number;
			}
		}
		//鼠标抬起时判断
		else	
		{
			Number =judgePointIn(tp, Vec2(610, 326), Vec2(695, 326), Vec2(777, 326)) - 1 + 3;
			if (Number == 2)
				Number =judgePointIn(tp, Vec2(355, 326), Vec2(440, 326), Vec2(525, 326)) - 1;
			//十位暂定为什么任何用
			return	300 + 0 * 10 + Number;
				
		
		}
	}
	else
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////敌方随从
	if (tp.y < BoardScreen.y * 3 / 4)
	{
		int Number = -1;
		
		//判读点在了第几个框框内
		
		
		//Number =judgePointIn(tp, Vec2(610, 455), Vec2(695, 455), Vec2(777, 455)) - 1 + 3;
		//if (Number == 2)
		//	Number =judgePointIn(tp, Vec2(355, 455), Vec2(440, 455), Vec2(525, 455)) - 1;
		////判断该位置是否有卡牌
		//if (true/*_battleIt->_cardPool[POOL_BATTLE].at(Number).Health == 0*/)
		//	Number = -1;									//表示该处没有卡牌
		
		
		//指向手牌类的指针Card
		for (int i = 0; i < _battleIt->_cardPool[POOL_BATTLE].size(); i++)
		{
			auto card = _gameboard->getCard(POOL_BATTLE,i,1);
			if (collisionCheck(tp, card))
			{
				Number = i;
			}
		}		
		
		//如果点中，返回点中的随从的序列号（链表中的第几张）Number,未点中Number=-1
		if (Number != -1)
		{
			//判断Retinue的类型，返回一个值Type
			return  400 + 0 * 10 + Number;
		}
	}
	else
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////敌方手牌
	if (tp.y > BoardScreen.y * 3 / 4)
	{
		//暂时未想到有何用
		return 0;
	}
					

	return 0;
}

void BattleTool2D::resetOneTouch(float dt)
{
	_isOneTouch = false;
}

void BattleTool2D::disSkilldiscribe(float dt)
{
	if (_beginID > 0)
	{
		int Number = _beginID % 10;
		auto Skill = _gameboard->getRole(0)->getRoleSkill(Number);
		Skill->displayDescription();		
	}
}

int BattleTool2D::judgeCard(CCard *_card, int judgeType)
{
	switch (judgeType)
	{
	case AIM:	//	判断随从或法术是否是指向性
	{		
		for (int i = 0; i < _card->get_spellID().size(); i++)
		{
			if ((_card->get_spellID().at(i) / 100) % 10 >= 1)
				return 1;
		}

			
		return 0;
		break;
	}
	case ATTACK:
	{
		//判断攻击力
		if (!_card->getFinalAttack())
			return 0;
		//判断身上的Debuff
		if (false)
			return 0;
		
		return 1;

		break;
	}

	default:
		break;
	}

	
}


bool BattleTool2D::judgeAimed(int type, int ID, int destType, int destNum)
{
	//先判断技能使用的范围
	bool result = false;
	int num = ID / 100 % 10;
	switch (num)
	{
	case 1:
		result = true;
		break;
	case 2:
		if (destType == 3 || destType == 6)
			result = true;
		break;
	case 3:
		if (destType == 4 || destType == 7)
			result = true;
		break;
	case 4:
		if (destType == 7 || destType == 6)
			result = true;
		break;
	case 5:
		if (destType == 6)
			result = true;
		break;
	case 6:
		if (destType == 7)
			result = true;
		break;
	case 7:
		if (destType == 3 || destType == 4)
			result = true;
		break;
	case 8:
		if (destType == 3)
			result = true;
		break;
	case 9:
		if (destType == 4)
			result = true;
		break;
	default:
		break;
	}
	
	if (!result)
		return false;

	//判定特殊情况，例如免疫指向性战吼,野怪区特殊判断
	if ((destType == 3 || destType == 4) && _endID / 100 != 5)
	{
		CCard* _card = NULL;
		if (destType == 4)
			_card = &_battleIt->_cardPool[POOL_BATTLE].at(destNum);
		else
			_card = &_battleMy->_cardPool[POOL_BATTLE].at(destNum);
		//判断免疫指向性战吼
		for (int i = 0; i < _card->get_spellID().size(); i++)
		{
			if (_card->get_spellID().at(i) == 90099 && type == RETINUE)
				result = false;
		}
	
	}

	if (!result)
		return false;
}

bool BattleTool2D::judgePointIn(Vec2 p, Vec2 p0)
{
	if (((p.x<p0.x + 27) && (p.x>p0.x - 27)) && ((p.y<p0.y + 27) && (p.y>p0.y - 27)))
		return true;
	return false;
}

int BattleTool2D::judgePointIn(Vec2 p, Vec2 p0, Vec2 p1, Vec2 p2)
{
	if (((p.x<p0.x + 27) && (p.x>p0.x - 27)) && ((p.y<p0.y + 27) && (p.y>p0.y - 27)))
		return 1;
	if (((p.x<p1.x + 27) && (p.x>p1.x - 27)) && ((p.y<p1.y + 27) && (p.y>p1.y - 27)))
		return 2;
	if (((p.x<p2.x + 27) && (p.x>p2.x - 27)) && ((p.y<p2.y + 27) && (p.y>p2.y - 27)))
		return 3;

	return 0;
}

int BattleTool2D::judgePointIn(Vec2 p, Vec2 p0, Vec2 p1, Vec2 p2, Vec2 p3)
{
	if (((p.x<p0.x + 27) && (p.x>p0.x - 27)) && ((p.y<p0.y + 27) && (p.y>p0.y - 27)))
		return 1;
	if ((p.x<p1.x + 27) && (p.x>p1.x - 27) && (p.y<p1.y + 27) && (p.y>p1.y - 27))
		return 2;
	if ((p.x<p2.x + 27) && (p.x>p2.x - 27) && (p.y<p2.y + 27) && (p.y>p2.y - 27))
		return 3;
	if ((p.x<p3.x + 27) && (p.x>p3.x - 27) && (p.y<p3.y + 27) && (p.y>p3.y - 27))
		return 4;

	return 0;
}

bool BattleTool2D::collisionCheck(Point p, Node *node)
{
	auto s = Director::getInstance()->getWinSize();
	auto a = node->getPosition()+s/2;
	auto scale = node->getScale();
	Node*parent = node->getParent();
	while (parent != _gameboard)
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




int  BattleTool2D::cardPositionCheck(Node *node)
{
	for (int j = 0; j <= 4; j++)
	{
		Point p = Vec2(355 + j * 80, 326);
		auto s = Director::getInstance()->getWinSize();
		auto a = node->getPosition() + s / 2;
		auto scale = node->getScale();
		Node*parent = node->getParent();
		while (parent != _gameboard)
		{
			a += parent->getPosition() - parent->getContentSize() / 2;
			parent = parent->getParent();
		}
		auto b = node->getContentSize() / 2 * node->getScale();

		if (p.x<(a.x + b.width) && p.x>(a.x - b.width) /*&& p.y<(a.y + b.height) && p.y>(a.y - b.height)*/)
			return j;
	}

	return -1;
}

int	 BattleTool2D::cardPositionCheck(int cardPool, int num, int camp)
{

	return  cardPositionCheck(_gameboard->getCard(cardPool, num, camp));


}
