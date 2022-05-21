#include "ArrivalEvent.h"
#include "..\Rest\Restaurant.h"
ArrivalEvent::ArrivalEvent(int eTime, int oID, ORD_TYPE oType, int size, double money, int limit) :Event(eTime, oID)
{
	OrdType = oType;
	OrderID = oID;
	OrdMoney = money;
	OrdDishes = size;
	Limit = limit;
}

void ArrivalEvent::Execute(Restaurant* pRest)
{

	//This function should create an order and fills its data 
	Order* current_order = new Order(OrderID, OrdType);
	current_order->setSize(OrdDishes);
	current_order->setMoney(OrdMoney);
	current_order->setArrTime(EventTime);
	pRest->increasecountwait();
	switch (current_order->GetType())
	{
	case TYPE_NRM:
		current_order->setLimit(Limit);
		pRest->getNormList()->AddToList(*current_order);
		break;
	case TYPE_VGAN:
		pRest->getVeganList()->enqueue(*current_order);
		break;
	case TYPE_VIP:
		pRest->getVIPList()->Add(*current_order);
		break;
	default:
		break;
	}
}
