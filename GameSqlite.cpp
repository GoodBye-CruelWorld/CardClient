#include "GameSqlite.h"


GameSqlite::GameSqlite()
{
	Sql_group[0] = "D://xx//cocos//Dcg_database_Card.db";
	Sql_group[1] = "D://xx//cocos//Dcg_database_Character.db";
	Sql_group[2] = "D://xx//cocos//Dcg_database_Skill.db";
	Sql_group[3] = "";
	Sql_group[4] = "";
	Sql_group[5] = "";
	Sql_group[6] = "";
	
	
	//sqlite3_open("D://xx//cocos//Dcg_database_Card", &sql[0]);
	sqlite3_open("sql/Dcg_database_Card", &sql[0]);
	sqlite3_open("sql/Dcg_database_Character", &sql[1]);
	sqlite3_open("sql/Dcg_database_Skill", &sql[2]);
	SetData();
}


GameSqlite::~GameSqlite()
{}

//�ӿں���

char*	GameSqlite::getCardData(int ID, int Type)
{
	int i = WITCHCRAFT;
	char **errmsg = NULL;
	//ID��ת��int->string
	char s[10];
	sprintf_s(s, "%d", ID);

	string sqlstr, sqlstr1 = s;
	//���SQLָ��
	sqlstr = "select * from sqlTable where ID=" + sqlstr1;

	Sql_Type = SQL_CARD;
	//sqlite3_exec(sql[0], sqlstr.c_str(), GameSqlite::loadRecord, NULL, errmsg);

	//�����ӦID���ֶΣ�������Column_Value��
	char** Column_Value = { '\0' };
	int count, r, c;
	sqlite3_get_table(sql[Sql_Type], sqlstr.c_str(), &Column_Value, &r, &c, NULL);

	//��Column_Value�е�ÿ��ֵ�ֱ������Ӧ�Ĳ�����
	return Column_Value[c+Type];

}


vector<char *>	GameSqlite::selectCardData(int Type, string number)
{
	string sqlstr;
	//���SQLָ��
	sqlstr = "select * from sqlTable where ID=" + number;
					
				

	switch (Type)
	{
	case	CARD_ID:
		sqlstr = "select * from sqlTable where ID=" + number;
		break;
	case	CARD_NAME:
		sqlstr = "select * from sqlTable where CardName=" + number;
		break;
	case	CARD_COST:
		sqlstr = "select * from sqlTable where Cost=" + number;
		break;
	case	CARD_HEALTH:
		sqlstr = "select * from sqlTable where Health=" + number;
		break;
	case	CARD_ATTACK:
		sqlstr = "select * from sqlTable where Attack=" + number;
		break;
	case	CARD_DESCRIBE:
		sqlstr = "select * from sqlTable where CardDescribe=" + number;
		break;
	case	CARD_QUALITY:
		sqlstr = "select * from sqlTable where Quality=" + number;
		break;
	case	CARD_SPELL_1:
		sqlstr = "select * from sqlTable where Spell1=" + number;
		break;
	case	CARD_SPELL_2:
		sqlstr = "select * from sqlTable where Spell2=" + number;
		break;
	case	CARD_SPELL_3:
		sqlstr = "select * from sqlTable where Spell3=" + number;
		break;
	case	CARD_SPELL_4:
		sqlstr = "select * from sqlTable where Spell4=" + number;
		break;
	
		default:
			break;
	}




	//�����ӦID���ֶΣ�������Column_Value��
	char** Column_Value = { '\0' };

	//c�ڴ��ֶε�������r�ڴ��������������
	int  r, c;
	Sql_Type = SQL_CARD;	
	
	//����������
	vector<char*> relust;
	
	
	sqlite3_get_table(sql[Sql_Type], sqlstr.c_str(), &Column_Value, &r, &c, NULL);


	for (int i = 1; i <= r; i++)
		relust.push_back(Column_Value[i*c]);
		
	cout << r << endl;
	
	return relust;
}




void GameSqlite::SetData()
{
	//char **errmsg = NULL;
	//sqlite3_exec(sql[0], "create table Table( ID integer ,CardName nvarchar(32), Cost integer, Health integer, Attack integer,CardDescribe nvarchar(32), Quality integer,Spell0 integer,Spell1 nvarchar(32),Spell2 integer,Spell3 integer,GraphPath1 nvarchar(32),GraphPath2 nvarchar(32))", NULL, NULL, errmsg);
	////����
	//sqlite3_exec(sql[0], "insert into Table(ID,CardName, Cost, Health, Attack, CardDescribe, Quality, Spell0,Spell1,Spell2,Spell3,GraphPath1,GraphPath2) values()", 0, 0, errmsg);
	//sqlite3_exec(sql[0], "insert into Table(ID,CardName, Cost, Health, Attack, CardDescribe, Quality, Spell0,Spell1,Spell2,Spell3,GraphPath1,GraphPath2) values()", 0, 0, errmsg);


	////����
	///************************************************************************************/

	char **errmsg = NULL;
	sqlite3_exec(sql[0], "create table sqlTable( ID integer ,CardName nvarchar(32), Cost integer, Health integer, Attack integer,CardDescribe nvarchar(32), Quality integer,Spell0 integer,Spell1 nvarchar(32),Spell2 integer,Spell3 integer,GraphPath1 nvarchar(32),GraphPath2 nvarchar(32))", NULL, NULL, errmsg);
	/*sqlite3_exec(sql[0], "select * from sqlTable ", GameSqlite::loadRecord, NULL, errmsg);*/
	//*************************************************************************************//
	
	//���
	
	///սʿ5801003
	//sqlite3_exec(sql[0], "insert into sqlTable(ID,CardName, Cost, Health, Attack, CardDescribe, Quality, Spell0,Spell1,Spell2,Spell3,GraphPath1,GraphPath2) values(0000000,'�ͽײ���',0,1,1,'����1',0,3703001,2,3,4,7,7)", 0, 0, errmsg);
	sqlite3_exec(sql[0], "insert into sqlTable(ID,CardName, Cost, Health, Attack, CardDescribe, Quality, Spell0,Spell1,Spell2,Spell3,GraphPath1,GraphPath2) values(0000000,'�ͽײ���',0,1,1,'����1',0,1600002,2,3,4,7,7)", 0, 0, errmsg);
	sqlite3_exec(sql[0], "insert into sqlTable(ID,CardName, Cost, Health, Attack, CardDescribe, Quality, Spell0,Spell1,Spell2,Spell3,GraphPath1,GraphPath2) values(0000001,'��ǹ�̴�',1,3,1,'���٣�ʹӢ�۵�װ������;�+1',0,5802002,2,3,4,7,7)", 0, 0, errmsg);
	sqlite3_exec(sql[0], "insert into sqlTable(ID,CardName, Cost, Health, Attack, CardDescribe, Quality, Spell0,Spell1,Spell2,Spell3,GraphPath1,GraphPath2) values(0000002,'װ�׶�����',3,3,3,'���٣�ʹһ����ӻ�û���+2',0,2601102,2,3,4,7,7)", 0, 0, errmsg);
	sqlite3_exec(sql[0], "insert into sqlTable(ID,CardName, Cost, Health, Attack, CardDescribe, Quality, Spell0,Spell1,Spell2,Spell3,GraphPath1,GraphPath2) values(0000003,'����սʿ',5,3,4,'���٣�ѣ��һ�����ֱ������¸��غϿ�ʼ',1,1602102,2,3,4,7,7)", 0, 0, errmsg);
	sqlite3_exec(sql[0], "insert into sqlTable(ID,CardName, Cost, Health, Attack, CardDescribe, Quality, Spell0,Spell1,Spell2,Spell3,GraphPath1,GraphPath2) values(0000004,'Ӣ��',6,5,7,'������6',1,6904001,2,3,4,7,7)", 0, 0, errmsg);
	sqlite3_exec(sql[0], "insert into sqlTable(ID,CardName, Cost, Health, Attack, CardDescribe, Quality, Spell0,Spell1,Spell2,Spell3,GraphPath1,GraphPath2) values(0000005,'������ʿ',8,7,7,'ÿ���غϽ�����û���+2',1,2603001,2,3,4,7,7)", 0, 0, errmsg);
	sqlite3_exec(sql[0], "insert into sqlTable(ID,CardName, Cost, Health, Attack, CardDescribe, Quality, Spell0,Spell1,Spell2,Spell3,GraphPath1,GraphPath2) values(0000006,'ǰ��ָ�ӹ�',2,3,3,'�ѷ���ɫÿ��һ�㻤��ֵ�����+1������',2,1604099,2,3,4,7,7)", 0, 0, errmsg);
	sqlite3_exec(sql[0], "insert into sqlTable(ID,CardName, Cost, Health, Attack, CardDescribe, Quality, Spell0,Spell1,Spell2,Spell3,GraphPath1,GraphPath2) values(0000007,'����',4,1,5,'�ѷ���ɫÿ��һ�㻤��ֵ�����+1������',2,5001002,2,3,4,7,7)", 0, 0, errmsg);
	sqlite3_exec(sql[0], "insert into sqlTable(ID,CardName, Cost, Health, Attack, CardDescribe, Quality, Spell0,Spell1,Spell2,Spell3,GraphPath1,GraphPath2) values(0000008,'ս��',11,10,10,'����4 ��ӻ��� ��������',3,4001002,90199,3,4,7,7)", 0, 0, errmsg);
	
	//������
	sqlite3_exec(sql[0], "insert into sqlTable(ID,CardName, Cost, Health, Attack, CardDescribe, Quality, Spell0,Spell1,Spell2,Spell3,GraphPath1,GraphPath2) values(0010000,'���ӥ',1,4,0,'�ѷ�����������ӹ�����1',0,1700002,2,3,4,7,7)", 0, 0, errmsg);
	sqlite3_exec(sql[0], "insert into sqlTable(ID,CardName, Cost, Health, Attack, CardDescribe, Quality, Spell0,Spell1,Spell2,Spell3,GraphPath1,GraphPath2) values(0010001,'��ӥ',2,1,3,'���',0,1709002,2,3,4,7,7)", 0, 0, errmsg);
	sqlite3_exec(sql[0], "insert into sqlTable(ID,CardName, Cost, Health, Attack, CardDescribe, Quality, Spell0,Spell1,Spell2,Spell3,GraphPath1,GraphPath2) values(0010002,'��ϰ����',0,1,2,'���٣����1���˺�',0,1707102,2,3,4,7,7)", 0, 0, errmsg);
	sqlite3_exec(sql[0], "insert into sqlTable(ID,CardName, Cost, Health, Attack, CardDescribe, Quality, Spell0,Spell1,Spell2,Spell3,GraphPath1,GraphPath2) values(0010003,'������',3,3,4,'���٣���һ����ɫ���2���˺�',1,1707102,2,3,4,7,7)", 0, 0, errmsg);
	sqlite3_exec(sql[0], "insert into sqlTable(ID,CardName, Cost, Health, Attack, CardDescribe, Quality, Spell0,Spell1,Spell2,Spell3,GraphPath1,GraphPath2) values(0010004,'������',4,4,5,'����������ѣЧ��',0,2702004,2,3,4,7,7)", 0, 0, errmsg);
	sqlite3_exec(sql[0], "insert into sqlTable(ID,CardName, Cost, Health, Attack, CardDescribe, Quality, Spell0,Spell1,Spell2,Spell3,GraphPath1,GraphPath2) values(0010005,'����ɯ',7,4,3,'���٣�ʯ��1�����',2,1711102,2,3,4,7,7)", 0, 0, errmsg);
	sqlite3_exec(sql[0], "insert into sqlTable(ID,CardName, Cost, Health, Attack, CardDescribe, Quality, Spell0,Spell1,Spell2,Spell3,GraphPath1,GraphPath2) values(0010006,'�Ȼ���С��',7,7,5,'�غϽ���ʱ�����1���з���ɫ���3���˺�',0,3703001,2,3,4,7,7)", 0, 0, errmsg);
	sqlite3_exec(sql[0], "insert into sqlTable(ID,CardName, Cost, Health, Attack, CardDescribe, Quality, Spell0,Spell1,Spell2,Spell3,GraphPath1,GraphPath2) values(0010007,'������',5,3,5,'һ�غϿ��Թ�������',2,1708002,2,3,4,7,7)", 0, 0, errmsg);
	sqlite3_exec(sql[0], "insert into sqlTable(ID,CardName, Cost, Health, Attack, CardDescribe, Quality, Spell0,Spell1,Spell2,Spell3,GraphPath1,GraphPath2) values(0010008,'ħ��',11,12,8,'���٣���ѣ�����������',3,1705002,2,3,4,7,7)", 0, 0, errmsg);
	sqlite3_exec(sql[0], "insert into sqlTable(ID,CardName, Cost, Health, Attack, CardDescribe, Quality, Spell0,Spell1,Spell2,Spell3,GraphPath1,GraphPath2) values(0010009,'��������',2,3,2,'���٣�ʹ�����ҵ���ӹ���+1',1,1706002,2,3,4,7,7)", 0, 0, errmsg);
	sqlite3_exec(sql[0], "insert into sqlTable(ID,CardName, Cost, Health, Attack, CardDescribe, Quality, Spell0,Spell1,Spell2,Spell3,GraphPath1,GraphPath2) values(0010010,'��Ӱ����',3,4,3,'���٣���ǰ�غ�ʹһ����ӹ�����3',1,3712102,2,3,4,7,7)", 0, 0, errmsg);

	//����																																									
	sqlite3_exec(sql[0], "insert into sqlTable(ID,CardName, Cost, Health, Attack, CardDescribe, Quality, Spell0,Spell1,Spell2,Spell3,GraphPath1,GraphPath2) values(0100000,'ð�ռ�',1,2,1,'���٣���1����',1,1,2,3,4,7,7)", 0, 0, errmsg);
	sqlite3_exec(sql[0], "insert into sqlTable(ID,CardName, Cost, Health, Attack, CardDescribe, Quality, Spell0,Spell1,Spell2,Spell3,GraphPath1,GraphPath2) values(0100001,'���̷�',2,2,5,'ÿ���غϽ�����û���+2',1,1,2,3,4,7,7)", 0, 0, errmsg);
	sqlite3_exec(sql[0], "insert into sqlTable(ID,CardName, Cost, Health, Attack, CardDescribe, Quality, Spell0,Spell1,Spell2,Spell3,GraphPath1,GraphPath2) values(0100002,'XX������',4,6,3,'����',2,1,2,3,4,7,7)", 0, 0, errmsg);
	sqlite3_exec(sql[0], "insert into sqlTable(ID,CardName, Cost, Health, Attack, CardDescribe, Quality, Spell0,Spell1,Spell2,Spell3,GraphPath1,GraphPath2) values(0100003,'��ɡ��Ů',3,4,2,'�����ɶ������������˺�',1,1,2,3,4,7,7)", 0, 0, errmsg);
	sqlite3_exec(sql[0], "insert into sqlTable(ID,CardName, Cost, Health, Attack, CardDescribe, Quality, Spell0,Spell1,Spell2,Spell3,GraphPath1,GraphPath2) values(0100004,'�о�ǿ��',3,3,3,'���',1,1,2,3,4,7,7)", 0, 0, errmsg);
	sqlite3_exec(sql[0], "insert into sqlTable(ID,CardName, Cost, Health, Attack, CardDescribe, Quality, Spell0,Spell1,Spell2,Spell3,GraphPath1,GraphPath2) values(0100005,'��ˮ������',0,1,2,'ÿ���غϽ�����û���+2',0,1,2,3,4,7,7)", 0, 0, errmsg);
	sqlite3_exec(sql[0], "insert into sqlTable(ID,CardName, Cost, Health, Attack, CardDescribe, Quality, Spell0,Spell1,Spell2,Spell3,GraphPath1,GraphPath2) values(0100006,'��βè',2,4,3,'�ѷ���ɫÿ��һ�㻤��ֵ�����+1������',0,1,2,3,4,7,7)", 0, 0, errmsg);
	sqlite3_exec(sql[0], "insert into sqlTable(ID,CardName, Cost, Health, Attack, CardDescribe, Quality, Spell0,Spell1,Spell2,Spell3,GraphPath1,GraphPath2) values(0100007,'����',1,3,2,'����4 ��ӻ��� ��������',0,1,2,3,4,7,7)", 0, 0, errmsg);
	sqlite3_exec(sql[0], "insert into sqlTable(ID,CardName, Cost, Health, Attack, CardDescribe, Quality, Spell0,Spell1,Spell2,Spell3,GraphPath1,GraphPath2) values(0100008,'���������',5,5,4,'���ԣ����������1~3���˺�����ӻ���',1,1,2,3,4,7,7)", 0, 0, errmsg);
		
	
	//����
	//սʿ
	sqlite3_exec(sql[0], "insert into sqlTable(ID,CardName, Cost, Health, Attack, CardDescribe, Quality, Spell0,Spell1,Spell2,Spell3,GraphPath1,GraphPath2) values(0001000,'�ط�ս��',0,0,0,'�ظ�һ�������������ֵ����ʹ���û���+1',0,620107,2,3,4,7,7)", 0, 0, errmsg);
	sqlite3_exec(sql[0], "insert into sqlTable(ID,CardName, Cost, Health, Attack, CardDescribe, Quality, Spell0,Spell1,Spell2,Spell3,GraphPath1,GraphPath2) values(0001001,'��������',0,0,0,'���غ���������ӣ��غϽ������·���ս������1������ֵ',2,621007,2,3,4,7,7)", 0, 0, errmsg);
	sqlite3_exec(sql[0], "insert into sqlTable(ID,CardName, Cost, Health, Attack, CardDescribe, Quality, Spell0,Spell1,Spell2,Spell3,GraphPath1,GraphPath2) values(0001002,'ս��׼��',0,0,0,'ÿ��һ���ѷ���ɫ�л��ף���һ����',2,622007,2,3,4,7,7)", 0, 0, errmsg);

	
	//����
	sqlite3_exec(sql[0], "insert into sqlTable(ID,CardName, Cost, Health, Attack, CardDescribe, Quality, Spell0,Spell1,Spell2,Spell3,GraphPath1,GraphPath2) values(0011000,'ʯ��֮��',0,0,0,'3�غ��޷�����',0,711107,2,3,4,7,7)", 0, 0, errmsg);
	sqlite3_exec(sql[0], "insert into sqlTable(ID,CardName, Cost, Health, Attack, CardDescribe, Quality, Spell0,Spell1,Spell2,Spell3,GraphPath1,GraphPath2) values(0011001,'�������',0,0,0,'����1��ָ�����',1,720107,2,3,4,7,7)", 0, 0, errmsg);
	sqlite3_exec(sql[0], "insert into sqlTable(ID,CardName, Cost, Health, Attack, CardDescribe, Quality, Spell0,Spell1,Spell2,Spell3,GraphPath1,GraphPath2) values(0011002,'����',0,0,0,'�Եз�����������2�˺�',2,721007,2,3,4,7,7)", 0, 0, errmsg);
	sqlite3_exec(sql[0], "insert into sqlTable(ID,CardName, Cost, Health, Attack, CardDescribe, Quality, Spell0,Spell1,Spell2,Spell3,GraphPath1,GraphPath2) values(0011003,'�����',0,0,0,'��ѣ',2,602107,2,3,4,7,7)", 0, 0, errmsg);
	sqlite3_exec(sql[0], "insert into sqlTable(ID,CardName, Cost, Health, Attack, CardDescribe, Quality, Spell0,Spell1,Spell2,Spell3,GraphPath1,GraphPath2) values(0011004,'�������',0,0,0,'��Ŀ�����6�˺�',1,722107,2,3,4,7,7)", 0, 0, errmsg);
	sqlite3_exec(sql[0], "insert into sqlTable(ID,CardName, Cost, Health, Attack, CardDescribe, Quality, Spell0,Spell1,Spell2,Spell3,GraphPath1,GraphPath2) values(0011005,'��������',0,0,0,'�ѷ�������ӻָ�3����ֵ',2,723007,2,3,4,7,7)", 0, 0, errmsg);

	





}
/***********************************************************************************/


///////////////////////////////////////////////////////////////////////////////////////////
////////////
//�������



/////////����
/********************************************************************************************/
//���
void GameSqlite::GetData(int SQL_Type, int ID, string CardName, int Cost, int Health, int Attack, string CardDescribe, int Quality, int Spell0, int Spell1, int Spell2, int Spell3, string GraphPath1, string GraphPath2)
{


	char **errmsg = NULL;
	//ID��ת��int->string
	char s[10];
	sprintf_s(s, "%d", ID);

	string sqlstr, sqlstr1 = s;
	//���SQLָ��
	sqlstr = "select * from Table where ID=" + sqlstr1;
	
	Sql_Type = SQL_Type;
	//sqlite3_exec(sql[SQL_Type], sqlstr.c_str(), GameSqlite::loadRecord, NULL, errmsg);

	//�����ӦID���ֶΣ�������Column_Value��
	char** Column_Value = { '\0' };
	int count, r, c;
	sqlite3_get_table(sql[SQL_Type], sqlstr.c_str(), &Column_Value, &r, &c, NULL);

	//��Column_Value�е�ÿ��ֵ�ֱ������Ӧ�Ĳ�����
	ID = atoi(Column_Value[c]);
	CardName = Column_Value[c + 1];
	Health = atoi(Column_Value[c + 2]);
	Attack = atoi(Column_Value[c + 3]);
	CardDescribe = Column_Value[c + 4];
	Quality = atoi(Column_Value[c + 5]);
	Spell0 = atoi(Column_Value[c + 6]);
	Spell1 = atoi(Column_Value[c + 7]);
	Spell2 = atoi(Column_Value[c + 8]);
	Spell3 = atoi(Column_Value[c + 9]);
	GraphPath1 = Column_Value[c + 10];
	GraphPath2 = Column_Value[c + 11];

}
///����
void GameSqlite::GetData(int SQL_Type, int ID, string CardName, int Cost, string CardDescribe, int Quality, int Effect1, int Effect2, int Effect3, string GraphPath1, string GraphPath2)
{
	char **errmsg = NULL;
	//ID��ת��int->string
	char s[10];
	sprintf_s(s, "%d", ID);

	string sqlstr, sqlstr1 = s;
	//���SQLָ��
	sqlstr = "select * from Table where ID=" + sqlstr1;

	Sql_Type = SQL_Type;
	//sqlite3_exec(sql[SQL_Type], sqlstr.c_str(), GameSqlite::loadRecord, NULL, errmsg);

	//�����ӦID���ֶΣ�������Column_Value��
	char** Column_Value = { '\0' };
	int count, r, c;
	sqlite3_get_table(sql[SQL_Type], sqlstr.c_str(), &Column_Value, &r, &c, NULL);

	//��Column_Value�е�ÿ��ֵ�ֱ������Ӧ�Ĳ�����
	ID = atoi(Column_Value[c]);
	CardName = Column_Value[c + 1];
	CardDescribe = Column_Value[c + 4];
	Quality = atoi(Column_Value[c + 5]);
	Effect1 = atoi(Column_Value[c + 6]);
	Effect2 = atoi(Column_Value[c + 7]);
	Effect3 = atoi(Column_Value[c + 8]);
	GraphPath1 = Column_Value[c + 10];
	GraphPath2 = Column_Value[c + 11];





}
///װ��
void GameSqlite::GetData(int SQL_Type, int ID, string CardName, int Health, int Attack, int Cost, string CardDescribe, int Quality, int Effect1, int Effect2, int Effect3, string GraphPath1, string GraphPath2)
{
	char **errmsg = NULL;
	//ID��ת��int->string
	char s[10];
	sprintf_s(s, "%d", ID);

	string sqlstr, sqlstr1 = s;
	//���SQLָ��
	sqlstr = "select * from Table where ID=" + sqlstr1;

	Sql_Type = SQL_Type;
	//sqlite3_exec(sql[SQL_Type], sqlstr.c_str(), GameSqlite::loadRecord, NULL, errmsg);

	//�����ӦID���ֶΣ�������Column_Value��
	char** Column_Value = { '\0' };
	int count, r, c;
	sqlite3_get_table(sql[SQL_Type], sqlstr.c_str(), &Column_Value, &r, &c, NULL);

	//��Column_Value�е�ÿ��ֵ�ֱ������Ӧ�Ĳ�����
	ID = atoi(Column_Value[c]);
	CardName = Column_Value[c + 1];
	Health = atoi(Column_Value[c + 2]);
	Attack = atoi(Column_Value[c + 3]);
	CardDescribe = Column_Value[c + 4];
	Quality = atoi(Column_Value[c + 5]);
	Effect1 = atoi(Column_Value[c + 6]);
	Effect2 = atoi(Column_Value[c + 7]);
	Effect3 = atoi(Column_Value[c + 8]);

	GraphPath1 = Column_Value[c + 10];
	GraphPath2 = Column_Value[c + 11];
}
/****************************************************************************************/

//////////////////
////Ӣ��
void GameSqlite::GetData(int SQL_Type, int ID, string CharacterNmae, int SkillID1, int SkillID2, int SkillID3, string CharacterDescribe, string GraphPath)
{

	char **errmsg = NULL;
	//ID��ת��int->string
	char s[10];
	sprintf_s(s, "%d", ID);

	string sqlstr, sqlstr1 = s;
	//���SQLָ��
	sqlstr = "select * from Table where ID=" + sqlstr1;

	Sql_Type = SQL_Type;
	//sqlite3_exec(sql[SQL_Type], sqlstr.c_str(), GameSqlite::loadRecord, NULL, errmsg);

	//�����ӦID���ֶΣ�������Column_Value��
	char** Column_Value = { '\0' };
	int count, r, c;
	sqlite3_get_table(sql[SQL_Type], sqlstr.c_str(), &Column_Value, &r, &c, NULL);

	//��Column_Value�е�ÿ��ֵ�ֱ������Ӧ�Ĳ�����
	ID = atoi(Column_Value[c]);
	CharacterNmae = Column_Value[c + 1];
	SkillID1 = atoi(Column_Value[c + 2]);
	SkillID2 = atoi(Column_Value[c + 3]);
	SkillID3 = atoi(Column_Value[c + 4]);
	CharacterDescribe = Column_Value[c + 5];
	GraphPath = Column_Value[c + 6];
}

/////////////////
/////����
void GameSqlite::GetData(int SQL_Type, int ID, string SkillName, int Cost, string SkillDiscrible, string GraphPath)
{

	char **errmsg = NULL;
	//ID��ת��int->string
	char s[10];
	sprintf_s(s, "%d", ID);
	
	string sqlstr, sqlstr1 = s;
	//���SQLָ��
	sqlstr = "select * from Table where ID=" + sqlstr1;

	Sql_Type = SQL_Type;
	
	//sqlite3_exec(sql[SQL_Type], sqlstr.c_str(), GameSqlite::loadRecord, NULL, errmsg);

	//�����ӦID���ֶΣ�������Column_Value��
	char** Column_Value = { '\0' };
	int count, r, c;
	sqlite3_get_table(sql[SQL_Type], sqlstr.c_str(), &Column_Value, &r, &c, NULL);

	//��Column_Value�е�ÿ��ֵ�ֱ������Ӧ�Ĳ�����
	ID = atoi(Column_Value[c]);
	SkillName = Column_Value[c + 1];
	Cost = atoi(Column_Value[c + 2]);
	SkillDiscrible = Column_Value[c + 3];
	GraphPath = Column_Value[c + 4];



}

void GameSqlite::GetData(int ID)
{
	sqlite3 * db = NULL;
	int result;
	char **errmsg = NULL;
	char s[10];
	sprintf_s(s, "%d", ID);
	
	string sqlstr, sqlstr1 = s;

	sqlstr = "select * from MyTable_1 where ID="+sqlstr1;
	

	result = sqlite3_open("f://Dcg_database5.db", &db);
	char** Column_Value = { '\0' };
	int count,r,c;
	//sqlite3_exec(db, sqlstr.c_str(), GameSqlite::loadRecord,NULL, errmsg);
	//printf("ID=%s,name=%s,cost=%s", Column_Value[0], Column_Value[1], Column_Value[2]);
	
	sqlite3_get_table(db, sqlstr.c_str(), &Column_Value, &r, &c, NULL);
	//printf("ID=%s", Column_Value[c]);
	 //cout << Column_Value[r+1];
	//cout << count;
}


/***************************************************************************************************************************/

//////////////////////////////////////////�ص�����
//�����ӦID�ֶεĻص�����
int GameSqlite::loadRecord(void * para, int n_column, char ** column_value, char ** column_name)
{
	//printf("ID=%s,name=%s,cost=%s\n", column_value[0], column_value[1], column_value[2]);
	
	//char **Column_Value = (char **)para;
	//*Column_Value = column_value[1];
	//int *count = (int*)para;
	//*count = n_column;



	/**/int i;
	printf("��¼���� %d ���ֶ� \n", n_column);
	for (i = 0; i < n_column; i++)
	{
		printf("�ֶ���:%s ; �ֶ�ֵ:%s \n", column_name[i], column_value[i]);
	}
	printf("------------------\n");
	return 0;

}