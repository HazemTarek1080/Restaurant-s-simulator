#include "Cook.h"
Cook::Cook()
{
	
	Cooking_Time = 0;
}
//Cook::~Cook()
//{
//}
int Cook::GetID() const
{
	return ID;
}
ORD_TYPE Cook::GetType() const
{
	return type;
}
void Cook::setID(int id)
{
this->	ID = id;
}
void Cook::setType(ORD_TYPE t)
{
	type = t;
	switch (t)
	{
	case(TYPE_VIP): BD = info.BV; speed = info.SV; break;
	case(TYPE_NRM): BD = info.BN; speed = info.SN; break;
	case(TYPE_VGAN): BD = info.BG; speed = info.SG; break;
	}
}
bool Cook::check_aval()
{
	//if (Cooking_Order) return false;
	//else 
		return aval;
}
void Cook::set_aval(bool t)
{
	aval = t;
}

int Cook:: getBD() const
{
	return BD;
}
int Cook::getSpeed()const
{
	return speed;
}
/*
void Cook::readcookno(int bn, int bg, int bv, int sn, int sg, int sv)
{
	BN = bn; BG = bg; BV = bv; SN = sn; SG = sg; SV = sv;
}
*/
void Cook::readcookinfo(cooks_info infox)
{
	info = infox;
}
bool Cook::setOrder(Order O)
{
		Cooking_Order = O;
		Cooking_Time = speed;
		orders_cooked++;
		aval = false;
		return true;
	
}
void Cook::stepCookingTime()
{
	Cooking_Time--;
}
int Cook::getCookingTime()
{
	return Cooking_Time;
}
ostream& operator<<(ostream& out, const Cook& C)
{
	out << C.GetType() << " " << C.getSpeed() << " " << C.getBD();
	return out;
}


