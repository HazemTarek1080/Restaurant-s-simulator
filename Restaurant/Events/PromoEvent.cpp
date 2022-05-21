#include "PromoEvent.h"

PromoEvent::PromoEvent(int TS, int o_ID, double extra_Money) :Event(TS, o_ID)
{
	PromoTS = TS;
	oID = o_ID;
	extraMoney = extra_Money;
}

void PromoEvent::Execute(Restaurant* pRest)
{
	if (!pRest->getNormList()->isEmpty())
	{
		Order* ToVIp = pRest->getNormList()->MoveNormalOrder(oID);
	//Order* ToVIp = p;
		
		if (ToVIp != nullptr)
		{
		//	ToVIp->setType(TYPE_VIP);
			/*
			ToVIp->setMoney(ToVIp->getMoney() + extraMoney);
			int Dishes = ToVIp->getSize();
			double totalMoney = ToVIp->getMoney();
			int ArrTime = ToVIp->getArrTime();
			double priority = (totalMoney / Dishes) + (100 / ArrTime);
			*/
			pRest->getVIPList()->Add(*ToVIp);
			//pRest->setcountp();
		}

	}


}