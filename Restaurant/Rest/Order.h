#ifndef __ORDER_H_
#define __ORDER_H_
#include<iostream>
using namespace std;
#include "..\Defs.h"

class Order
{

protected:
	int ID;         //Each order has a unique ID (from 1 --> 999 )
	ORD_TYPE type;		//order type: Normal, vegan, VIP
	ORD_STATUS status;	//waiting, in-service, done
	int Distance;	//The distance (in meters) between the order location and the resturant 
	                
	double totalMoney;	//Total order money

	int ArrTime, ServTime, ServDuration, FinishTime = 0;	//arrival, service start, and finish times
	int size;
	int Limit;
	//
	// TODO: Add More Data Members As Needed
	//

public:
	Order();
	Order(int ID, ORD_TYPE r_Type);
	Order(int ID);
	virtual ~Order();

	int GetID()const;

	ORD_TYPE GetType() const;

	void SetDistance(int d);
	int GetDistance() const;

	void setType(ORD_TYPE o);
	ORD_TYPE getType();

	void setStatus(ORD_STATUS s);
	ORD_STATUS getStatus() const;

	double getPriority()const;
	friend ostream& operator << (ostream& out, const Order& O);

	void setMoney(double m);
	double getMoney() const;

	void setSize(int s);
	int getSize() const;

	void setArrTime(int T);
	int getArrTime()const;

	void setLimit(int i);
	int getLimit();

	void setServDur(int t);
	int getServDur();

	void setServTime(int t);
	int getServTime();
	//
	// TODO: Add More Member Functions As Needed
	//
};

#endif