#pragma once
#include "..\Defs.h"
#include"Order.h"
#include<iostream>
using namespace std;

struct cooks_info
{
	int BN, BG, BV, SN, SG, SV, BO;
};
class Cook
{
	int ID;
	ORD_TYPE type;	//for each order type there is a corresponding type (VIP, Normal, Vegan)
	int speed;		//dishes it can prepare in one clock tick (in one timestep)
	int BD;
	bool aval;
	int BO;
	int BStartime;
	bool inbreak = false;
	int orders_cooked = 0;
	Order Cooking_Order;
	cooks_info info;
	int Cooking_Time;

public:
	Cook();
	//virtual ~Cook();
	int GetID() const;

	ORD_TYPE GetType() const;

	void setID(int);

	void setType(ORD_TYPE);

	int getBD() const;

	int getSpeed()const;

	void set_aval(bool t);

	bool check_aval();

	//void readcookno(int bn, int bg, int bv, int sn, int sg, int sv);
	void readcookinfo(cooks_info infox)
		;
	friend ostream& operator << (ostream& out, const Cook& C);

	Order getorder() 
	{
		return Cooking_Order;
	}

	void stepCookingTime();

	int getCookingTime();

	bool setOrder(Order O);

	void setBO(int n) 
	{
		BO = n;

	}

	void setBS(int n) 
	{
		BStartime = n;
	}
	
	int getBS() 
	{
		return BStartime;
	}
	int getBO() 
	{
		return BO;
	}
	bool Breaktime() 
	{
		if (orders_cooked == BO) {
			orders_cooked = 0;
			inbreak = true;
			return true;
		}
		return false;
	}
	
	bool getinbreak() 
	{
		return inbreak;
	} 
	
	void setinbreak(bool i) 
	{
		inbreak = i;
	}
 
};
