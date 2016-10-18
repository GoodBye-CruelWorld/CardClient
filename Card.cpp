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
			//spell����
			if (num == 0){
				_spellID.push_back(5801003);

			}
			if (num == 1){
				_spellID.push_back(5802002);
			}
			if (num == 2){
				_spellID.push_back(5802001);
			}
		}
		//if (num == 0){
		//	_health = 3;
		//	_cardPath = "card/0.png";
		//	_attack = 4;
		//	_cost = 2;
		//	_armor = 1;
		//	_quality = 0;
		//	_cardName = "Name0";
		//	_cardDescribe = "Desc0";
		//}
		//if (num == 1){
		//	_cost = 1;
		//	_attack = 2;
		//	_health = 3;
		//	_cardPath = "card/1.png";
		//	_quality = 0;
		//	_cardName = "Name1";
		//	//_cardName = "����";
		//	//_cardName = _tool.IConvConvert_GBKToUTF8("�������");
		//	//_cardName = _gSql.getCardData(num, CARD_NAME);

		//	_cardDescribe = "Desc1";
		//}
		//if (num == 2){
		//	_cost = 3;
	
		//	_attack = 3;
		//	_health = 3;
		//	_quality = 0;
		//	_cardName = "Name2";
		//	_cardDescribe = "Desc2";
		//	_spellID.push_back(2001102);
		//}
		//if (num == 3){
		//	_cost = 5;
		//	_attack = 4;
		//	_health = 3;
		//	_quality = 1;
		//	_cardName = "����սʿ";
		//	_cardDescribe = "���٣�ѣ��һ�����ֱ������¸��غϿ�ʼ";
		//	_spellID.push_back(2102);
		//}
		//if (num == 4){
		//	_cost = 6;
		//	_attack = 7;
		//	_health = 5;
		//	_quality = 1;
		//	_cardName = "Ӣ��";
		//	_cardDescribe = "������6";
		//	_spellID.push_back(6904001);
		//}
		//if (num ==5){
		//	_cost = 8;
		//	_attack = 7;
		//	_health = 7;
		//	_quality = 1;
		//	_cardName = "������ʿ";
		//	_cardDescribe = "ÿ���غϽ�����û���+2";
		//	_spellID.push_back(2001101);
		//}
		//if (num == 6){
		//	_cost = 2;
		//	_attack = 3;
		//	_health = 3;
		//	_quality = 2;
		//	_cardName = "ǰ��ָ�ӹ�";
		//	_cardDescribe = "�ѷ���ɫÿ��һ�㻤��ֵ�����+1������";
		//	_spellID.push_back(20099);
		//}
		//if (num == 7){
		//	_cost = 4;
		//	_attack = 5;
		//	_health = 1;
		//	_quality = 2;
		//	_cardName = "����";
		//	_armor = 5;
		//	_cardDescribe = "����5 ��������";
		//	_spellID.push_back(90004);
		//}
		//if (num == 8){
		//	_cost = 11;
		//	_attack = 10;
		//	_health = 10;
		//	_armor = 4;
		//	_quality = 3;
		//	_cardName = "ս��";
		//	_cardDescribe = "����4 ��ӻ��� ��������";
		//	_spellID.push_back(90004);
		//	_spellID.push_back(90199);
		//}
		if (num == 1000){
			_cost = 0;
			_quality = 0;
			_cardName = "�ط�ս��";
			_cardDescribe = "�ظ�һ�������������ֵ����ʹ���û���+1";
		}
		if (num == 1001){
			_cost = 0;
			_quality = 1;
			_cardName = "���";
			_cardDescribe = "��һ���ƣ��¸��غϿ�ʼǰ��Ӣ�ۻ�ù�������";
		}
		if (num == 1002){
			_cost = 2;
			_quality = 2;
			_cardName = "��������";
			_cardDescribe = "���غ���������ӣ��غϽ������·���ս������1������ֵ";
		}
		if (num == 1003){
			_cost = 4;
			_quality = 2;
			_cardName = "ս��׼��";
			_cardDescribe = "ÿ��һ���ѷ���ɫ�л��ף���һ����";
		}
		if (num == 2000){
			_cost = 1;
			_attack = 2;
			_health = 2;
			_quality = 0;
			_cardName = "����֮��";
			_cardDescribe = "����";
		}
		if (num == 2001){
			_cost = 3;
			_attack = 4;
			_health = 2;
			_quality = 0;
			_cardName = "���ֽ�";
			_cardDescribe = "����";
		}
		if (num == 2002){
			_cost = 4;
			_attack = 0;
			_health = 3;
			_quality = 1;
			_cardName = "����װ��";
			_cardDescribe = "�غ���Ӣ�ۻ�û���+1������+1��ӵ�й�������";
		}
		if (num == 2003){
			_cost = 2;
			_attack = 1;
			_health = 2;
			_quality = 2;
			_cardName = "��Ѫ��Ʒ";
			_cardDescribe = "�������˵����ʱ��ֱ������";
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

//10.6 buff�����

void CCard::addBuff(Buff& buff){
	_cardbuff.push_back(buff);
	if (buff._bufftype == 0){
		//��ֵ�Ķ�
		_costBattle += buff._buffdata[0];
		_attackBattle += buff._buffdata[1];
		_attackBattle = _attackBattle;
		_healthBattle += buff._buffdata[2];
	}
}

void CCard::deleteBuff(int k){
	if (_cardbuff[k]._bufftype == 0){
		//��ֵ��ԭ �ݲ�������
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
	if (_attacktime == 0){
		_canAttack = false;
		_isAttack = true;
	}
	else{
		_canAttack = true;
		_isAttack = false;

	}
}

void CCard::buffCheck(){
	int k = _cardbuff.size();
	for (int i = 0; i < k; i++){
		if (_cardbuff[i]._bufftype == 1){
			if (_cardbuff[i]._buffid == 1){
				_attacktime == 0;
			}
			if (_cardbuff[i]._buffid == 2){
				_attacktime *= 2;
			}
		}
	}
}