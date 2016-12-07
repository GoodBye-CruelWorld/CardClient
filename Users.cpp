#include "Users.h"
using namespace std;

User::User()
{
}

User::User(string Account)
{
	char * _ID = _sql.getUsersID(Account);
	ID = atoi(_ID);
	_account = Account;
	_passWord = _sql.getUsersData(_ID, USER_PASSWORD);
	_userName = _sql.getUsersData(_ID, USER_NAME);
	_money = atoi(_sql.getUsersData(_ID, USER_MONEY));
}

int User::getID()
{
	return ID;
}
int User::getMoney()
{
	return _money;
}
string User::getAccount()
{
	return _account;
}
string User::getPassWord()
{
	return _passWord;
}
string User::getUserName()
{
	return _userName;
}

void User::setMoney(int Money)
{
	_money = Money;
	char sID[10];
	sprintf_s(sID, "%d", ID);
	string strID = sID;
	char sMny[10];
	sprintf_s(sMny, "%d", Money);
	string strMny = sMny;
	_sql.upDateUserData(strID, USER_MONEY, strMny);
}

void User::setData(string Account)
{
	char * _ID = _sql.getUsersID(Account);
	ID = atoi(_ID);
	_account = Account;
	_passWord = _sql.getUsersData(_ID, USER_PASSWORD);
	_userName = _sql.getUsersData(_ID, USER_NAME);
	_money = atoi(_sql.getUsersData(_ID, USER_MONEY));

	vector<int> result=_sql.getUserAllArray(ID);
	for (int i = 0; i < result.size(); i++)
	{
		
		
		USERCardArray array;
		array._arrayName = _sql.getArrayData(result.at(i),ARRAY_NAME);
		array._roleID = atoi(_sql.getArrayData(result.at(i), ARRAY_ROLEID));
		array._cardNumber = 0;
		array._cardID = _sql.getArrayAllCard(result.at(i));
		for (int j = 0; j < array._cardID.size(); j++)
		{
			array._cardNumber += array._cardID.at(j) / 10000000;
		}
		_cardArraySet.push_back(array);
	}
}


int	User::createCardArray(string cardName, int roleID)		//�������飬����cardArrayID���������е�λ��
{
	USERCardArray array;
	array._arrayName = cardName;
	array._roleID = roleID;
	array._cardNumber = 0;
	_cardArraySet.push_back(array);
	return _cardArraySet.size()-1;
}
void		User::addCardArray(int cardArrayID)						//���ӿ��飬��ӵ����ݿ�
{
	_sql.addCardArray(ID, _cardArraySet.at(cardArrayID)._roleID, _cardArraySet.at(cardArrayID)._arrayName);
	int _cardArrayID = _sql.getArrayID(_cardArraySet.at(cardArrayID)._arrayName);
	for (int i = 0; i < _cardArraySet.at(cardArrayID)._cardID.size(); i++)
	{
		int	num = _cardArraySet.at(cardArrayID)._cardID.at(i) / 10000000;
		for (int j = 0; j < num; j++)
			_sql.addArrayCard(_cardArrayID, _cardArraySet.at(cardArrayID)._cardID.at(i) % 10000000);
	}
}
bool		User::delCardArray(int cardArrayID)						//ɾ������
{
	int _cardArrayID = _sql.getArrayID(_cardArraySet.at(cardArrayID)._arrayName);
	_sql.delCardArray(_cardArrayID);
	return true;
}
int	User::getCardArrayNumber()								//������еĿ�������
{

	
	return _cardArraySet.size();
}


vector<int> User::getCardformArray(int cardArrayID)					//��õ�ǰ��������п�
{
	return _cardArraySet.at(cardArrayID)._cardID;
}
bool User::addCardintoArray(int cardArrayID, int cardID)		//�ڿ��������ӿ���
{
	if (_cardArraySet.at(cardArrayID)._cardNumber >= _cardArraySet.at(cardArrayID)._cardNumberMax)
		return false;
	int i = 0;
	for (i = 0; i < _cardArraySet.at(cardArrayID)._cardID.size(); i++)
	{
		int	num = _cardArraySet.at(cardArrayID)._cardID.at(i) % 10000000;
		if (cardID == num)
			_cardArraySet.at(cardArrayID)._cardID.at(i) += 10000000;		
		break;
	}
	if (i >= _cardArraySet.at(cardArrayID)._cardID.size())
		_cardArraySet.at(cardArrayID)._cardID.push_back(10000000 + cardID);
	_cardArraySet.at(cardArrayID)._cardNumber++;
	return true;
}
bool		User::delCardofArray(int cardArrayID, int cardID)		//ɾ������
{
	for (int i = 0; i < _cardArraySet.at(cardArrayID)._cardID.size(); i++)
	{
		int	num = _cardArraySet.at(cardArrayID)._cardID.at(i) % 10000000;
		if (cardID == num)
		{
			_cardArraySet.at(cardArrayID)._cardID.at(i) -= 10000000;
			if (_cardArraySet.at(cardArrayID)._cardID.at(i) / 10000000 == 0)
				_cardArraySet.at(cardArrayID)._cardID.erase(_cardArraySet.at(cardArrayID)._cardID.begin()+i);
		}
		break;
	}
	_cardArraySet.at(cardArrayID)._cardNumber--;
	return true;
}
vector<int> User::getUserAllCard()									//����û������п���
{
	return _sql.getUserAllCard(ID);
}
bool		User::addUserCard(int CardID)								//���ӿ���
{
	_sql.addUserCard(CardID, ID);
	return true;
}