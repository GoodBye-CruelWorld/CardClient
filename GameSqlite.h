extern"C"
{
#include"sqlite3.h"
}
#include<iostream>
#include<vector>

using namespace std;

//数据库的类别
#define SQL_CARD			0
#define SQL_CHARACTER		1
#define SQL_SKILL			2
#define SQL_ASSIGNMENT		3
#define SQL_ACHIEVEMENT		4
#define SQL_DIALOGUE		5
#define SQL_ACCOUNT			6


//卡牌数据库
#define	WITCHCRAFT			1
#define	RETINUE				0
#define	EQUIPMENT			2

#define CARD_ID				0
#define	CARD_NAME			1
#define	CARD_COST			2
#define	CARD_HEALTH			3
#define	CARD_ATTACK			4
#define	CARD_DESCRIBE		5
#define	CARD_QUALITY		6
#define	CARD_SPELL_1		7
#define	CARD_SPELL_2		8
#define	CARD_SPELL_3		9
#define	CARD_SPELL_4		10
#define	CARD_GRAPHPATH_1	11
#define	CARD_GRAPHPATH_2	12


//////////////////////////////////////////////////////////////////////////数据库类
class GameSqlite
{
public:
	GameSqlite();
	~GameSqlite();
	//获得响应ID字段的回调函数
	static int loadRecord( void * para, int n_column, char ** column_value, char ** column_name);
	int Sql_Type;

	//插入字条
	void SetData(int SQL_Type, int ID, string CardName, int Cost,int Health, int Attack, string CardDescribe, int Quality, string GraphPath1, string GraphPath2, int Spell0 = 0, int Spell1 = 0, int Spell2 = 0, int Spell3 = 0);
	//删除字条
	void DeleteData(int SQL_Type, int ID);

	void SetData();
	
	//获得字条数据
	//获得卡牌的数据（单个）
	char*			getCardData(int ID,int Type);
	
	vector<char*>	selectCardData(int Type, string number);
	




	
	//卡牌
	///随从
	void GetData(int SQL_Type, int ID, string CardName, int Cost, int Health, int Attack, string CardDescribe, int Quality, int Spell0, int Spell1, int Spell2, int Spell3, string GraphPath1, string GraphPath2);
	///法术
	void GetData(int SQL_Type, int ID, string CardName, int Cost, string CardDescribe, int Quality, int Effect1, int Effect2, int Effect3, string GraphPath1, string GraphPath2);
	///装备
	void GetData(int SQL_Type, int ID, string CardName, int Health, int Attack, int Cost, string CardDescribe, int Quality, int Effect1, int Effect2, int Effect3, string GraphPath1, string GraphPath2);


	//英雄
	void GetData(int SQL_Type, int ID, string CharacterNmae, int SkillID1, int SkillID2, int SkillID3, string CharacterDescribe, string GraphPath);
	//技能
	void GetData(int SQL_Type, int ID, string SkillName, int Cost, string SkillDiscrible, string GraphPath);

	void GetData(int ID);



private:
	std::string Sql_group[7];	//七个数据库的路径
	sqlite3 * sql[7];			//表示响应的七个数据库,声明sqlite关键结构指针

	
	//用于数据交互中的传递

	int DataInt[7];
	string DataStr[7];
};