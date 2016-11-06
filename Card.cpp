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
	//���˵�� ���Ϊ7λ ǰ3λ��ʾְҵ 3λ��ʾ���� ����λΪ���
	/*ְҵ 00սʿ 01������ 100 Ϊ���� 200+Ϊboss
	����ͬǰ
	*/

	GameSqlite _gSql;
	Tool _tool;

	_armor = 0;
	_cardID = num;
	_profession = num / 10000;
	num = num % 1000;
	_type = num % 10;

	std::string _name = "Name", _desc = "Desc";



	if (num / 1000 == 0)
	{
		char s[10];
		std::string str;

		_health = atoi(_gSql.getCardData(_cardID, CARD_HEALTH));
		_attack = atoi(_gSql.getCardData(_cardID, CARD_ATTACK));
		_cost = atoi(_gSql.getCardData(_cardID, CARD_COST));
		_armor = atoi(_gSql.getCardData(_cardID, CARD_NAME));
		_quality = atoi(_gSql.getCardData(_cardID, CARD_QUALITY));

		sprintf_s(s, "%d", _cardID);
		str = s;
		_cardPath = "card/" + str + ".png";
		_cardName = "Name" + str;
		_cardDescribe = "Desc" + str;
		for (int i = 0; i <= 3; i++)
		{
			int _spell = atoi(_gSql.getCardData(_cardID, CARD_SPELL_1 + i));
			if (_spell < 100)
				break;
			_spellID.push_back(_spell);
		}
		//spell����
		if (_cardID == 10001){
			//int a = _spellID[0];//���ݿ���Ϊ3703001,Ϊ���Է����Ϊ5801003
			//set_armor(1);
			_spellID.push_back(3);
			_cardID += 2000;
			_health = 3;
			_attack = 2;
			//��С3��
			//1705002;//ѣ��ȫ��
			//	1711102	;//ʯ��
			//1710102;//����
			//1708002 ��ŭ

			//_spellID.push_back(5801003);սʿ���
			/*
			���� X001-- - Ϊ + X����

			�ͽײ���
			ս��1���� 1600002

			��ǹ�̴� ��̸

			װ������
			�ѷ���� + 2���� 2601102

			����
			�з���� xuanyun 1602102

			����
			�غϽ��� ���� + 2 2603001

			ǰ��
			���������ѷ����׺� 1604099
			��buff
			1 5 ����ȡ�ѷ����׺�

			����
			ս��5���� 5605002 - 5001002

			ս��
			ս��4���� 4606002 - 4001002
			��ӻ��� 90199*/


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
	_cost = 1;
	relife();

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
	if (_attackBattle + _attackBuff< 0)  return 0;
	return _attackBattle + _attackBuff;
}
int CCard::getFinalHealth(){
	return _healthBattle + _healthBuff;
}
int CCard::getFinalCost(){
	return _costBattle + _costBuff;
}

//10.6 buff�����

void CCard::addBuff(Buff& buff){
	_cardbuff.push_back(buff);
	if (buff._bufftype == 0){
		//��ֵ�Ķ�
		_costBattle += buff._buffdata[0];
		_attackBattle += buff._buffdata[1];
		_healthMax += buff._buffdata[2];
		_healthBattle += buff._buffdata[2];
	}
	canAttack();
}

void CCard::deleteBuff(int k){
	if (_cardbuff[k]._bufftype == 0){
		//��ֵ��ԭ �ݲ�������
		_costBattle -= _cardbuff[k]._buffdata[0];
		_attackBattle -= _cardbuff[k]._buffdata[1];
		_healthMax -= _cardbuff[k]._buffdata[2];
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
	if (_attacktime <= 0 || _attackBattle <= 0){

		_canAttack = false;
		_isAttack = true;
	}
	else{
		_canAttack = true;
		_isAttack = false;
	}
}




int CCard::buffCheck(int sTime){
	int k = _cardbuff.size();
	for (int i = 0; i < k; i++){
		if (sTime == 0){
			if (_cardbuff[i]._bufftype == 1){
				if (_cardbuff[i]._buffid == 2){
					_attacktime = 0;
				}
				if (_cardbuff[i]._buffid == 3){

					_attacktime *= 2;
				}
			}
		}

		if (sTime == 1){
			if (_cardbuff[i]._bufftype == 1){
				if (_cardbuff[i]._buffid == 5){
					//թʬ
				}
			}
		}
	}
	return 0;
}
bool CCard::buffCheck(int sTime,int buf){
	int k = _cardbuff.size();
	for (int i = 0; i < k; i++){
		if (sTime == 1){
			if (_cardbuff[i]._bufftype == 1){
				if (_cardbuff[i]._buffid == buf){
					//թʬ
					return true;
				}
			}
		}
	}
	return false;
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

void CCard::relife(){
	_healthBuff = 0;
	_healthBattle = _health;
	_healthMax = _health;
	_attackBuff = 0;
	_attackBattle = _attack;
	_costBattle = _cost;
	_costBuff = 0;
	
}