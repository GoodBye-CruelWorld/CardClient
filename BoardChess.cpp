#include"BoardChess.h"

void BoardChess::onEnter()
{
	Node::onEnter();
	_camp = CAMP_UNDEFINE;
	_ID = -1;    //���δ����ID����Ϊ-1
}

void BoardChess::setCamp(int campID)
{
	_camp = campID;
}

int BoardChess::getCamp()
{
	return _camp;
}

void BoardChess::turnSide(float time )
{	
	this->runAction(Sequence::create(RotateBy::create(0.3f, Vec3(0, 90, 0)), CallFunc::create(CC_CALLBACK_0(BoardChess::turnSideCallback, this)), RotateBy::create(0.3f, Vec3(0, 90, 0)), NULL));
}

void BoardChess::turnSideCallback()
{
	if (this->_face&&this->_back)
	{
		this->_face->setVisible(!this->_face->isVisible());
		this->_back->setVisible(!this->_back->isVisible());
	}
}

void BoardChess::displayDescription()
{
	if (_description)
		_description->setVisible(true);
}

void BoardChess::hideDescription()
{
	if (_description)
		_description->setVisible(false);
}

int BoardChess::getID()
{
	return _chessID;
}