#pragma once
class Buff
{
public:
	Buff(int,int);
	void setResource(int);
	void setdata(int, int, int);
	~Buff();

	int _bufftype;// buff 类型
	int _times;// 剩余时间
	int _buffdata[3];// 简单数值记录
	int _resource;// 来源
	int _buffid;
private:

};
