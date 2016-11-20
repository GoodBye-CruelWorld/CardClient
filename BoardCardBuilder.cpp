#include"BoardCardBuilder.h"

Sprite* BoardCardBuilder::buildCardInBattle(int cardID)
{
	//011
	
	auto c = convertCardIDtoPath(cardID);
	CCSprite * sprite = CCSprite::create(c.c_str(), CCRectMake(55, 111, 414,627));
	sprite->setScale(0.17);

	return sprite;
}

Sprite* BoardCardBuilder::buildCardPhote(int cardID)
{
	auto c = convertCardIDtoPath(cardID);
	CCSprite * sprite = CCSprite::create(c.c_str(), CCRectMake(80,70, 360, 360));
	sprite->setScale(0.15);

	return sprite;
}

Sprite* BoardCardBuilder::buildCardInSet(int cardID)
{
	auto c = convertCardIDtoPath(cardID);
	CCSprite * sprite = CCSprite::create(c.c_str(), CCRectMake(80, 60, 159*2, 43*4));
	sprite->setScaleX(0.5);
	sprite->setScaleY(0.25);

	return sprite;
}

std::string BoardCardBuilder::convertCardIDtoPath(int cardID)
{
	char c[40];
	int a = cardID % 100;//1,2,,10
	int b = cardID / 10000;//0,1,10,11
	int num = cardID / 1000 % 10;
	switch (num)
	{
	case 0:
		if (b >= 10)
			sprintf(c, "battleScene/card/0%d%d.png", b, a);
		else
			sprintf(c, "battleScene/card/%d%d.png", b, a);
		break;
	case 1:
		sprintf(c, "battleScene/card/m%d%d.png", b, a);
		break;
	case 2:
		sprintf(c, "battleScene/card/w%d%d.png", b, a);
		break;
	case 3:
		sprintf(c, "battleScene/card/e%d%d.png", b, a);
		break;
	default:
		break;
	}
	return c;
}