#pragma once
#include<vector>
#include"Card.h"
#include"GameBoard.h"
#include"GameSocket.h"

using namespace std;

class CAIEnemy;

enum GameState{
	GAME_BEGIN,
	GAME_RUN,  //�Լ��Ļغ�
	GAME_WAIT, //�Է��Ļغ�
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
	ս������
		��Ϸ��ʼ-׼���ƿ�-���ʼ��
		��Ϸ��
		��Ϸ����
	*/
	CBattle *_enemy,*_wild;//���ݶ���ս�����ָ��

	void gameStart();
	void gameOver();
	void gaming();


	/*
	�������� ##
		�غϿ�ʼ����-ˮ������-����ˮ��-����

		���
		��Ӽ������-�Ƴ�
		Ӣ�ۼ������-�Ƴ� to be writen
		

		����
		��ӹ���
		�ٻ����
		����
		�غϽ���
	*/
	/*
	*/
	void turnStart();
	void turnOver();

	virtual void turning();

	void actionChange(int num);
	vector<CCard>& checkPosition(CCard &card);//����Ƶ�λ��


	void drawCard();//����
	//void drawCard(vector<CCardinHand> &cardIH, vector<CCardinDeck> &cardID);
	//void cardAttack(CCard &creAttack, CCard &creBeattacked);	//��Ӽ�Ĺ���
//	void cardAttack(CCard &creAttack);							//��ӹ���Ӣ��
	void cardAttack(int srcNum, int srcCamp, int destNum, int destCamp);
	void cardAttack(int num);							//��ӹ���Ӣ�� ����+1

	void roleAttack();											//Ӣ�۹���Ӣ��
	void roleAttack(CCard &creBeattacked);						//Ӣ�۹������
	//void CreatureCommon(CCard card, vector<CCardinBattlefield> cardIB);//�����ù���

	void cardCummon(vector<CCard>&card1, vector<CCard>&card2, int num1, int num2,int battlePlace=0);//�ٻ� 
	void cardCummon(int srcPool, int destPool, int num1, int num2, int battlePlace = 0);//�ٻ� ����
	//void CardTranslate(vector<CCard>&card1, vector<CCard>&card2, int num1,int num2);//���ݿ��� 1Ϊԭ�� 2Ϊ����֮�� numΪ����
	//void CardTranslate(CCard &card, vector<CCard>&card2, int num2);
	void cardTransfer(int srcPool,int destPool, int srcNum, int destNum,int battlePlace=0);


	void cardDead(int num);
	//void cardDead(CCard &card);
	//void cardDeadE(CCard &card);

	void check();//������з����¼�����
	void checkDead();


	void spellCheck();
	//���ܷ������ּ���ʱ�� �غϿ�ʼ00 �غϽ���01 �ٻ�02 ����03
	void spellCheck(int sTime);
	void spellCheck(int srcPool, int srcNum, int sTime);
	void spellCheckPlayer(int sTime);
	void spellCheckEnemy(int sTime);
	void spelling(int spell_num);//�������� ����ID��
//	bool Spelling(int spell_num, CCard &card1);
	void spelling(int spell_num, int srcPool, int srcNum,int srcCamp);
	void skillSpelling(int spellNum, int srcPool, int srcNum);
	void skillSpelling();//������������
	int getCardPos(int cardPool,CCard &card);//��ȡcard�������е�λ��,û���򷵻�-1
	//void CreatureDead(CCardinBattlefield &creature);
	void creatureDead(CCard &card);
	bool heroIsDead();
	void setBattleID(int battleID);
	/*
	��Ա���� vector��
	�ƿ���� ���� ���� Ĺ��
	*/
//	vector<CCard>CinBattle, CinHand, CinDeck, CinCeme;
	/*�±����ΪPOOL_HAND,POOL_DECK,POOL_CEME,POOL_BATTLE*/
	vector<CCard> _cardPool[4];

	BoardRole *_hero;
	int actionPoints,ActPtsMax;//�ж����������ֵ

	int _battleID;
//	int *_battleID;				// ���ڴ��潻���õ�ID��ַ
	bool _battleState;			// �û���Ϣ��Ӧ�ı�־
	GameState _gameState;  //�ж���Ϸ����״̬


	void update(float dt);

	GameBoard *_gameboard;
	int _camp;
	int *_cardIds; 
	GameSocket *_gameSocket;
	bool _gameMode;
	bool _firstHand;

	//10.6 ���
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