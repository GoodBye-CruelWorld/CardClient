#pragma once
#include "string"
#include<vector>
#include"GameSqlite.h"
using namespace std;


struct USERCardArray
{
	string _arrayName;
	int _roleID;				//ID��������CardID��ɣ���+0000000
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

	int getMoney();					//���ID
	int getID();					//���money
	string getAccount();
	string getPassWord();
	string getUserName();


	//����
	int createCardArray(string cardName,int roleID);		//�������飬����cardArrayID
	void addCardArray(int cardArrayID);						//���ӿ��飬��ӵ����ݿ�
	bool delCardArray(int cardArrayID);						//ɾ������
	int getCardArrayNumber();								//������еĿ�������
	vector<int> getCardformArray(int cardArrayID);			//��õ�ǰ��������п�
	bool addCardintoArray(int cardArrayID, int cardID);		//�ڿ��������ӿ���
	bool delCardofArray(int cardArrayID, int cardID);		//ɾ������
	vector<int> getUserAllCard();							//����û������п���
	bool addUserCard(int CardID);							//���ӿ���




private:
	GameSqlite _sql;
	string _account, _passWord, _userName;
	int ID, _money;
	//�û��Ŀ������飬�濨��ID
	vector<USERCardArray> _cardArraySet;

};