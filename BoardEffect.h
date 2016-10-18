#pragma once
#include"cocos2d.h"
USING_NS_CC;

/*��ЧĬ�ϳ���ʱ��*/
#define DEFAULT_DURATION 0
#define EFFECT_FLOWER 0
#define EFFECT_FIREBALL 5
/*
*@BoardEffect
*brief a union of effects which can  be used in chessboard,including changes of board,effects manifest on the board.
1.��ͬʱ��Ӷ����Ч
2.��δ������Ч����Ӷ�����������Լ�����
3.��δ�������ʱ�䣬�������Ч����Ĭ�ϳ���ʱ��
*/
class BoardEffect:public Node
{
public:
	virtual void onEnter()override;
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
	void addEffect(int effectID, float delay=0,float duration = DEFAULT_DURATION, float positionX = 0, float positionY = 0, float positionZ = 0);
	void addEffect(int effectID, Node * target, float delay = 0, float duration = DEFAULT_DURATION, float positionX = 0, float positionY = 0, float positionZ = 0);
private:
	void endCallback(Node* sender); //����Ч�����������������
	Vec3 effects[100];	//x=effectID,y=effectKeyTime,z=effectLastTime
};