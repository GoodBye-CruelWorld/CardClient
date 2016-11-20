#pragma once
#include"cocos2d.h"
USING_NS_CC;
/*��Ч��*/
#define EFFECT_LAYER 10
/*��ЧĬ�ϳ���ʱ��*/
#define DEFAULT_DURATION 0
#define EFFECT_FLOWER 0
#define EFFECT_FIREBALL 5
#define EFFECT_BIT 4
#define EFFECT_BUFF 6
#define EFFECT_DEBUFF 7
#define EFFECT_FIRE_FLASH 8
#define EFFECT_FROZEN 9
#define EFFECT_BUFF_BOARD 10
#define EFFECT_HEAL 11
#define EFFECT_FOSS 12
#define EFFECT_ARROW 13
#define EFFECT_BOMB 14
/*
*@BoardEffect
*brief a union of effects which can  be used in chessboard,including changes of board,effects manifest on the board.
1.��ͬʱ��Ӷ����Ч
2.��δ������Ч����Ӷ�����������Լ�����
3.��δ�������ʱ�䣬�������Ч����Ĭ�ϳ���ʱ��
*/
class BoardEffect
{
	static BoardEffect *_instance;
private:
	BoardEffect(){};
	
public:
	static BoardEffect* getInstance();
public:
	void initEffectsInfo();
	/**
	*@getKeyTimeOfEffect
	*@brief ��ȡ��Ч�Ĺؼ�ʱ���
	*@param effectID ��ЧID
	*@return ����Ч��ʼ���ؼ�ʱ����ʱ��
	*/
	float getKeyTimeOfEffect(int effectID);
	float getLastTimeOfEffect(int effectID);
	void addEffect(int effectID, float lastTime, Node *src, Node*dest);
	
private:
	ParticleSystemQuad* addParticle(std::string fileName, Node* dest, float scale=NULL, float rotation=NULL, Vec2 position=Vec2(0,0));
	void addAnimation(std::string fileName, std::string fileType, int size,Node* dest);
	//ParticleSystemQuad* addParticleEffect(std::string fileName,Node* dest);
	void endCallback(Node* sender); //����Ч�����������������
	Vec3 effects[100];	//x=effectID,y=effectKeyTime,z=effectLastTime
};