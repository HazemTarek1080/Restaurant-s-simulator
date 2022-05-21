//#include "Drawing.h"
#include "Rest\Restaurant.h"
#include "GUI\GUI.h"
#include "Rest/CooksList.h"
#include"Rest/Order.h"
#include"Events/Event.h"
int main()
{
	
	CooksList c1;
	c1.PrintList();
	Order o1(21, TYPE_VIP);
	Queue<Event*> e1;
	Restaurant* pRest = new Restaurant();
	pRest->RunSimulation();
	delete pRest;
	
	





	
	return 0;
}
