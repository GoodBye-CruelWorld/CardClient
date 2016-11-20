#pragma once
#include "string"
#include<vector>
#include"GameSqlite.h"
using namespace std;


struct USERCardArray
{
	string _arrayName;
	int _roleID;				//ID由数量和CardID组成，×+0000000
	const int _cardNumberMax = 20;
	vector<int> _cardID;
	int _cardNumber;
};

class User
{
public:
	User();
	User(string Account);

	void setMoney(int Money);
	void setData(string Account);
	//void setPassWord(int PassWord);
	//void setUserName(int UserName);

	int getMoney();					//获得ID
	int getID();					//获得money
	string getAccount();
	string getPassWord();
	string getUserName();


	//卡组
	int createCardArray(string cardName,int roleID);		//创建卡组，返回cardArrayID
	void addCardArray(int cardArrayID);						//增加卡组，添加到数据库
	bool delCardArray(int cardArrayID);						//删除卡组
	int getCardArrayNumber();								//获得所有的卡组数量
	vector<int> getCardformArray(int cardArrayID);			//获得当前卡组的所有卡
	bool addCardintoArray(int cardArrayID, int cardID);		//在卡组中增加卡牌
	bool delCardofArray(int cardArrayID, int cardID);		//删除卡牌
	vector<int> getUserAllCard();							//获得用户的所有卡牌
	bool addUserCard(int CardID);							//增加卡牌




private:
	GameSqlite _sql;
	string _account, _passWord, _userName;
	int ID, _money;
	//用户的卡牌数组，存卡牌ID
	vector<USERCardArray> _cardArraySet;

};