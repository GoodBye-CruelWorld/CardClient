#include"BoardEffect.h"
#include"GameBoard.h"

BoardEffect* BoardEffect::_instance = nullptr;

BoardEffect* BoardEffect::getInstance()
{
	if (_instance != NULL)
		return _instance;
	else
	{
		_instance = new BoardEffect();
		return _instance;
	}
}


void BoardEffect::initEffectsInfo()
{
	for (int i = 0; i < 100; i++)
	{
		effects[i].x = i;
	}
	effects[EFFECT_FLOWER].y = 1.f;
	effects[EFFECT_FLOWER].z = 3.f;
	effects[EFFECT_FIREBALL].y = 0.8f;
	effects[EFFECT_FIREBALL].z = 1.f;
	effects[EFFECT_FIRE_FLASH].y = 0.8f;
	effects[EFFECT_FIRE_FLASH].z = 1.f;
}
float BoardEffect::getKeyTimeOfEffect(int effectID)
{
	return effects[effectID].y;
}
float BoardEffect::getLastTimeOfEffect(int effectID)
{
	return effects[effectID].z;
}
void BoardEffect::addEffect(int effectID, float lastTime, Node *src, Node*dest)
{

	switch (effectID)
	{
		//多彩粒子环绕效果
	case EFFECT_FLOWER:
	{	
		auto emitter = ParticleFlower::create();  //花效果，可换成其他ParticleFire，ParticleExplosion等等
		emitter->setTexture(Director::getInstance()->getTextureCache()->addImage("effects/particles/stars.png"));//设置贴图
		emitter->setStartColor(Color4F(0.1, 0.5, 0.5, 0.5));
		emitter->setEndColor(Color4F(0.7, 0.1, 0.1, 0.2));

		emitter->setPosition3D(dest->getPosition3D());
		dest->addChild(emitter);
		emitter->runAction(Sequence::create(ScaleTo::create(lastTime*2.f / 3.f, 3.0f), /*淡出*/FadeOut::create(lastTime / 3.f), CallFunc::create(CC_CALLBACK_0(BoardEffect::endCallback, this, emitter)), NULL));
	}
		break;
		//火球冲击效果
	case EFFECT_FIREBALL:
	{	
		auto emitter = ParticleFire::create();
		emitter->setTexture(Director::getInstance()->getTextureCache()->addImage("effects/particles/stars.png"));//设置贴图
		emitter->setPosition3D(src->getPosition3D());
		GameBoard::getInstance()->addChild(emitter);
		emitter->runAction(Sequence::create(DelayTime::create(lastTime), CallFunc::create(CC_CALLBACK_0(BoardEffect::endCallback, this, emitter)), NULL));
		emitter->runAction(MoveTo::create(lastTime, dest->getPosition3D()));
	}
		break;
	case 1:
	{//网格震动效果//未实现
	/*	CCActionInterval* shaky3D = CCShaky3D::create(5, CCSize(10, 10), 15, false);

		Size visibleSize = Size(400, 400);
		Rect gridRect = Rect(visibleSize.width * 0.2,
			visibleSize.height * 0.2,
			visibleSize.width * 0.6,
			visibleSize.height * 0.6);
		auto _gridNodeTarget = NodeGrid::create(gridRect);
		_gridNodeTarget->setPosition(0, 0);
		target->addChild(_gridNodeTarget);
		_gridNodeTarget->runAction(shaky3D);

		auto s22 = Sprite::create("ChessBoard/decorations/1.png");
		s22->setPosition(2, 2);
		_gridNodeTarget->addChild(s22, 0);*/
		//s->setPosition(VisibleRect::center());
	}
		break;
	case 2:
	{	//上下震动

		dest->runAction(Sequence::create(JumpBy::create(lastTime, Vec2(0, 0), 15, 100), NULL));

	}
		break;
	case 3:
	{	//碎片


	}
		break;
	case EFFECT_BIT:
	{
		Animation *animation = Animation::create();
		auto node = Sprite::create();
		for (int i = 0; i < 4; i++)
		{
			char szName[50] = { 0 };
			sprintf(szName, "effects/png/%d.png", i);
			animation->addSpriteFrameWithFile(szName);
		}
		// should last 2.8 seconds. And there are 14 frames.
		animation->setDelayPerUnit(0.2f / 4.0f);
		animation->setRestoreOriginalFrame(true);

		auto action = Animate::create(animation);
		action->retain();
		node->runAction(Sequence::create(
			//DelayTime::create(delay),
			action,
			CallFunc::create(CC_CALLBACK_0(Node::removeFromParent, node)),
			NULL));
		node->setScale(0.7);
		dest->addChild(node);
	}
		break;
		//卡牌增益时的特效
	case EFFECT_BUFF:
	{
		addParticle("particles/shine.plist", dest);
		auto die_par = ParticleSystemQuad::create("particles/shine.plist");
		die_par->setPosition(dest->getContentSize()/2);
		die_par->setRotation(90);
		die_par->setScale(0.5);
		dest->addChild(die_par, EFFECT_LAYER);
	
	}
		break;
	case EFFECT_DEBUFF:
	{

		
	}
		break;
		//火焰冲击
	case EFFECT_FIRE_FLASH:
	{
		auto par=addParticle("particles/fireFlash.plist", src);
		par->runAction(Sequence::create(DelayTime::create(lastTime), CallFunc::create(CC_CALLBACK_0(BoardEffect::endCallback, this, par)), NULL));
		par->runAction(MoveTo::create(lastTime, dest->getPosition3D()));
	}
		break;
	default:
		break;
	}

}


void BoardEffect::endCallback(Node *sender)
{
	sender->removeFromParentAndCleanup(true);
}


ParticleSystemQuad* BoardEffect::addParticle(std::string fileName, Node* dest, float scale, float rotation, Vec2 position)
{
	auto par = ParticleSystemQuad::create(fileName);
	if (position !=Vec2(0,0))
		par->setPosition(position);
	else
		par->setPosition(dest->getPosition());

	if (rotation!=NULL)
		par->setRotation(rotation);



	if (scale != NULL)
		par->setScale(scale);

	GameBoard::getInstance()->addChild(par, EFFECT_LAYER);
	return par;
}