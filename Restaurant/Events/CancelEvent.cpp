#include "CancelEvent.h"
#include "..\Rest\Restaurant.h"


CancelEvent::CancelEvent(int Cancel_Timestep, int Cancel_ID) : Event(Cancel_Timestep, Cancel_ID) {
	cancel_event_Timestep = Cancel_Timestep;
	cancel_order_ID = Cancel_ID;

}

void CancelEvent::Execute(Restaurant* pRest) {
	
	if (!pRest->getNormList()->isEmpty())
	{
		Order*p=pRest->getNormList()->MoveNormalOrder(cancel_order_ID);
		delete p;
		pRest->lessencountwait();
	}
}