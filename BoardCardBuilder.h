#pragma once
#include"cocos2d.h"
USING_NS_CC;

class BoardCardBuilder
{
private:
public:

	static Sprite* buildCardInBattle(int cardID);
	static Sprite* buildCardInBook(int cardID);
	static Sprite*	buildCardPhote(int cardID);
	static Sprite* buildCardBack(int backID);
};

/*
// ������������ַ���  
//����һ��ֱ�Ӵ�������  
//�ʺ���Ҫ��ʾ��������ͼƬ��ȫ������  
CCSprite * sprite = CCSprite::create("Icon.png");
//�����Ǿ仰Ҳ���Ը�����Ҫ������д��  
//CCString* fileName = CCString::createWithFormat("Icon_%d.jpg", flag);  
//CCSprite* sprite = CCSprite::create(fileName->getCString());  
sprite->setPosition(ccp(100, 100));
this->addChild(sprite);
// ������������ ͼƬ���� ��������  
//�ʺ�����Ҫ��ʾ��ͼƬ�Ĳ�������  
CCSprite * sprite = CCSprite::create("Icon.png", CCRectMake(0, 0, 30, 30));
sprite->setPosition(ccp(100, 100));
this->addChild(sprite);
//�������� ����֡�����е�һ֡����������һ������  
// �ʺ���plist����õ��ļ�  
CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("test_icon.plist");
CCSprite * sprite = CCSprite::createWithSpriteFrameName("Icon.png");
sprite->setPosition(ccp(100, 100));
this->addChild(sprite);
//�����ģ� ��������һ֡����һ���������    
//�ʺ�����֡����ʹ��  
CCSpriteFrame * frame = CCSpriteFrame::create("Icon.png", CCRectMake(0, 0, 40, 30));
CCSprite * sprite = CCSprite::createWithSpriteFrame(frame);
sprite->setPosition(ccp(310, 150));
addChild(sprite);
//�����壺��������  
//�ʺ�����ҪƵ��ʹ�õ�ͼƬ  
CCSpriteBatchNode* spriteTexture = CCSpriteBatchNode::create("iocn.png");
spriteTexture->setPosition(CCPointZero);
addChild(spriteTexture);
CCSprite* sprite = CCSprite::createWithTexture(spriteTexture->getTexture());
sprite->setPosition(ccp(visiblesize.width / 2, 100));
spriteTexture->addChild(sprite, 2);
*/