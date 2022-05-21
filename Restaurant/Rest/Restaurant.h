#ifndef __RESTAURANT_H_
#define __RESTAURANT_H_
#include<string>
#include "CooksList.h"
#include "..\Defs.h"
#include "..\CMUgraphicsLib\CMUgraphics.h"
#include "..\GUI\GUI.h"
#include "..\Generic_DS\Queue.h"
#include "..\Events\Event.h"
#include "Order.h"
#include"VIP_PQ.h"
#include"NormalOrdersList.h"
#include"../Events/CancelEvent.h"
#include"../Events/ArrivalEvent.h"
#include<fstream>
#include"../Events/PromoEvent.h"
#include"../Generic_DS/InServiceList.h"
// it is the maestro of the project
class Restaurant  
{	
private:
	GUI *pGUI;
	Queue<Event*>* EventsQueue = new Queue<Event*>;	//Queue of all events that will be loaded from file.
	CooksList* Cok = new CooksList;
	int countwaitorder = 0;
	int countnrmorders = 0;
	int countviporders = 0;
	int countvgnorders = 0;
	int countnrmcooks = 0;
	int countvipcooks = 0;
	int countvgncooks = 0;
	int countpromote = 0;
	double sumwaittime = 0;
	double sumservtime = 0;
	Queue<Order>* VeganQ =new Queue<Order>;
	VIP_PQ* VIPQ = new VIP_PQ;
	NormalOrdersList* NRMQ = new NormalOrdersList;
	Queue<Order>* SRVQ = new Queue<Order>;
	Queue<Order>* DoneQ= new Queue<Order>;
	Queue<Order>* FileQ = new Queue<Order>;
	InServiceList* SRVList = new InServiceList;
	
	

	/// ==> 
	//	DEMO-related members. Should be removed in phases 1&2
		//Important: This is just for demo
	/// ==>

	
	
	//
	// TODO: Add More Data Members As Needed
	//

public:
	
	Restaurant();
	~Restaurant();
	
	void ExecuteEvents(int TimeStep);	//executes all events at current timestep
	void RunSimulation();
	
	void Load(string filename);
	//void intermode(GUI* pGUI,CooksList* Cs,Order* o);

	void FillDrawingList();

	//
	// TODO: Add More Member Functions As Needed
	//

	NormalOrdersList* getNormList();
	VIP_PQ* getVIPList();
	Queue<Order>* getVeganList();
/// ===================    DEMO-related functions. Should be removed in phases 1&2   ================= 

	void simplesim();	//just to show a demo and should be removed in phase1 1 & 2
	void AddtoDemoQueue(Order* po);	//adds an order to the demo queue
	void lessencountwait() 
	{
		countwaitorder--;
	}
	void increasecountwait() 
	{
		countwaitorder++;
	}
/// ================================================================================================== 
	void AssignCooks(int t);
	void CookStep(int t);

	void intermode();
	void stepmode();
	void silentmode();
	void setcountp();

	void save(string filename);
	

};

#endif