#include "Battle.h"
#include "AIEnemy.h"
#include "Command.h"
CBattle::CBattle(GameBoard * gameboard,  int *cardId, int camp, GameSocket *gameSocket, bool gameMode, bool firstHand)
{

	// 储存两者的指针
	_battleID =0;
	_battleState = false;
	_gameState = GAME_BEGIN;
	_gameboard = gameboard;
	_camp = camp;
	_hero = _gameboard->getRole(camp);
	_cardIds = cardId;
	_gameSocket = gameSocket;
	_gameMode = gameMode;
	_firstHand = firstHand;
	actionPoints = ActPtsMax = 1;
};

void CBattle::setWild(CBattle *e){
	_wild = e;
}

void CBattle::setEnemy(CBattle *e)
{
	_enemy = e;
}

void attackReset(CCard&card){
	card._attacktime = 1;
	card.buffCheck(0);
	card.canAttack();
}

void CBattle::turnStart(){
	//武器攻击初始化
	//for (int i = 0;i<her)
	int p = _hero->checkWeapon();
	if (p > 0)
		_hero->_hero._attackBattle += _hero->_equip[p]._attackBattle;

	//回合开始 行动上限+1 行动充满 检测上限点数 抽牌 待完善
	actionPoints = ActPtsMax++;
	_gameState = GAME_RUN;
	//

	if (_camp == 0)
		_gameboard->startPlayerTurn();
	else
		_gameboard->startAiTurn();


	_gameboard->getActionPointBar(_camp)->addTotalActionPoint();
	_gameboard->getActionPointBar(_camp)->fullAvailActionPoint();
	//?检测行动
	drawCard();
	_gameboard->getActionQueue()->reset(false);
	//ai->AiTurn();
	//技能发动检测
	spellCheck(00);
	check();


	//将上回合的随从变成可以攻击

	//_hero->_hero
	attackReset(_hero->_hero);
	for (int i = 0; i < _cardPool[POOL_BATTLE].size(); i++)
	{
		attackReset(_cardPool[POOL_BATTLE].at(i));
	}


	//如果是敌方,转到Ai中
	if (_camp)
	{
		turning();
	}
}

void CBattle::turning()
{
	if (!_gameMode)//如果是单机模式
	{
		auto t = ai->AiTurn();
		_gameboard->getActionQueue()->reset(false);
		this->runAction(Sequence::create(
			DelayTime::create(t),
			CallFunc::create(CC_CALLBACK_0(CBattle::turnOver, this)),
			NULL));
	}
	else
	{
		this->runAction(Sequence::create(
			DelayTime::create(0.5f),
			CallFunc::create(CC_CALLBACK_0(GameSocket::recvMsg,_gameSocket)),
			CallFunc::create(CC_CALLBACK_0(cocos2d::log, "turning recv")),
			NULL));
		//_gameSocket->recvMsg();
		//log("turning recv");
	}

}

void CBattle::turnOver()
{
	//武器结束
	int p = _hero->checkWeapon();
	if (p > 0)
		_hero->_hero._attackBattle -= _hero->_equip[p]._attackBattle;
	//buff check
	for (int i = 0; i < _cardPool[POOL_CEME].size(); i++){
		if (_cardPool[POOL_CEME][i].buffCheck(1, 5)){
			bool f = true; int pos = 0;
			for (int j = 0; j < 5; j++){
				f = true;
				for (int k = 0; k < _cardPool[POOL_BATTLE].size(); i++){
					if (_cardPool[POOL_BATTLE][k].get_pos() == j){
						f = false;
						break;
					}
				}
				if (f){ pos = j; break; }
			}
			if (f){
				_cardPool[POOL_CEME][i].relife();
				_cardPool[POOL_CEME][i]._healthBattle = 1;
				cardTransfer(POOL_CEME, POOL_BATTLE, i, 0, pos);
				i--;
			}
		}
	}
	//buff reduce
	for (int j = 0; j < 4; j++){
		for (int i = 0; i < _cardPool[j].size(); i++){
			_cardPool[j][i].reduceBuffTimes();
		}
		for (int i = 0; i < _enemy->_cardPool[j].size(); i++){
			_enemy->_cardPool[j][i].reduceBuffTimes();
		}
	}

	if (rand() % 4 == 3) _wild->addWild();

	spellCheck(01);
	check();
	if (_camp)
	{

		if (_gameMode)
			_gameSocket->_recv = false;
	}
	
	//发送回合开始的命令
	_gameState = GAME_WAIT;
	_enemy->turnStart();
}
void CBattle::actionChange(int num){
	actionPoints += num;
}


vector<CCard>& CBattle::checkPosition(CCard &card){
	return _cardPool[card.get_state()];
}
void CBattle::spellCheck(int sTime){
	//检测玩家自身以及对面战场随从 是否符合法术发动sTime
	spellCheckPlayer(sTime);
	spellCheckEnemy(sTime);
}

void CBattle::spellCheck(int srcPool, int srcNum, int sTime){

	auto &card = _cardPool[srcPool][srcNum];
	int jS = card.get_spellID().size();
	for (int j = 0; j < jS; j++)
		if (card.get_spellID()[j] % 100 == sTime){
		spelling(card.get_spellID()[j], srcPool, srcNum,_camp);
		}
}

void CBattle::spellCheck(int srcPool, int srcNum, CCard &card1){
	auto &card = _cardPool[srcPool][srcNum];
	int jS = card.get_spellID().size();
	for (int j = 0; j < jS; j++)
		if (card.get_spellID()[j] % 100 == 4){
			spelling(card.get_spellID()[j], srcPool, srcNum,_camp);
		}
}



void CBattle::spellCheckPlayer(int sTime){

	int iP = _cardPool[POOL_BATTLE].size();
	for (int i = 0; i < iP; i++){
		spellCheck(POOL_BATTLE, i, sTime);
	}
}

void CBattle::spellCheckEnemy(int sTime){


}



void CBattle::spelling(int spell_num)
{

}



//void CBattle::cardAttack(int srcNum,int src)

void CBattle::cardAttack(int srcNum, int srcCamp, int destNum, int destCamp)
{

	//srcCard即攻击方,destCard即被攻击方
	CCard *srcCard, *destCard;

	if (srcNum == 7){
		srcCard = &_hero->_hero;
	}
	else{
		if (srcCamp == _camp)
			srcCard = &_cardPool[POOL_BATTLE].at(srcNum);
		else
			srcCard = &_enemy->_cardPool[POOL_BATTLE].at(srcNum);
	}

	if (destCamp == _camp)
		destCard = &_cardPool[POOL_BATTLE].at(destNum);
	else
		destCard = &_enemy->_cardPool[POOL_BATTLE].at(destNum);
	if (destCamp==2)
		destCard = &_wild->_cardPool[POOL_BATTLE].at(destNum);
	if (srcNum!=7)
		spellCheck(POOL_BATTLE, srcNum, _enemy->_cardPool[POOL_BATTLE].at(destNum));
	reduceAttack(*srcCard);

	//随从实际和显示血量的改变
	//判断敌方随从是否带有“猎人印记”的buff
	Buff buff(1, 4);
	if (srcCard->buffCheck(buff))
		srcCard->damaged((destCard->getFinalAttack() - srcCard->get_armor()) <= 0 ? 0 : (destCard->getFinalAttack() - srcCard->get_armor()) * 2);
	else
		srcCard->damaged((destCard->getFinalAttack() - srcCard->get_armor()) <= 0 ? 0 : destCard->getFinalAttack() - srcCard->get_armor());
	if (srcNum == 7 && _hero->checkWBuff(2)){
		spelling(721007,0,0,0);
	}
	else{
		
		if (destCard->buffCheck(buff))
			destCard->damaged((srcCard->getFinalAttack() - destCard->get_armor()) <= 0 ? 0 : (srcCard->getFinalAttack() - destCard->get_armor()) * 2);
		else
			destCard->damaged((srcCard->getFinalAttack() - destCard->get_armor()) <= 0 ? 0 : srcCard->getFinalAttack() - destCard->get_armor());
	}
	//显示血量
	int srcHealth;
	int destHealth;
	if (srcNum != 7)
		srcHealth = _cardPool[POOL_BATTLE].at(srcNum).getFinalHealth();
	else
		srcHealth = _hero->getHealthData();
	if (destNum != 7)
		destHealth = _enemy->_cardPool[POOL_BATTLE].at(destNum).getFinalHealth();
	else
		destHealth = _enemy->_hero->getHealthData();
	_gameboard->setCardProperties(POOL_BATTLE, srcNum, srcCamp ,srcHealth, 2);
	_gameboard->setCardProperties(POOL_BATTLE, destNum,destCamp, destHealth, 2);

	//取消本回合随从的攻击能力
	srcCard->set_isAttack(true);
	if (srcNum==7&&_hero->checkWBuff(1)&&destCard->_healthBattle<destCard->get_healthMax()){
		destCard->_healthBattle = 0;
	}

	_gameboard->cardAttack(srcNum, srcCamp, srcCard->getFinalHealth(), destNum, destCamp, destCard->getFinalHealth()); 
	
	if (srcNum!=7)
	if (srcCard->isDead()){
		if (srcCamp == _camp)
			cardDead(srcNum);
		else
			_enemy->cardDead(srcNum);
	}
	if (!(srcNum == 7 && _hero->checkWBuff(2)))
	if (destCard->isDead()){
		if (destCamp == _camp)
			cardDead(destNum);
		else
			if (destCamp < 2)
				_enemy->cardDead(destNum);
			else
				_wild->cardDead(destNum);
	}
}


void CBattle::cardAttack(int num)						//随从攻击英雄 重载+1
{

	CCard& creAttack = num <5 ?  _cardPool[POOL_BATTLE][num]:_hero->_hero;

	reduceAttack(creAttack);

	//TODO 创建一个hero数据类
	_enemy->_hero->setHealthData(_enemy->_hero->getHealthData() - creAttack.getFinalAttack());

	_gameboard->cardAttack(num, _camp, creAttack.getFinalHealth(),7, !_camp, _enemy->_hero->getHealthData());
	if (_enemy->_hero->getHealth() <= 0)
	{
		_gameState = GAME_WIN;
		_enemy->_gameState = GAME_LOSE;
		gameOver();
	}
}





void CBattle::drawCard(){

	int num1 = _cardPool[POOL_DECK].size() - 1, num2 = _cardPool[POOL_HAND].size();

	//显示操作
	//_gameboard->getCard(_cardPool[POOL_DECK][num1].flagNum)->transFromDeckToHand(num2, num2,_camp);

	//数据操作
	//CardTranslate(_cardPool[POOL_DECK], _cardPool[POOL_HAND], num1, num2);
	cardTransfer(POOL_DECK, POOL_HAND, num1, num2);
	spellCheck(5);
	check();


}

void CBattle::cardCummon(vector<CCard>&card1, vector<CCard>&card2, int num1, int num2, int battlePlace){
	num2 = 0;
	//检测是否可召唤 召唤随从 卡牌传递 技能检测 
	spellCheck(12);
	_resourceNumber++;
	//暂不函数化的卡牌召唤光环添加
	card1[num1].relife();
	for (int j = 0; j < card2.size(); j++){
		for (int i = 0; i < card2[j]._cardbuff.size(); i++){
			buffAdd(card2[j]._cardbuff[i], card2[j]._resourceID, card1[num1]);
		}
	}
	card1[num1]._attacktime = 0;
	card1[num1].canAttack();

	actionChange(-card1[num1].get_cost());
	_gameboard->getActionPointBar(_camp)->reduceAvailActionPoint(card1[num1].get_cost());
	//CardTranslate(card1, card2, num1, num2);
	if (_camp == 1) card1[num1].set_pos(battlePlace);
	cardTransfer(POOL_HAND, POOL_BATTLE, num1, num2, battlePlace);

	spellCheck(POOL_BATTLE, num2, 2);
	//card2[num2].Pos = num2;
	check();

}
//召唤 重载
void CBattle::cardCummon(int srcPool, int destPool, int num1, int num2, int battlePlace)
{
	cardCummon(_cardPool[srcPool], _cardPool[destPool], num1, num2, battlePlace);
}

void CBattle::spellLaunch(vector<CCard>&card, int num)
{
	actionChange(-card[num].get_cost());
	_gameboard->getActionPointBar(_camp)->reduceAvailActionPoint(card[num].get_cost());
	cardTransfer(POOL_HAND, POOL_CEME, num, 0);
	spellCheck(POOL_CEME, 0, 7);
}

void CBattle::spellLaunch(int cardPool, int num)
{
	spellLaunch(_cardPool[cardPool],num);
}

//11.6
void CBattle::equipLaunch(int cardPool, int num){
	//_hero->_equip.push_back()
	actionChange(-_cardPool[cardPool][num].get_cost());
	_gameboard->getActionPointBar(_camp)->reduceAvailActionPoint(_cardPool[cardPool][num].get_cost());
	_hero->realAddWeapon(_cardPool[cardPool][num]);
	cardTransfer(POOL_HAND, POOL_CEME, num, 0);
}


void CBattle::cardTransfer(int srcPool, int destPool, int srcNum, int destNum, int battlePlace)
{
	if (_cardPool[destPool].size() == 0 || destNum>_cardPool[destPool].size())
	{
		_cardPool[destPool].push_back(_cardPool[srcPool][srcNum]);
	}
	else
	{
		_cardPool[destPool].insert(_cardPool[destPool].begin() + destNum, _cardPool[srcPool][srcNum]);
	}

	_gameboard->cardTransfer(srcPool, destPool, srcNum, destNum, _camp, _cardPool[destPool][destNum], battlePlace);

	_cardPool[srcPool].erase(_cardPool[srcPool].begin() + srcNum);
}

CBattle::~CBattle()
{
}


void CBattle::cardDead(int num){
	//CardTranslate(card, _cardPool[POOL_CEME], 0);
	CCard& card = _cardPool[POOL_BATTLE].at(num);
	if (card.get_place() < 0 || card.get_place()>5) card.set_place(0);
	if (_camp<2) ai->place[card.get_place()] = true;
	spellCheck(POOL_BATTLE, num, 03);
	//if (_camp<2) 
		cardTransfer(POOL_BATTLE, POOL_CEME, num, 0);
	//else _cardPool[POOL_BATTLE].erase(_cardPool[POOL_BATTLE].begin() + num);

	//spellCheck(13);
	//spellCheck(card, 03);
}
void CBattle::checkDead(){
	int k = _cardPool[POOL_BATTLE].size();
	vector<int>t;
	for (int i = 0; i < k; i++){
		if (_cardPool[POOL_BATTLE][i].isDead()){
			cardDead(i);
			i--; k--;
			//			t.push_back(i); 还须统一死亡结算
		}
	}

}

//void CBattle::CreatureDead(CCard &card){
void CBattle::update(float dt)
{
	int BattleID = _battleID;

	//当是己方,且为对战模式，且处于回合中,则发送命令
	if ((!_camp) && _gameMode&&_gameState == GAME_RUN)
	{
		if (BattleID != 0 && _battleState)
		{
			;
		//	char c[10];
		//	sprintf(c, "%d", BattleID);
		//	_gameSocket->sendMsg(c);
		//	_battleState = false;
		}

	}


	//当是敌方,且为对战模式，且处于其回合状态,则实时监听服务器端的命令
	if ((_camp) && _gameMode&&_gameState == GAME_RUN&&_gameSocket->_recv == false)
	{


		string msg = _gameSocket->getMsg();
		if (msg != "")
		{
			int msg1;
			sscanf(msg.c_str(), "%d", &msg1);
			if (msg1 < 0 || msg1 > 6000000)
				return;

			if (msg1 != 6000000)
			{
				_gameSocket->recvMsg();
				log("update-recv");
			}
			//turnOver();
			Command::getInstance()->sendCommand(msg1, _camp,false);
			//_battleState = true;
			//BattleID = msg1;
		
		}
	}

	


}
void CBattle::check()
{
	spellCheck(99);
}




//游戏开始 - 准备牌库 - 抽初始牌
void CBattle::gameStart()
{
	//准备牌库
	for (int i = 0; i < 30; i++)
	{
		CCard card = CCard(_cardIds[i]);
		_gameboard->addCard(card, POOL_DECK, i, _camp, 0.1f*i);
		_cardPool[POOL_DECK].push_back(card);
	}

	//通知棋盘游戏开始,播放相应动画
	_gameboard->gameBegin();

	//抽初始牌
	/*	this->runAction(Sequence::create(
	DelayTime::create(1.f),
	CallFunc::create(CC_CALLBACK_0(CBattle::drawCard,this)),
	NULL));*/
	if (_camp)
	{
		//_gameboard->getActionQueue()->advance(1.5f);
		this->drawCard();
		//_gameboard->getActionQueue()->advance(0.4f);
		_enemy->drawCard();
		//_gameboard->getActionQueue()->advance(1.5f);
		this->drawCard();
		//_gameboard->getActionQueue()->advance(0.4f);
		_enemy->drawCard();


		_gameboard->getActionQueue()->reset(false);
	}

	//ai的设定 这边使得玩家也操控是为了测试
	ai = new CAIEnemy(this);


	//转致gaming状态
	if (!_firstHand){
		_gameState = GAME_WAIT;
	}
	else
	{
		_gameState = GAME_RUN;
		turnStart();
	}
	gaming();
}

void CBattle::gaming()
{
	this->scheduleUpdate();
}

void CBattle::gameOver()
{
	//移除定时器
	this->unscheduleAllSelectors();

	if (!_camp) //为己方
	{
		if (_gameState == GAME_WIN)
			_gameboard->gameWin();
		else if (_gameState == GAME_LOSE)
			_gameboard->gameLose();
	}
}




void CBattle::spelling(int spell_num,int srcPool,int srcNum,int srcCamp){

	CCard &spellCard = srcCamp == _camp ? _cardPool[srcPool][srcNum] : _enemy->_cardPool[srcPool][srcNum];



	int numX = spell_num / 1000000, numID = spell_num % 1000000 / 1000, numChoose = spell_num / 100 % 10;
	switch (numID)
	{
	case 001:{
		spellCard.set_armor(spellCard.get_armor() + numX);
		_gameboard->setCardProperties(srcPool, srcNum, _camp, spellCard.get_armor(),3);
		break; 
	}
	case 002:
		spellCard.get_buffID().push_back(0000);
		break;
	case 003:
		spellCard.damaged(numX);
		break;
	case 004:
		if (numX == 0){
			numX = 9999;
		}
		spellCard.heal(numX);
		break;
	case 005:
		if (numX == 0)numX++;
		for (int i = 0; i < numX; i++){
			drawCard();
		}
		break;
	case 200:
	{
		//待完善
		spellCard.set_attackBuff(0);
		int s = /*CinBattle*/_cardPool[POOL_BATTLE].size();
		for (int i = 0; i < s; i++){
			spellCard.set_attackBuff(spellCard.get_attackBuff() + _cardPool[POOL_BATTLE][i].get_armor());
		}
		break;
	}
	case 400:{
		CCard ncard;
		//无中生有
		ncard.cardCreate(6);
		CBattle *run;
		if (_wild->_gameState = GAME_RUN) run = _wild; else run = _enemy;
		//run->_cardPool[POOL_DECK].insert(_cardPool[POOL_DECK].begin(), ncard);
		run->drawCard();
		break;
	}
	case 500:
		// 获得牌ID
		//Spelling(1001100, srcPool, srcNum);
		//Spelling(4100, srcPool, srcNum);
		break;
	case 501:{
		drawCard();
		//hero 类互动
		break; }
	case 502:
		//to be 
		break;
	case 503:
	{
		int s = _cardPool[POOL_BATTLE].size();
		for (int i = 0; i < s; i++){
			if (_cardPool[POOL_BATTLE][i].get_armor() > 0) 
				drawCard();
		}
		break;
	}
	//10.30 战士随从
	case 600:{
		spelling( 2001000,srcPool, srcNum,srcCamp);
		break;
	}
	case 601:{
		auto i = _battleID % 10+1;
		if (_camp == 1&&_gameMode==0) i=ai->chooseCardofMaxAtk(false);
		if (i < 0) break;
		spelling( 2001000, srcPool, i,srcCamp);
		break;
	}
	case 602:{
		auto i = _battleID % 10;
		if (_camp == 1 && _gameMode == 0) i = ai->chooseCardofMaxAtk(true);
		if (i < 0) break;
		//Buff buff(1, 2);
		Buff buff(1,2);
		buff._times = 3;
		_enemy->_cardPool[POOL_BATTLE].at(i).addBuff(buff);
		break;
		/*
		int a=3;
		int &b=a;
		*/
	}
	case 603:{
		spelling(2001000, srcPool, srcNum, srcCamp);
		
		break;
	}
	case 604:{
		for (int i = 0; i < spellCard._cardbuff.size();i++){
			if (spellCard._cardbuff[i]._bufftype == 0 && spellCard._cardbuff[i]._buffid == 5){
				spellCard.deleteBuff(i);
				//_gameboard->setCardProperties(srcPool,srcNum,srcCamp,)
				break;
			}
		}
		int addarmor = 0;// _hero->getArmor();//*hero
		for (int i = 0; i < _cardPool[POOL_BATTLE].size(); i++){
			addarmor += _cardPool[POOL_BATTLE][i].get_armor();
		}
		Buff buff(0, 5);
		buff.setdata(0, addarmor, 0);
		spellCard.addBuff(buff);
		break;
	}
	case 620:{
		if (_camp == 1&&_gameMode==0) break;
		auto i = _battleID % 10;
		_cardPool[POOL_BATTLE][i].heal(100);
		spelling(1001000, POOL_BATTLE, i,srcCamp);
		break;
	}
	case 621:{
		Buff buff(1, 5);
		buff._times = 1;
		for (int i = 0; i < _cardPool[POOL_BATTLE].size(); i++)
		{
			_cardPool[POOL_BATTLE][i].addBuff(buff);
		}
		break;
	}
	case 622:{
		if (_hero->getArmor()>0) drawCard();
		for (int i = 0; i < _cardPool[POOL_BATTLE].size(); i++)
		{
			if (_cardPool[POOL_BATTLE][i].get_armor())
				drawCard();
		}
		break;
	}
		//10.6 猎人部分随从异能
	case 700:{
		Buff buff(2, 1);
		buff.setdata(0, 1, 0);
		spellCard.addBuff(buff);
		for (int i = 0; i < _cardPool[POOL_BATTLE].size(); i++){
			if (_cardPool[POOL_BATTLE].at(i)._resourceID != spellCard._resourceID){
				buffAdd(buff, spellCard._resourceID, _cardPool[POOL_BATTLE].at(i));
			}
		}
		break;
	}
	case 701:{
		break;
	}
	case 702:{
		Buff buff(1, 2);
		buff._times = 3;
		spellCard.addBuff(buff);
		break;
	}
	case 703:{
		int k = _enemy->_cardPool[POOL_BATTLE].size();
		int ran = rand() % (k + 1);
		if (ran == k)
			_enemy->_hero->setHealth(_enemy->_hero->getHealth() - 3);
		else{
			_enemy->_cardPool[POOL_BATTLE][ran].damaged(10);
			if (_enemy->_cardPool[POOL_BATTLE][ran].isDead()){
				//CCard *card = &(_enemy->_cardPool[POOL_BATTLE][ran]);
				_enemy->cardDead(ran);
			}
		}
		break;
	}
	case 705:{
		Buff buff(1, 2);
		buff._times = 3;
		for (int i = 0; i < _enemy->_cardPool[POOL_BATTLE].size(); i++){
			_enemy->_cardPool[POOL_BATTLE][i].addBuff(buff);
		}
		for (int i = 1; i < _cardPool[POOL_BATTLE].size(); i++){
			_cardPool[POOL_BATTLE][i].addBuff(buff);
		}

		break;
	}
	case 706:{
		Buff buff(0, 1);
		buff.setdata(0, 1, 0);
		int p=spellCard.get_pos();
		for (int i = 1; i < _cardPool[POOL_BATTLE].size(); i++){
			if (_cardPool[POOL_BATTLE][i].get_pos() == p + 1 || _cardPool[POOL_BATTLE][i].get_pos()==p-1){
				_cardPool[POOL_BATTLE][i].addBuff(buff);
			}
		}break;
	}
	case 708:{
		Buff buff(1, 3);
		spellCard.addBuff(buff);
		break;
	}
	case 709:{
		//Buff buff(1,)
		spellCard._attacktime = 1;
		spellCard.canAttack();
		//spellCard.set_armor(1);
		break;
	}
	case 710:{
		auto i = _battleID % 10;
		if (_camp == 1 && _gameMode == 0) i = ai->chooseCardofMaxAtk(true);
		if (i < 0) break;
		Buff buff(1, 2);
		buff._times = 3;
		_cardPool[POOL_BATTLE][i].addBuff(buff);
		_cardPool[POOL_BATTLE][i]._attacktime = 0;
		_cardPool[POOL_BATTLE][i].canAttack();
		break;
	}
	case 711:{
		auto i = _battleID % 10;
		if (_camp == 1 && _gameMode == 0) i = ai->chooseCardofMaxAtk(true);
		if (i < 0) break;
		Buff buff(1, 2);
		buff._times = 7;
		_enemy->_cardPool[POOL_BATTLE][i].addBuff(buff);
		break;
	}
	case 712:{
		auto i = _battleID % 10;
		if (_camp == 1 && _gameMode == 0) i = ai->chooseCardofMaxAtk(true);
		if (i < 0) break;
		Buff buff(0, 1);
		buff._times = 1;
		buff.setdata(0, -3, 0);
		_enemy->_cardPool[POOL_BATTLE][i].addBuff(buff);
		break;
	}
	case 720:{

		if (_camp == 1&&_gameMode==0) break;
		auto i = _battleID % 10;
		_enemy->cardDead(i);
		break;
	}
	case 721:{
		for (int i = 0; i < _enemy->_cardPool[POOL_BATTLE].size(); i++){
			_enemy->_cardPool[POOL_BATTLE][i].damaged(10);
		}
		_enemy->checkDead();
		break;
	}
	case 722:{
		if (_camp == 1&&_gameMode==0) break;
		auto i = _battleID % 10;
		_enemy->_cardPool[POOL_BATTLE][i].damaged(6);
		break;
	}
	case 723:{
		for (int i = 0; i < _cardPool[POOL_BATTLE].size(); i++){
			_cardPool[POOL_BATTLE][i].heal(3);
		}
		break;
	}

		//测试用 800 801 802
	case 800:{
		int s = /*CinBattle*/_cardPool[POOL_BATTLE].size();
		for (int i = 0; i < s; i++){
			_cardPool[POOL_BATTLE][i].set_costBattle(_cardPool[POOL_BATTLE][i].get_costBattle() - 1);
		}
		break;
	}
		/*增加属性*/
	case 801:{
		int s = _cardPool[POOL_BATTLE].size();
		for (int i = 0; i < s; i++){
			_cardPool[POOL_BATTLE][i].getBuff(1, 1);

			_gameboard->setCardProperties(POOL_BATTLE, i, _camp, _cardPool[POOL_BATTLE][i].get_attackBattle(), 1);
			_gameboard->setCardProperties(POOL_BATTLE, i, _camp, _cardPool[POOL_BATTLE][i].get_healthBattle(), 2);
		}
		break;
	}
	case 802:{
		_enemy->_hero->setHealthData(_enemy->_hero->getHealthData() - 3);/*数据处理*/

		_gameboard->addEffect(EFFECT_FIREBALL, 0.5f, srcPool, srcNum, _camp, _enemy->_hero);/*特效处理*/
		/*显示处理*/
		auto e = _gameboard->getEffect();
		auto aq = _gameboard->getActionQueue();
		auto keytime = e->getKeyTimeOfEffect(EFFECT_FIREBALL);

		aq->advanceToLastAction();
		aq->delay(keytime*0.5);
		_gameboard->setHeroHealth(!_camp, _enemy->_hero->getHealthData());
		aq->delay((1 - keytime)*0.5);
		//_enemy->_hero->setHealth(_enemy->_hero->getHealth() - 3, 3.f);//todo
		break;
	}
	case 707:{
		//3 damage
		auto i = _battleID % 10;
		if (_camp == 1 && _gameMode == 0) i = ai->chooseCardofMaxAtk(true);
		if (i < 0) break;
		_enemy->_cardPool[POOL_BATTLE][i].damaged(2);
		_gameboard->setCardProperties(POOL_BATTLE, i, !_camp, _enemy->_cardPool[POOL_BATTLE][i].getFinalHealth(), 2);
		if (_enemy->_cardPool[POOL_BATTLE][i].isDead()){
			//CCard *card = &(_enemy->_cardPool[POOL_BATTLE][ran]);
			_enemy->cardDead(i);
		}
		break;
	}
	case 900:
		//获取指令 攻击反制 待完善
		break;
	case 901:
		//随从豁免
		break;
	case 904:
		if (actionPoints < numX) break;
		actionChange(-numX);
		//CardTranslate(spellCard, /*CinBattle*/_cardPool[POOL_BATTLE], spellCard.Pos);//可能有问题 回头再看看
		cardTransfer(spellCard.get_state(), /*CinBattle*/POOL_BATTLE, spellCard.get_pos(), spellCard.get_pos());//可能有问题 回头再看看
		break;
	default:
		break;
	}

}


int CBattle::getCardPos(int cardPool, CCard &card)//获取card在数组中的位置,没有则返回-1
{
	auto pool = &_cardPool[cardPool];
	int i = 0;
	int size = pool->size();
	for (i = 0; i < size; i++)
	{
		if (&pool->at(i) == &card)
			return i;
	}
	return -1;
}


void CBattle::buffAdd(Buff& buff, int resource, CCard&card){
	if (buff._bufftype == 2){
		if (buff._buffid == 1){
			Buff buff1(0, 0);
			buff1.setdata(0, 1, 0);
			buff1.setResource(resource);
			card.addBuff(buff1);
		}
	}
}

void CBattle::reduceAttack(CCard &card){
	card._attacktime = card._attacktime - 1;

	card.canAttack();
}

void CBattle::skillSpelling(int spell_num, int destPool, int destNum)
{

	auto skillID = _gameboard->getRole(_camp)->getRoleSkill(spell_num)->getSkillID();
	//猎人的技能
	if (skillID == 10)
	{
		switch (destPool)
		{
		case 3:	//己方随从
			//实际扣血
			_cardPool[POOL_BATTLE].at(destNum).damaged(1);
			/*特效处理*/
			_gameboard->addEffect(EFFECT_FIRE_FLASH, 0.2f, _hero,POOL_BATTLE,destNum,_camp);
			//显示扣血
			_gameboard->setCardProperties(POOL_BATTLE, destNum, _camp,  _cardPool[POOL_BATTLE].at(destNum).getFinalHealth(), 2);

			if (_cardPool[POOL_BATTLE].at(destNum).isDead())
				cardDead(destNum);
			
			break;
		case 4:			//敌方随从
			//实际扣血
			_enemy->_cardPool[POOL_BATTLE].at(destNum).damaged(1);
			/*特效处理*/
			_gameboard->addEffect(EFFECT_FIRE_FLASH, 0.4f, _hero, POOL_BATTLE, destNum, !_camp);
			//实际扣血
			_gameboard->setCardProperties(POOL_BATTLE, destNum, !_camp, _enemy->_cardPool[POOL_BATTLE].at(destNum).getFinalHealth(), 2);
			if (ActPtsMax >= 0)
			{
				Buff _buff(1, 4);
				_buff._times = 1;
				_enemy->_cardPool[POOL_BATTLE].at(destNum).addBuff(_buff);
			}

			if (_enemy->_cardPool[POOL_BATTLE].at(destNum).isDead())
				_enemy->cardDead(destNum);
			
			break;
		case 6:		//己方英雄
			_hero->setHealthData(_hero->getHealthData() - 1);
			_gameboard->addEffect(EFFECT_FIRE_FLASH, 0.2f, _hero,_hero);
			_hero->setHealth(_hero->getHealthData());
			break;
		case 7:		//敌方英雄
			_enemy->_hero->setHealthData(_enemy->_hero->getHealthData() - 1);
			_gameboard->addEffect(EFFECT_FIRE_FLASH, 0.5f, _hero, _enemy->_hero);
			_enemy->_hero->setHealth(_enemy->_hero->getHealthData());
			break;
		default:
			break;
		}


	}

	if (skillID == 12)
	{
		for (int i = 1; i <= 15; i++)
		{
			int num = rand() % (_enemy->_cardPool[POOL_BATTLE].size() + 1);
			if (num == _enemy->_cardPool[POOL_BATTLE].size())

			{
				/*数据处理*/
				_enemy->_hero->setHealthData(_enemy->_hero->getHealthData() - 1);

				/*特效处理*/
				_gameboard->addEffect(EFFECT_FIREBALL, 0.5f, _hero, _enemy->_hero);

				/*显示处理*/
				_gameboard->setHeroHealth(!_camp, _enemy->_hero->getHealthData());


			}
			else
			{
				_enemy->_cardPool[POOL_BATTLE].at(num).damaged(1);


				/*特效处理*/
				_gameboard->addEffect(EFFECT_FIREBALL, 0.5f, _hero, POOL_BATTLE, num, !_camp);

				/*显示处理*/
				auto e = _gameboard->getEffect();
				auto aq = _gameboard->getActionQueue();
				auto keytime = e->getKeyTimeOfEffect(EFFECT_FIREBALL);

				aq->advanceToLastAction();
				aq->delay(keytime*0.5);
				//_gameboard->setHeroHealth(!_camp, _enemy->_hero->getHealthData());
				_gameboard->setCardProperties(POOL_BATTLE, num, !_camp, _enemy->_cardPool[POOL_BATTLE].at(num).getFinalHealth(), 2);
				aq->delay((1 - keytime)*0.5);

				if (_enemy->_cardPool[POOL_BATTLE].at(num).isDead())
					_enemy->cardDead(num);
			}
		}
	}
	//战士的技能
	if (skillID == 0)
	{
		if (destPool != 3)
			return;
		_cardPool[POOL_BATTLE].at(destNum).set_armor(_cardPool[POOL_BATTLE].at(destNum).get_armor() + 1);
		Buff _buff(1, 4);
		_buff._times = 1;
		_cardPool[POOL_BATTLE].at(destNum).addBuff(_buff);
	}

	if (skillID == 2)
	{
		_hero->setHealthData(_hero->getHealthData() + 10);
		_hero->setArmor(_hero->getArmor() + 2);
		_hero->setAttackSkill(3);

	}




	//判定使用技能后敌方英雄是否死亡


	if (_enemy->_hero->getHealth() <= 0)
	{
		_gameState = GAME_WIN;
		_enemy->_gameState = GAME_LOSE;
		gameOver();
	}


}

void CBattle::buffCheck(int sTime, CCard& card){
	for (int i = 0; i < card._cardbuff.size(); i++){
		auto buff = card._cardbuff[i];
		if (sTime == 0){
			if (buff._bufftype == 1){
				if (buff._buffid == 2){
					card._attacktime = 0;
				}
				if (buff._buffid == 3){
					card._attacktime *= 2;
				}
			}
		}
	}
}


void CBattle::skillSpelling()
{
	if (ActPtsMax != 6)
		return;
	int _heroID = _hero->getID();
	switch (_heroID)
	{
	case 0:
		_hero->setArmor(_hero->getArmor() + 1);
	default:
		break;
	}

}


void  CBattle::setBattleID(int battleID)
{
	_battleID = battleID;
	_battleState = true;
}

void CBattle::addWild(){
	bool f = true; int i = 0;
	for (i = 0; i < 4; i++){
		f = true;
		for (int j = 0; j < _cardPool[POOL_BATTLE].size();j++){
			if (_cardPool[POOL_BATTLE][j].get_pos() == i){
				f = false;
				break;
			}
		}
		if (f) break;
	}
	if (f){
		CCard card;
		card.cardCreate(10001);
		card.set_pos(i);
		//无中生有
		_cardPool[POOL_BATTLE].push_back(card);
		_gameboard->addCard(card, POOL_MONSTER, 0, 0, 0);
	}
}
/*
CCard& CBattle::chooseCard(int bid){
	CBattle
	return 
}*/
//10-w5-p3-e4