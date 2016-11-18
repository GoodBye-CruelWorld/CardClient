#pragma once
#include<vector>
#include"Card.h"
#include"GameBoard.h"
#include"GameSocket.h"

using namespace std;

class CAIEnemy;

enum GameState{
	GAME_BEGIN,
	GAME_RUN,  //自己的回合
	GAME_WAIT, //对方的回合
	GAME_WIN,
	GAME_LOSE
};

class CBattle: public cocos2d::Node
{
public:
	CBattle(GameBoard *gameboard,int *cardId,int camp,GameSocket *gameSocket,bool gameMode,bool firstHand);
	~CBattle();

	CAIEnemy* ai;

	void setEnemy(CBattle*enemy);
	void setWild(CBattle*enemy);
	/*
	战斗流程
		游戏开始-准备牌库-抽初始牌
		游戏中
		游戏结束
	*/
	CBattle *_enemy,*_wild;//传递对面战斗类的指针

	void gameStart();
	void gameOver();
	void gaming();


	/*
	基础流程 ##
		回合开始（）-水晶增加-充满水晶-抽牌

		检测
		随从检测死亡-移除
		英雄检测死亡-移除 to be writen
		

		抽牌
		随从攻击
		召唤随从
		异能
		回合结束
	*/
	/*
	*/
	void turnStart();
	void turnOver();

	virtual void turning();

	void actionChange(int num);
	vector<CCard>& checkPosition(CCard &card);//检测牌的位置


	void drawCard();//抽牌
	//void drawCard(vector<CCardinHand> &cardIH, vector<CCardinDeck> &cardID);
	//void cardAttack(CCard &creAttack, CCard &creBeattacked);	//随从间的攻击
//	void cardAttack(CCard &creAttack);							//随从攻击英雄
	void cardAttack(int srcNum, int srcCamp, int destNum, int destCamp);
	void cardAttack(int num);							//随从攻击英雄 重载+1

	void roleAttack();											//英雄攻击英雄
	void roleAttack(CCard &creBeattacked);						//英雄攻击随从
	//void CreatureCommon(CCard card, vector<CCardinBattlefield> cardIB);//由引用构造

	void cardCummon(vector<CCard>&card1, vector<CCard>&card2, int num1, int num2,int battlePlace=0);//召唤 
	void cardCummon(int srcPool, int destPool, int num1, int num2, int battlePlace = 0);//召唤 重载
	//void CardTranslate(vector<CCard>&card1, vector<CCard>&card2, int num1,int num2);//传递卡牌 1为原来 2为传递之后 num为序列
	//void CardTranslate(CCard &card, vector<CCard>&card2, int num2);
	void cardTransfer(int srcPool,int destPool, int srcNum, int destNum,int battlePlace=0);


	void cardDead(int num);
	//void cardDead(CCard &card);
	//void cardDeadE(CCard &card);

	void check();//检测所有发生事件后续
	void checkDead();


	void spellCheck();
	//技能发动共分几个时段 回合开始00 回合结束01 召唤02 死亡03
	void spellCheck(int sTime);
	void spellCheck(int srcPool, int srcNum, int sTime);
	void spellCheckPlayer(int sTime);
	void spellCheckEnemy(int sTime);
	void spelling(int spell_num);//发动技能 技能ID号
//	bool Spelling(int spell_num, CCard &card1);
	void spelling(int spell_num, int srcPool, int srcNum,int srcCamp);
	void skillSpelling(int spellNum, int srcPool, int srcNum);
	void skillSpelling();//触发被动技能
	int getCardPos(int cardPool,CCard &card);//获取card在数组中的位置,没有则返回-1
	//void CreatureDead(CCardinBattlefield &creature);
	void creatureDead(CCard &card);
	bool heroIsDead();
	void setBattleID(int battleID);
	/*
	成员变量 vector类
	牌库的牌 手牌 场地 墓地
	*/
//	vector<CCard>CinBattle, CinHand, CinDeck, CinCeme;
	/*下标可以为POOL_HAND,POOL_DECK,POOL_CEME,POOL_BATTLE*/
	vector<CCard> _cardPool[4];

	BoardRole *_hero;
	int actionPoints,ActPtsMax;//行动点数及最大值

	int _battleID;
//	int *_battleID;				// 用于储存交互用的ID地址
	bool _battleState;			// 用户信息响应的标志
	GameState _gameState;  //判断游戏进行状态


	void update(float dt);

	GameBoard *_gameboard;
	int _camp;
	int *_cardIds; 
	GameSocket *_gameSocket;
	bool _gameMode;
	bool _firstHand;

	//10.6 添加
	void reduceAttack(CCard&);
	void buffAdd(Buff&, int, CCard&);
	int _resourceNumber;


	//10.27 
	void buffCheck(int,CCard&);
	//10.28
	//void spellAttackCheck(CCard&, CCard&){}
	void spellCheck(int,int, CCard &card);
	//void spelling(CCard&, int,int,int,int);
	//10.30
	void spellLaunch(vector<CCard>&, int);
	void spellLaunch(int cardPool, int);
	//void relife();
	//11.6
	void equipLaunch(int ,int);
	//11.10
//	void cardAttack(Card*,Card*)
	bool _posf[4] ;
	void addWild();

	
	//11.13
	int rand_wrapped();  //wrap rand() in order to send and recv msg;
	int _randRemote;

	CCard& chooseCard(int);

private:
	int _randlist[5];
	int _randPos;

	//11.11
	

};