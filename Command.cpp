#include"Command.h"
#include"GameBoard.h"
void Command::sendCommand(int command, int camp)
{
		_battles[camp]->setBattleID(command);
		switch (command / 1000000)
		{
		case 1:					//表示随从召唤
		{
			auto place = command / 100 % 10;
			_battles[camp]->cardCummon(POOL_HAND, POOL_BATTLE, command / 1000 % 10, command % 10, command / 100 % 10);
			GameBoard::getInstance()->getActionQueue()->reset(false);
			break;
		}
		case 02:					//道具法术装备的使用
		{
			int p = command / 1000 % 10;
			auto place = command / 100 % 10;
			if (p == 1)
				_battles[camp]->spellLaunch(POOL_HAND, command / 1000 % 10);
			else
				_battles[camp]->equipLaunch(POOL_HAND, command / 1000 % 10);
			GameBoard::getInstance()->getActionQueue()->reset(false);
			break;
		}
		case 03:					//	技能的使用
		{
			_battles[camp]->skillSpelling(command / 1000 % 10, command % 1000 / 10, command % 10);

			GameBoard::getInstance()->getActionQueue()->reset(false);
			break;
		}
		case 4:					//攻击
		{
			auto p = command / 1000 % 10;
			if (command / 10000 % 10 == 6) {
				p = 7; 
			}
			if (command / 10 % 10 != 7)
				//cardAttack(/*CinBattle*/_cardPool[POOL_BATTLE].at(command / 1000 % 10), /*CinBattle*/_cardPool[POOL_BATTLE].at(command / 1000 % 10));
			{
				auto a = command / 10 % 10;
				if (a == 5)
					_battles[camp]->cardAttack(p, 0, command % 10, 2);
				else
					_battles[camp]->cardAttack(p, 0, command % 10, 1);
				GameBoard::getInstance()->getActionQueue()->reset(false);
			}
			else
				//cardAttack(/*CinBattle*/_cardPool[POOL_BATTLE].at(command / 1000 % 10));
			{
				_battles[camp]->cardAttack(p);
				GameBoard::getInstance()->getActionQueue()->reset(false);
			}
			if (command / 10000 % 10 == 6) {
				_battles[camp]->_hero->reduceWeapon();
			}
			break;

		}
		case 5:  //回合开始
		{
			_battles[camp]->turnStart();
			break;
		}
		case 6: //回合结束
		{
			_battles[camp]->turnOver();
			break;
		}
		default:
			break;
		}
		
		char c[10];
		sprintf(c, "%d", command);
		_battles[camp]->_gameSocket->sendMsg(c);

		_battles[camp]->check();
	
}

void Command::setBattles(CBattle*my, CBattle*enemy)
{
	_battles[0] = my;
	_battles[1] = enemy;
}

Command*  Command::_instance = nullptr;

Command*  Command::getInstance()
{
	if (_instance != NULL)
		return _instance;
	else
	{
		_instance = new  Command();
		return _instance;
	}
}