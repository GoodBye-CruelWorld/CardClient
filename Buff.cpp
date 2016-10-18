#include"Buff.h"
Buff::Buff(int type,int id)
{
	_bufftype = type;
	_buffid = id;
	setdata(0, 0, 0);
	setResource(-1);

}

void Buff::setdata(int data1, int data2, int data3){
	_buffdata[0] = data1;
	_buffdata[1] = data2;
	_buffdata[2] = data3;
}

void Buff::setResource(int resource){
	_resource = resource;
}

Buff::~Buff()
{
}