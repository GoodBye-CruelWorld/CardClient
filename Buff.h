#pragma once
class Buff
{
public:
	Buff(int,int);
	void setResource(int);
	void setdata(int, int, int);
	~Buff();

	int _bufftype;// buff ����
	int _times;// ʣ��ʱ��
	int _buffdata[3];// ����ֵ��¼
	int _resource;// ��Դ
	int _buffid;
private:

};
