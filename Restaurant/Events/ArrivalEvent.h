#ifndef __ARRIVAL_EVENT_H_
#define __ARRIVAL_EVENT_H_

#include "Event.h"


//class for the arrival event
class ArrivalEvent: public Event
{
	//info about the order ralted to arrival event
	int OrdDishes;	
	ORD_TYPE OrdType;		//order type: Normal, vegan, VIP	                
	double OrdMoney;	//Total order money
	int Limit;
public:
	ArrivalEvent(int eTime, int oID, ORD_TYPE oType, int size, double money, int limit = 0);
	//Add more constructors if needed

	virtual void Execute(Restaurant* pRest);	//override execute function

};

#endif