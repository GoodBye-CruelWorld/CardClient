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
// 创建精灵的五种方法  
//方法一：直接创建精灵  
//适合于要显示的是这张图片的全部区域，  
CCSprite * sprite = CCSprite::create("Icon.png");
//上面那句话也可以根据需要这样来写：  
//CCString* fileName = CCString::createWithFormat("Icon_%d.jpg", flag);  
//CCSprite* sprite = CCSprite::create(fileName->getCString());  
sprite->setPosition(ccp(100, 100));
this->addChild(sprite);
// 方法二：参数 图片名称 矩形区域  
//适合于需要显示此图片的部分区域  
CCSprite * sprite = CCSprite::create("Icon.png", CCRectMake(0, 0, 30, 30));
sprite->setPosition(ccp(100, 100));
this->addChild(sprite);
//方法三： 利用帧缓存中的一帧的名称声称一个对象  
// 适合于plist打包好的文件  
CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("test_icon.plist");
CCSprite * sprite = CCSprite::createWithSpriteFrameName("Icon.png");
sprite->setPosition(ccp(100, 100));
this->addChild(sprite);
//方法四： 利用另外一帧生成一个精灵对象    
//适合于做帧动画使用  
CCSpriteFrame * frame = CCSpriteFrame::create("Icon.png", CCRectMake(0, 0, 40, 30));
CCSprite * sprite = CCSprite::createWithSpriteFrame(frame);
sprite->setPosition(ccp(310, 150));
addChild(sprite);
//方法五：利用纹理，  
//适合于需要频繁使用的图片  
CCSpriteBatchNode* spriteTexture = CCSpriteBatchNode::create("iocn.png");
spriteTexture->setPosition(CCPointZero);
addChild(spriteTexture);
CCSprite* sprite = CCSprite::createWithTexture(spriteTexture->getTexture());
sprite->setPosition(ccp(visiblesize.width / 2, 100));
spriteTexture->addChild(sprite, 2);
*/