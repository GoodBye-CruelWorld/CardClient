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
}