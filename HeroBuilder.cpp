#include"HeroBuilder.h"

Sprite* HeroBuilder::buildHeroSkill(int skillID)
{
	char c[]="battleScene/skill/skill.png";
	int reflect=skillID;
	switch (skillID)
	{
	case 0:
		reflect = 5;
		break;
	case 1:
		reflect = 4;
		break;
	case 2:
		reflect = 2;
		break;
	case 10:
		reflect = 23;
		break;
	case 11:
		reflect = 22;
		break;
	case 12:
		reflect = 20;
		break;
	case 110:
		reflect = 12;
		break;
	case 111:
		reflect = 11;
		break;
	case 112:
		reflect = 13;
		break;
	default:
		break;
	}
	int y = reflect / 10;
	int x = reflect % 10;
	Rect rect = Rect(12 + x * 76, 335 - y * 78, 76, 76);
	Sprite * sprite = Sprite::create(c,rect);//321,257
	if (skillID%10 != 2)
		sprite->setScale(0.8);
	else
		sprite->setScale(1.0);

	return sprite;
}

Sprite*	HeroBuilder::buildHeroPhote(int cardID)
{
	return NULL;
}