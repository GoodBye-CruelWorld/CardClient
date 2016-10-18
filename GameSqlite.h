extern"C"
{
#include"sqlite3.h"
}
#include<iostream>
#include<vector>

using namespace std;

//���ݿ�����
#define SQL_CARD			0
#define SQL_CHARACTER		1
#define SQL_SKILL			2
#define SQL_ASSIGNMENT		3
#define SQL_ACHIEVEMENT		4
#define SQL_DIALOGUE		5
#define SQL_ACCOUNT			6


//�������ݿ�
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


//////////////////////////////////////////////////////////////////////////���ݿ���
class GameSqlite
{
public:
	GameSqlite();
	~GameSqlite();
	//�����ӦID�ֶεĻص�����
	static int loadRecord( void * para, int n_column, char ** column_value, char ** column_name);
	int Sql_Type;

	//��������
	void SetData(int SQL_Type, int ID, string CardName, int Cost,int Health, int Attack, string CardDescribe, int Quality, string GraphPath1, string GraphPath2, int Spell0 = 0, int Spell1 = 0, int Spell2 = 0, int Spell3 = 0);
	//ɾ������
	void DeleteData(int SQL_Type, int ID);

	void SetData();
	
	//�����������
	//��ÿ��Ƶ����ݣ�������
	char*			getCardData(int ID,int Type);
	
	vector<char*>	selectCardData(int Type, string number);
	




	
	//����
	///���
	void GetData(int SQL_Type, int ID, string CardName, int Cost, int Health, int Attack, string CardDescribe, int Quality, int Spell0, int Spell1, int Spell2, int Spell3, string GraphPath1, string GraphPath2);
	///����
	void GetData(int SQL_Type, int ID, string CardName, int Cost, string CardDescribe, int Quality, int Effect1, int Effect2, int Effect3, string GraphPath1, string GraphPath2);
	///װ��
	void GetData(int SQL_Type, int ID, string CardName, int Health, int Attack, int Cost, string CardDescribe, int Quality, int Effect1, int Effect2, int Effect3, string GraphPath1, string GraphPath2);


	//Ӣ��
	void GetData(int SQL_Type, int ID, string CharacterNmae, int SkillID1, int SkillID2, int SkillID3, string CharacterDescribe, string GraphPath);
	//����
	void GetData(int SQL_Type, int ID, string SkillName, int Cost, string SkillDiscrible, string GraphPath);

	void GetData(int ID);



private:
	std::string Sql_group[7];	//�߸����ݿ��·��
	sqlite3 * sql[7];			//��ʾ��Ӧ���߸����ݿ�,����sqlite�ؼ��ṹָ��

	
	//�������ݽ����еĴ���

	int DataInt[7];
	string DataStr[7];
};