#pragma once
#include "Event.h"


class CancelEvent : public Event
{
	int cancel_order_ID;
	int cancel_event_Timestep;

public:
	CancelEvent(int Cancel_Timestep, int Cancel_ID);

	virtual void Execute(Restaurant* pRest);

};