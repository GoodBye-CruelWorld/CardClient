#include "string"
#include<vector>
#include"GameSqlite.h"
using namespace std;
class User
{
public:
	User();
	User(string Account);

	void setMoney(int Money);
	void setData(string Account);
	//void setPassWord(int PassWord);
	//void setUserName(int UserName);

	int getMoney();
	int getID();
	string getAccount();
	string getPassWord();
	string getUserName();

private:
	GameSqlite _sql;
	string _account, _passWord, _userName;
	int ID, _money;
	vector<char*> _cardArray;


};