#include "Battle.h"
#include "AIEnemy.h"

CBattle::CBattle(GameBoard * gameboard, int *battleID, bool * battleState, int *cardId, int camp,GameSocket *gameSocket,bool gameMode,bool firstHand) 
{

	// �������ߵ�ָ��
	_battleID = battleID;
	_battleState = battleState;
	_gameState = GAME_BEGIN;
	_gameboard = gameboard;
	_camp = camp;
	_hero = _gameboard->getRole(camp);
	_cardIds = cardId;
	_gameSocket = gameSocket;
	_gameMode = gameMode;
	_firstHand = firstHand;
	ActionPoints = ActPtsMax = 1;
	//vector<CCard>*/*CinBattle*/_cardPool[POOL_BATTLE], *_cardPool[POOL_HAND], *_cardPool[POOL_DECK], *_cardPool[POOL_CEME];
	
};
void CBattle::setEnemy(CBattle *e)
{
	_enemy = e;
}

void CBattle::TurnStart(){
	//�غϿ�ʼ �ж�����+1 �ж����� ������޵��� ���� ������
	ActionPoints = ActPtsMax++;
	_gameState = GAME_RUN;
	//
	if (!_camp)
		_gameboard->startPlayerTurn();
	else
		_gameboard->startAiTurn();
	_gameboard->getActionPointBar(_camp)->addTotalActionPoint();
	_gameboard->getActionPointBar(_camp)->fullAvailActionPoint();
	//?����ж�
	DrawCard();
	_gameboard->getActionQueue()->reset(false);
	//ai->AiTurn();
	//���ܷ������
	SpellCheck(00);
	Check();


	//���ϻغϵ���ӱ�ɿ��Թ���
	for (int i = 0; i < _cardPool[POOL_BATTLE].size(); i++)
	{
		_cardPool[POOL_BATTLE].at(i).set_canAttack ( true);
		_cardPool[POOL_BATTLE].at(i).set_isAttack(false);
	}

	//����ǵз�,ת��Ai��
	if (_camp)
	{
		//ai->AiTurn()
		Turning();	
		/*this->runAction(Sequence::create(
		DelayTime::create(2.f),
			CallFunc::create(CC_CALLBACK_0(CBattle::Turning, this)),
			NULL));*/
	}
}

void CBattle::Turning()
{
	if (!_gameMode)//����ǵ���ģʽ
	{
		auto t = ai->AiTurn();
		_gameboard->getActionQueue()->reset(false);
		this->runAction(Sequence::create(
			DelayTime::create(t),
			CallFunc::create(CC_CALLBACK_0(CBattle::TurnOver, this)),
			NULL));
	}
	else
	{
		_gameSocket->recvMsg();
	}

}

void CBattle::TurnOver()
{
	SpellCheck(01);
	if (!_camp)
	{
		_gameboard->startAiTurn();
	}
	else
	{
		_gameboard->startPlayerTurn();
		if (_gameMode)
			_gameSocket->_recv=false;
	}
	//���ͻغϿ�ʼ������
	_gameState = GAME_WAIT;
	*_enemy->_battleState = true;
	*_enemy->_battleID = 5000000;
}
void CBattle::ActionChange(int num){
	ActionPoints += num;
}


vector<CCard>& CBattle::CheckPosition(CCard &card){
	return _cardPool[card.get_state()];
}
void CBattle::SpellCheck(int sTime){
	//�����������Լ�����ս����� �Ƿ���Ϸ�������sTime
	SpellCheckPlayer(sTime);
	SpellCheckEnemy(sTime);
}

void CBattle::SpellCheck(int srcPool,int srcNum,int sTime){
	
	auto &card = _cardPool[srcPool][srcNum];
	int jS = card.get_spellID().size();
	for (int j = 0; j < jS; j++)
		if (card.get_spellID()[j] % 100 == sTime){
			Spelling(card.get_spellID()[j], srcPool,srcNum);
		}
}



void CBattle::SpellCheckPlayer(int sTime){

	int iP = _cardPool[POOL_BATTLE].size();
	for (int i = 0; i < iP; i++){
		SpellCheck(POOL_BATTLE,i, sTime);
	}
}
void CBattle::SpellCheckEnemy(int sTime){
	//int iE =_enemy->_cardPool[POOL_BATTLE].size();
	//for (int i = 0; i < iE; i++){
	//	SpellCheck(_enemy->_cardPool[POOL_BATTLE][i], sTime);
	//}

}



void CBattle::Spelling(int spell_num)
{

}






void CBattle::cardAttack(int srcNum, int srcCamp, int destNum, int destCamp)
{

	//srcCard��������,destCard����������
	CCard *srcCard, *destCard;
	if (srcCamp == _camp)
		srcCard = &_cardPool[POOL_BATTLE].at(srcNum);
	else
		srcCard = &_enemy->_cardPool[POOL_BATTLE].at(srcNum);

	if (destCamp == _camp)
		destCard = &_cardPool[POOL_BATTLE].at(destNum);
	else
		destCard = &_enemy->_cardPool[POOL_BATTLE].at(destNum);

	reduceAttack(_cardPool[POOL_BATTLE].at(srcNum));
	srcCard->damaged(destCard->getFinalAttack());
	destCard->damaged(srcCard->getFinalAttack());

	//ȡ�����غ���ӵĹ�������
	srcCard->set_isAttack(true);


	_gameboard->cardAttack(srcNum, srcCamp, srcCard->getFinalHealth(), destNum, destCamp, destCard->getFinalHealth());

	if (srcCard->isDead()){
		if (srcCamp == _camp)
			CardDead(srcNum);
		else
			_enemy->CardDead(srcNum);
	}

	if (destCard->isDead()){
		if (destCamp == _camp)
			CardDead(destNum);
		else
			_enemy->CardDead(destNum);
	}
}


void CBattle::CardAttack(int num)						//��ӹ���Ӣ�� ����+1
{

	auto creAttack = _cardPool[POOL_BATTLE][num];
	reduceAttack(creAttack);
	//TODO ����һ��hero������
	_enemy->_hero->setHealthData(_enemy->_hero->getHealthData() - creAttack.getFinalAttack());
	_gameboard->cardAttack(num, _camp, creAttack.getFinalHealth(), -1, !_camp, _enemy->_hero->getHealthData());
	if (_enemy->_hero->getHealth() <= 0)
	{
		_gameState = GAME_WIN;
		_enemy->_gameState = GAME_LOSE;
		GameOver();
	}
}



void CBattle::DrawCard(){
	
	int num1 = _cardPool[POOL_DECK].size()-1, num2 = _cardPool[POOL_HAND].size();

	//��ʾ����
	//_gameboard->getCard(_cardPool[POOL_DECK][num1].flagNum)->transFromDeckToHand(num2, num2,_camp);

	//���ݲ���
	//CardTranslate(_cardPool[POOL_DECK], _cardPool[POOL_HAND], num1, num2);
	cardTransfer(POOL_DECK, POOL_HAND, num1, num2);
	SpellCheck(5);
	Check();


}

void CBattle::CardCummon(vector<CCard>&card1, vector<CCard>&card2, int num1, int num2,int battlePlace){
	//����Ƿ���ٻ� �ٻ���� ���ƴ��� ���ܼ�� 
	SpellCheck(12);
	_resourceNumber++;
	//�ݲ��������Ŀ����ٻ��⻷���

	for (int j = 0; j < card2.size(); j++){
		for (int i = 0; i < card2[j]._cardbuff.size(); i++){
			buffAdd(card2[j]._cardbuff[i], card2[j]._resourceID, card1[num1]);
		}
	}
	ActionChange(-card1[num1].get_cost());
	_gameboard->getActionPointBar(_camp)->reduceAvailActionPoint(card1[num1].get_cost());
	//CardTranslate(card1, card2, num1, num2);
	cardTransfer(POOL_HAND, POOL_BATTLE, num1, num2,battlePlace);
	SpellCheck(POOL_BATTLE,num2,2);
	//card2[num2].Pos = num2;
	Check();

}



//void CBattle::CardTranslate(vector<CCard>&card1, vector<CCard>&card2, int num1, int num2){//���ݿ���
//	//���������� ���Ƶ������Ƿ�����
//	if (card2.size() == 0 || num2>card2.size()){
//		card2.push_back(card1[num1]);
//	}
//	else{
//		card2.insert(card2.begin() + num2, card1[num1]);
//	}
//
//	_gameboard->cardTransfer(POOL_DECK, POOL_HAND, num1, num2, _camp, card2[num2]);
//
//	card1.erase(card1.begin() + num1);
//}

void CBattle::cardTransfer(int srcPool, int destPool, int srcNum, int destNum,int battlePlace)
{
	if (_cardPool[destPool].size() == 0 || destNum>_cardPool[destPool].size())
	{
		_cardPool[destPool].push_back(_cardPool[srcPool][srcNum]);
	}
	else
	{
		_cardPool[destPool].insert(_cardPool[destPool].begin() + destNum, _cardPool[srcPool][srcNum]);
	}

	_gameboard->cardTransfer(srcPool, destPool, srcNum, destNum, _camp, _cardPool[destPool][destNum],battlePlace);
	
	_cardPool[srcPool].erase(_cardPool[srcPool].begin() + srcNum);
}

CBattle::~CBattle()
{
}


void CBattle::CardDead(int num){
	//CardTranslate(card, _cardPool[POOL_CEME], 0);
	auto card = _cardPool[POOL_BATTLE].at(num);
	if (card.get_place() < 0 || card.get_place()>4) card.set_place(0);
	ai->place[card.get_place()] = true;
	SpellCheck(POOL_BATTLE, num, 03);
	cardTransfer(POOL_BATTLE, POOL_CEME, num, 0);

	//SpellCheck(13);
	//SpellCheck(card, 03);
}
void CBattle::CheckDead(){
	int k =_cardPool[POOL_BATTLE].size();
	vector<int>t;
	for (int i = 0; i < k; i++){
		if (_cardPool[POOL_BATTLE][i].isDead()){
			CardDead(i);
//			t.push_back(i); ����ͳһ��������
		}
	}
	
}

//void CBattle::CreatureDead(CCard &card){
void CBattle::update(float dt)
{
	int BattleID = *_battleID;

	//���Ǽ���,��Ϊ��սģʽ���Ҵ��ڻغ���,��������
	if ((!_camp) && _gameMode&&_gameState == GAME_RUN)
	{
		if (BattleID != 0&&*_battleState)
		{
			char c[10];
			sprintf(c, "%d", BattleID);
			_gameSocket->sendMsg(c);
		}

	}


	//���ǵз�,��Ϊ��սģʽ���Ҵ�����غ�״̬,��ʵʱ�����������˵�����
	if ((_camp) && _gameMode&&_gameState == GAME_RUN&&_gameSocket->_recv==false)
	{

		
		string msg = _gameSocket->getMsg();
		if (msg != "")
		{
			int msg1;
			sscanf(msg.c_str(), "%d", &msg1);
			if (msg1 != 6000000)
				_gameSocket->recvMsg();
			*_battleState = true;
			BattleID = msg1;
			//_gameSocket->recvMsg();
		}
	}

	

	while (*_battleState)
	{
		switch (BattleID / 1000000)
		{
		case 1:					//��ʾ����ٻ�
		{
			auto place = BattleID / 100 % 10;
			CardCummon(_cardPool[POOL_HAND],_cardPool[POOL_BATTLE], BattleID / 1000 % 10, BattleID % 10,BattleID/100%10);
			_gameboard->getActionQueue()->reset(false);
			break;
		}
		case 02:					//���߷���װ����ʹ��
		{
			auto CardID = _cardPool[POOL_HAND].at(BattleID / 1000 % 10).get_cardID();
			if (CardID / 1000 == 2)
				Spelling(CardID, POOL_HAND,BattleID / 1000 % 10);
			break;
		}
		case 03:					//	���ܵ�ʹ��
		{
			skillSpelling(BattleID / 1000 % 10, BattleID % 1000 / 10, BattleID % 10);
			_gameboard->getActionQueue()->reset(false);
			break;
		}
		case 4:					//����
		{
			if (BattleID / 10 % 10 != 7)
				//CardAttack(/*CinBattle*/_cardPool[POOL_BATTLE].at(BattleID / 1000 % 10), /*CinBattle*/_cardPool[POOL_BATTLE].at(BattleID / 1000 % 10));
			{
				cardAttack(BattleID / 1000 % 10, 0, BattleID % 10, 1);
				_gameboard->getActionQueue()->reset(false);
			}
			else
				//CardAttack(/*CinBattle*/_cardPool[POOL_BATTLE].at(BattleID / 1000 % 10));
			{
				CardAttack(BattleID / 1000 % 10);
				_gameboard->getActionQueue()->reset(false);
			}
			break;
			
		}
		case 5:  //�غϿ�ʼ
		{
			TurnStart();
			break;
		}
		case 6: //�غϽ���
		{
			TurnOver();
			break;
		}
		default:
			break;
		}
		*_battleState = false;
	}


}
void CBattle::Check()
{}




//��Ϸ��ʼ - ׼���ƿ� - ���ʼ��
void CBattle::GameStart()
{
	//׼���ƿ�
	for (int i = 0; i < 30; i++)
	{
		CCard card = CCard(_cardIds[i]);
		_gameboard->addCard(card,POOL_DECK,i,_camp,0.1f*i);
		_cardPool[POOL_DECK].push_back(card);
	}
	
	//֪ͨ������Ϸ��ʼ,������Ӧ����
	_gameboard->gameBegin();

	//���ʼ��
	/*	this->runAction(Sequence::create(
		DelayTime::create(1.f),
		CallFunc::create(CC_CALLBACK_0(CBattle::DrawCard,this)),
		NULL));*/
	if (_camp)
	{
		_gameboard->getActionQueue()->advance(1.5f);
		this->DrawCard();
		_gameboard->getActionQueue()->advance(0.4f);
		_enemy->DrawCard();
		_gameboard->getActionQueue()->advance(1.5f);
		this->DrawCard();
		_gameboard->getActionQueue()->advance(0.4f);
		_enemy->DrawCard();

	
		_gameboard->getActionQueue()->reset(false);
	}

	//ai���趨 ���ʹ�����Ҳ�ٿ���Ϊ�˲���
	ai= new CAIEnemy(this);
	
	
	//ת��gaming״̬
	if (!_firstHand){
		_gameState = GAME_WAIT;
	}
	else
	{
		_gameState = GAME_RUN;
		TurnStart();
	}
	Gaming();
}

void CBattle::Gaming()
{
	this->scheduleUpdate();
}

void CBattle::GameOver()
{
	//�Ƴ���ʱ��
	this->unscheduleAllSelectors();

	if (!_camp) //Ϊ����
	{
		if (_gameState == GAME_WIN)
			_gameboard->gameWin();
		else if (_gameState == GAME_LOSE)
			_gameboard->gameLose();
	}
}


bool CBattle::Spelling(int spell_num, int srcPool, int srcNum){
	int numX = spell_num / 1000000, numID = spell_num % 1000000 / 1000, numChoose = spell_num / 100 % 10;
	CCard &SpellCard =_cardPool[srcPool][srcNum];
	if (numChoose == 1) {
		CCard card2;
		//����µĿ���
		//&SpellCard = card2
	}
	switch (numID)
	{
	case 001:
		SpellCard.set_armor(SpellCard.get_armor() + numX);
		break;
	case 002:
		SpellCard.get_buffID().push_back(0000);
		break;
	case 003:
		SpellCard.damaged(numX);
		break;
	case 004:
		if (numX == 0){
			numX = 9999;
		}
		SpellCard.heal(numX);
		break;
	case 005:
		if (numX == 0)numX++;
		for (int i = 0; i < numX; i++){
			DrawCard();
		}
		break;
	case 200:
	{
		//������
		SpellCard.set_attackBuff(0);
		int s = /*CinBattle*/_cardPool[POOL_BATTLE].size();
		for (int i = 0; i < s; i++){
			SpellCard.set_attackBuff(SpellCard.get_attackBuff() + _cardPool[POOL_BATTLE][i].get_armor());
		}
	}
	case 500:
		// �����ID
		Spelling(1001100, srcPool,srcNum);
		Spelling(4100, srcPool,srcNum);
		break;
	case 501:
		DrawCard();
		//hero �໥��
		break;
	case 502:
		//to be 
		break;
	case 503:
	{
		int s = /*CinBattle*/_cardPool[POOL_BATTLE].size();
		for (int i = 0; i < s; i++){
			if (/*CinBattle*/_cardPool[POOL_BATTLE][i].get_armor() > 0) DrawCard();
		}
		break;
	}
		//10.6 ���˲����������
	case 700:{
		Buff buff(2, 1);
		buff.setdata(0, 1, 0);
		SpellCard.addBuff(buff);
		for (int i = 0; i < _cardPool[POOL_BATTLE].size(); i++){
			if (_cardPool[POOL_BATTLE].at(i)._resourceID != SpellCard._resourceID){
				buffAdd(buff, SpellCard._resourceID, _cardPool[POOL_BATTLE].at(i));
			}
		}
		break;
	}
	case 701:{
		break;
	}
	case 702:{
		break;
	}
	
	case 703:{
		int k = _enemy->_cardPool[POOL_BATTLE].size();
		int ran = rand() % (k + 1);
		if (ran == k)
			_enemy->_hero->setHealth(_enemy->_hero->getHealth() - 3);
		else{
			_enemy->_cardPool[POOL_BATTLE][ran].damaged(3);
			if (_enemy->_cardPool[POOL_BATTLE][ran].isDead()){
				//CCard *card = &(_enemy->_cardPool[POOL_BATTLE][ran]);
				_enemy->CardDead(ran);
			}
		}
		break;
	}

		//������ 800 801 802
	case 800:{
		int s = /*CinBattle*/_cardPool[POOL_BATTLE].size();
		for (int i = 0; i < s; i++){
			_cardPool[POOL_BATTLE][i].set_costBattle(_cardPool[POOL_BATTLE][i].get_costBattle()-1);
		}
	}
		/*��������*/
	case 801:{
		int s = _cardPool[POOL_BATTLE].size();
		for (int i = 0; i < s; i++){
			_cardPool[POOL_BATTLE][i].getBuff(1, 1);
		
			_gameboard->setCardProperties(POOL_BATTLE, i, _camp, _cardPool[POOL_BATTLE][i].get_attackBattle(), _cardPool[POOL_BATTLE][i].get_healthBattle());
		}
		break;
	}
	case 802:{
	
		_enemy->_hero->setHealthData(_enemy->_hero->getHealthData() - 3);/*���ݴ���*/
		_gameboard->addEffect(EFFECT_FIREBALL, 0.5f, srcPool, srcNum, _camp, _enemy->_hero);/*��Ч����*/

		/*��ʾ����*/
		auto e = _gameboard->getEffect();
		auto aq = _gameboard->getActionQueue();
		auto keytime =e->getKeyTimeOfEffect(EFFECT_FIREBALL);
		
		aq->advanceToLastAction();
		aq->delay(keytime*0.5);
		_gameboard->setHeroHealth(!_camp,_enemy->_hero->getHealthData());
		aq->delay((1-keytime)*0.5);
		//_enemy->_hero->setHealth(_enemy->_hero->getHealth() - 3, 3.f);//todo
		break;
	}
	case 900:
		//��ȡָ�� �������� ������
		break;
	case 901:
		//��ӻ���
		break;
	case 904:
		if (ActionPoints < numX) break;
		ActionChange(-numX);
		//CardTranslate(SpellCard, /*CinBattle*/_cardPool[POOL_BATTLE], SpellCard.Pos);//���������� ��ͷ�ٿ���
		cardTransfer(SpellCard.get_state(), /*CinBattle*/POOL_BATTLE, SpellCard.get_pos(), SpellCard.get_pos());//���������� ��ͷ�ٿ���
		break;
	default:
		break;
	}
	return true;

}


int CBattle::getCardPos(int cardPool, CCard &card)//��ȡcard�������е�λ��,û���򷵻�-1
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
	card._attacktime--;
	card.canAttack();
}

void CBattle::skillSpelling(int spell_num, int destPool, int destNum)
{
	auto spellID = _gameboard->getRole(_camp)->getRoleSkill(spell_num)->getID();

	spellID = 12;
	if (spellID == 10)
	{
		switch (destPool)
		{
		case 3:
			_cardPool[POOL_BATTLE].at(destNum).damaged(1);
			if (_cardPool[POOL_BATTLE].at(destNum).isDead())
				CardDead(destNum);
			break;
		case 4:
			_enemy->_cardPool[POOL_BATTLE].at(destNum).damaged(1);
			if (_enemy->_cardPool[POOL_BATTLE].at(destNum).isDead())
				_enemy->CardDead(destNum);
			break;
		case 6:
			_hero->setHealth(_enemy->_hero->getHealth() - 1);
			break;
		case 7:
			_enemy->_hero->setHealth(_enemy->_hero->getHealth() - 1);
			break;
		default:
			break;
		}


	}

	if (spellID == 12)
	{
		for (int i = 1; i <= 15; i++)
		{
			int num = rand() % (_enemy->_cardPool[POOL_BATTLE].size() + 1);
			if (num == _enemy->_cardPool[POOL_BATTLE].size())
			{
				/*���ݴ���*/
				_enemy->_hero->setHealthData(_enemy->_hero->getHealthData() - 1);

				/*��Ч����*/
				_gameboard->addEffect(EFFECT_FIREBALL, 0.5f,_hero, _enemy->_hero);

				/*��ʾ����*/
				auto e = _gameboard->getEffect();
				auto aq = _gameboard->getActionQueue();
				auto keytime = e->getKeyTimeOfEffect(EFFECT_FIREBALL);

				aq->advanceToLastAction();
				aq->delay(keytime*0.5);
				_gameboard->setHeroHealth(!_camp, _enemy->_hero->getHealthData());
				aq->delay((1 - keytime)*0.5);

			
			}
			else
			{
				_enemy->_cardPool[POOL_BATTLE].at(num).damaged(1);

				/*��Ч����*/
				_gameboard->addEffect(EFFECT_FIREBALL, 0.5f, _hero,POOL_BATTLE,num,!_camp);

				/*��ʾ����*/
				auto e = _gameboard->getEffect();
				auto aq = _gameboard->getActionQueue();
				auto keytime = e->getKeyTimeOfEffect(EFFECT_FIREBALL);

				aq->advanceToLastAction();
				aq->delay(keytime*0.5);
				//_gameboard->setHeroHealth(!_camp, _enemy->_hero->getHealthData());
				_gameboard->setCardProperties(POOL_BATTLE, num, !_camp, -1, _enemy->_cardPool[POOL_BATTLE].at(num).getFinalHealth(), -1);
				aq->delay((1 - keytime)*0.5);


				if (_enemy->_cardPool[POOL_BATTLE].at(num).isDead())
					_enemy->CardDead(num);
			}
		}
	}



	if (_enemy->_hero->getHealth() <= 0)
	{
		_gameState = GAME_WIN;
		_enemy->_gameState = GAME_LOSE;
		GameOver();
	}


}