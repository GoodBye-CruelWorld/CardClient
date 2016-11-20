#include"BoardCardBuilder.h"

Sprite* BoardCardBuilder::buildCardInBattle(int cardID)
{
	//011
	char c[40];
	int a = cardID % 100;//1,2,,10
	int b = cardID / 10000;//0,1,10,11
	int num = cardID / 1000 % 10;
	switch (num)
	{
	case 0:
		if (b>=10)
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
	
	CCSprite * sprite = CCSprite::create(c, CCRectMake(55, 752-571-70, 414, 752-130+5));
	sprite->setScale(0.17);

	return sprite;
}