#pragma once 
#include"Battle.h"
/**
*C
*
*
*/
class Command
{
public:
	static Command* getInstance();
	void sendCommand(int battleID,int camp=0);
	void setBattles(CBattle*my, CBattle*enemy);
private:
	CBattle*_battles[2];
	static Command *_instance;
};