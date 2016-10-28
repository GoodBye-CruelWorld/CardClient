#include "Card.h"
#include "GameSqlite.h"
#include "Tool.h"
CCard::CCard()
{
}

CCard::CCard(int num)
{
	cardCreate(num);
}

void CCard::cardCreate(int num)
{
	//编号说明 编号为7位 前3位表示职业 3位表示类型 后三位为编号
	/*职业 00战士 01弓箭手 100 为中立 200+为boss 
	类型同前
	*/

	GameSqlite _gSql;
	Tool _tool;
	_cardPath = "card/2.png";

	_armor = 0;
	_cardID = num;
	_profession = num / 10000;
	num = num % 1000;
	_type = num % 10;

	std::string _name = "Name",_desc="Desc";
	

	if (_profession == 00){
		if (num / 1000 == 0)
		{
			char s[10];
			std::string str;

			_health = atoi(_gSql.getCardData(num, CARD_HEALTH));
			_attack = atoi(_gSql.getCardData(num, CARD_ATTACK));
			_cost = atoi(_gSql.getCardData(num, CARD_COST));
			_armor = atoi(_gSql.getCardData(num, CARD_NAME));
			_quality = atoi(_gSql.getCardData(num, CARD_QUALITY));

			sprintf_s(s, "%d", num);
			str = s;
			_cardPath = "card/" + str + ".png";
			_cardName = "Name" + str;
			_cardDescribe = "Desc" + str;
			for (int i = 0; i <= 3; i++)
			{
				int _spell = atoi(_gSql.getCardData(num, CARD_SPELL_1 + i));
				if (_spell < 100)
					break;
				_spellID.push_back(_spell);
			}
			//spell测试
			if (num == 0){
				//int a = _spellID[0];//数据库中为3703001,为测试方便改为5801003

				_spellID[0] =
					2500007;
				_cardID += 1000;
					//1710102;//冰冻
					//1708002 风怒

				//_spellID.push_back(5801003);

			}
			if (num == 1){
				//int a = _spellID[0];//5802002
				//log("5801002--" + _spellID[0]);
				//_spellID.push_back(5802002);
			}
			if (num == 2){
				//int a = _spellID[0];//5802001
				//log("5802001--" + _spellID[0]);
				//_spellID.push_back(5802001);
			}
		}
	}
	
	_healthBuff = 0;
	_healthBattle = _health;
	_attackBuff = 0;
	_attackBattle = _attack;
	_costBattle = _cost;
	_costBuff = 0;
}

CCard::~CCard()
{
}

void CCard::damaged(int x){
	_healthBattle -= (x - 0);
}

int CCard::heal(int x){
	x = (_healthBattle + x > _healthMax) ? _healthMax - _healthBattle : x;
	_healthBattle += x;
	return x;

}

bool CCard::isDead(){
	if (getFinalHealth() <= 0) return true;
	return false;
}

void CCard::getBuff(int _health_change, int _attack_change){
	_healthBattle += _health_change;
	_attackBattle += _attack_change;
}

int CCard::getFinalAttack(){
	return _attackBattle + _attackBuff;
}
int CCard::getFinalHealth(){
	return _healthBattle + _healthBuff;
}
int CCard::getFinalCost(){
	return _costBattle + _costBuff;
}

//10.6 buff类添加

void CCard::addBuff(Buff& buff){
	_cardbuff.push_back(buff);
	if (buff._bufftype == 0){
		//数值改动
		_costBattle += buff._buffdata[0];
		_attackBattle += buff._buffdata[1];
		_attackBattle = _attackBattle;
		_healthBattle += buff._buffdata[2];
	}
}

void CCard::deleteBuff(int k){
	if (_cardbuff[k]._bufftype == 0){
		//数值还原 暂不验上限
		_costBattle -= _cardbuff[k]._buffdata[0];
		_attackBattle -= _cardbuff[k]._buffdata[1];
		_healthBattle -= _cardbuff[k]._buffdata[2];
	}
	_cardbuff.erase(_cardbuff.begin() + k);
}

void CCard::clearBuff(){
	int k = _cardbuff.size();
	for (int i = 0; i < k; i++){
		if (_cardbuff[i]._times == 0){
			deleteBuff(i);
			i--; k--;
		}
	}
}

void CCard::reduceBuffTimes(){
	int k = _cardbuff.size();
	for (int i = 0; i < k; i++){
		if (_cardbuff[i]._times > 0){
			_cardbuff[i]._times--;

			if (_cardbuff[i]._times == 0){
				deleteBuff(i);
				i--; k--;
			}

		}
	}
}

void CCard::deleteBuffFromResource(int resource){
	int k = _cardbuff.size();
	for (int i = 0; i < k; i++){
		int r = _cardbuff[i]._resource;
		if (_cardbuff[i]._resource == resource){
			deleteBuff(i);
			i--; k--;
		}
	}
	clearBuff();
}

void CCard::canAttack(){
	if (_attacktime <= 0){

		_canAttack = false;
		_isAttack = true;
	}
	else{
		_canAttack = true;
		_isAttack = false;

	}
}




void CCard::buffCheck(int s){
	int k = _cardbuff.size();
	for (int i = 0; i < k; i++){
		if (_cardbuff[i]._bufftype == 1){
			if (_cardbuff[i]._buffid == 2){
				_attacktime = 0;
			}
			if (_cardbuff[i]._buffid == 3){

				_attacktime *= 2;
			}
		}
	}

}

bool CCard::buffCheck(Buff buff)
{
	for (int i = 0; i < _cardbuff.size(); i++)
	{
		if (_cardbuff[i]._buffid == buff._buffid &&_cardbuff[i]._bufftype == buff._bufftype)
			return true;
	}


	return false;

}