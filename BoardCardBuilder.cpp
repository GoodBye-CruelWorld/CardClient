#include"BoardCardBuilder.h"

Sprite* BoardCardBuilder::buildCardInBattle(int cardID)
{
	
	char c[40];
	int a = cardID % 100;//1,2,,10
	int b = cardID / 10000;//7,5
	sprintf(c, "battleScene/card/%d%d.png", b, a);
	CCSprite * sprite = CCSprite::create(c, CCRectMake(55, 752-571-70, 414, 752-130+5));
	sprite->setScale(0.17);

	return sprite;
}