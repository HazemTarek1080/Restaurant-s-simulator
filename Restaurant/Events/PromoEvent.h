#pragma once
#include "Event.h"
#include"..\Rest\Restaurant.h"


//class for the arrival event
class PromoEvent : public Event
{
	int oID;
	double extraMoney;
	int PromoTS;
public:
	PromoEvent(int TS, int o_ID, double extra_Money);
	//Add more constructors if needed

	virtual void Execute(Restaurant* pRest);	//override execute function

};
