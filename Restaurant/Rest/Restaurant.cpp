#include <cstdlib>
#include <time.h>
#include <iostream>
#include<fstream>
#include"CooksList.h"
#include"string"
using namespace std;

#include "Restaurant.h"
#include "..\Events\ArrivalEvent.h"
#include"..\Events\CancelEvent.h"


Restaurant::Restaurant() 
{
	pGUI = NULL;
}
void Restaurant::AssignCooks(int currtime)
{
	Order x;
	while (VIPQ->peekFront(x))
	{
		Node<Cook>* c = Cok->getHead();
		Cook c1;
		while (c)
		{
			if (c->getItem().check_aval())
			{
				VIPQ->dequeue(x);
				c1 = c->getItem();
				x.setServDur(c1.getSpeed());
				x.setServTime(currtime);
				x.setStatus(SRV);
				countwaitorder--;
				countviporders++;
				SRVList->AddToList(x);
				c1.setOrder(x);
				c->setItem(c1);
				//SRVQ->enqueue(x);
				break;
			}
			c = c->getNext();
		}
		if (c == nullptr)
		{
			break;
		}
	}
	while (VeganQ->peekFront(x))
	{
		Node<Cook>* c = Cok->getHead();
		Cook c1;
		while (c)
		{
			if (c->getItem().GetType() == TYPE_VGAN && c->getItem().check_aval())
			{
				VeganQ->dequeue(x);
				c1 = c->getItem();
				x.setServDur(c1.getSpeed());
				x.setServTime(currtime);
				x.setStatus(SRV);
				countwaitorder--;
				countvgnorders++;
				SRVList->AddToList(x);
				c1.setOrder(x);
				c->setItem(c1);
				//SRVQ->enqueue(x);
				break;
			}
			c = c->getNext();
		}
		if (c == nullptr)
		{
			break;
		}
	}

	Node<Order>* Or = NRMQ->getHead();
	Node<Order>* Orr;
	while (Or) {
		Orr = Or->getNext();
		if (Or->getItem().getArrTime() + Or->getItem().getLimit() == currtime) {
			Order* O = NRMQ->MoveNormalOrder(Or->getItem().GetID());
			O->setType(TYPE_VIP);
			VIPQ->enqueue(*O);
			countpromote++;
		}
		Or = Orr;
	}
	while (NRMQ->getHead())
	{
		Node<Cook>* c = Cok->getHead();
		Cook c1;

		while (c)
		{

			if (c->getItem().GetType() == TYPE_NRM &&c->getItem().check_aval())
			{
				Node<Order>* p = NRMQ->getHead();
				Order O = p->getItem();
				c1 = c->getItem(); 
				O.setServDur(c1.getSpeed());
				O.setServTime(currtime);
				O.setStatus(SRV);
				countwaitorder--;
				countnrmorders++;
				SRVList->AddToList(O);
				c1.setOrder(O);
				c->setItem(c1);
				//SRVQ->enqueue(O);
				NRMQ->RemoveFirst();
				break;
			}
			c = c->getNext();
		}
		if (c == nullptr)
		{
			break;
		}
	}
	while (NRMQ->getHead())
	{
		Node<Cook>* c = Cok->getHead();
		Cook c1;
		while (c)
		{
			if (c->getItem().GetType() == TYPE_VIP && c->getItem().check_aval())
			{
				Node<Order>* p = NRMQ->getHead();
				Order O = p->getItem();
				c1 = c->getItem();
				O.setServDur(c1.getSpeed());
				O.setServTime(currtime);
				O.setStatus(SRV);
				countwaitorder--;
				countnrmorders++;
				SRVList->AddToList(O);
				c1.setOrder(O);
				c->setItem(c1);
			//	SRVQ->enqueue(O);
				NRMQ->RemoveFirst();
				break;
			}
			c = c->getNext();
		}
		if (c == nullptr)
		{
			break;
		}
	}

}
void Restaurant::CookStep(int currtime)
{
	Node<Cook>* c = Cok->getHead();
	Cook c1;
	while (c)
	{
		if (c->getItem().check_aval() == false)
		{
			c1 = c->getItem();
			if(!c1.getinbreak()) c1.stepCookingTime();
			if (c1.getCookingTime() == 0)
			{
				//c1.setOrder(NULL);

				if (c1.Breaktime()) {
					c1.setBS(currtime);
				}
				else if (c1.getinbreak()){
					if ((c1.getBS() + c1.getBD()) == currtime) 
					{
						c1.setinbreak(false);
						c1.set_aval(true);
					}
				}
				else {
					c1.set_aval(true);
				}
			}
			c->setItem(c1);
		}
		c = c->getNext();
	}
	Order x;
	Node<Order>* p = SRVList->getHead();
	
	while (p!=nullptr)
	{
		x = p->getItem();
		int key = x.getArrTime() + (x.getServTime() - x.getArrTime()) + x.getServDur();
		if (key == currtime)
		{
			if (SRVList->DeleteFromList(p))
			{
				sumservtime = sumservtime + (currtime- x.getServTime());
				sumwaittime = sumwaittime + (x.getServTime() - x.getArrTime());
				x.setStatus(DONE);
				DoneQ->enqueue(x);
			}
		}
		p = p->getNext();
	}
	
}

void Restaurant::RunSimulation()
{

	pGUI = new GUI;
	PROG_MODE mode = pGUI->getGUIMode();
	switch (mode)	//Add a function for each mode in next phases
	{
	case MODE_INTR:
		intermode();
		break;
		
	case MODE_STEP:
		stepmode();
		break;
		
	case MODE_SLNT:
		silentmode();
		break;
		
	case MODE_DEMO:
		simplesim();
		break;

	};

}



//////////////////////////////////  Event handling functions   /////////////////////////////

//Executes ALL events that should take place at current timestep
void Restaurant::ExecuteEvents(int CurrentTimeStep)
{
	Event *pE;
	while( EventsQueue->peekFront(pE) )	//as long as there are more events
	{
		if(pE->getEventTime() > CurrentTimeStep )	//no more events at current timestep
			return;

		pE->Execute(this);
		EventsQueue->dequeue(pE);	
		
		//remove event from the queue
		//deallocate event object from memory
	}
}


Restaurant::~Restaurant()
{
		if (pGUI)
			delete pGUI;
}

/*void Restaurant::FillDrawingList()
{
	for (int i = 0; i < maxcooknum;i++){ 
		pGUI->AddToDrawingList(cooklist);
	//It should get orders from orders lists/queues/stacks/whatever (same for Cooks)
	//To add orders it should call function  void GUI::AddToDrawingList(Order* pOrd);
	//To add Cooks it should call function  void GUI::AddToDrawingList(Cook* pCc);

}



*/

void Restaurant::simplesim()

{	//Now We have filled EventsQueue (randomly)
	int CurrentTimeStep = 1;
	

	Load("test5");
	//as long as events queue is not empty yet
	Node<Cook>* cokr1 = Cok->getHead();
	while (cokr1) {

		if (cokr1->getItem().GetType() == TYPE_NRM)countnrmcooks++;
		else if (cokr1->getItem().GetType() == TYPE_VIP)countvipcooks++;
		else if (cokr1->getItem().GetType() == TYPE_VGAN)countvgncooks++;
		cokr1 = cokr1->getNext();
	}
	
	while (!EventsQueue->isEmpty() || SRVList->getHead()!=nullptr || !VIPQ->isEmpty() || !NRMQ->isEmpty() || !VeganQ->isEmpty() || !DoneQ->isEmpty())
	{
		//print current timestep
		char timestep[10];
		itoa(CurrentTimeStep, timestep, 10);



		//The next line may add new orders to the DEMO_Queue
		ExecuteEvents(CurrentTimeStep);	//execute all events at current time step


		
	 

		//Let's add ALL randomly generated Cooks to GUI::DrawingList
		
		Node<Cook>* cokr = Cok->getHead();
		int countcook = 0;
		while (cokr->getNext()) {
			

			
			if (cokr->getItem().check_aval()) {
				pGUI->AddToDrawingList(&cokr->getItem());
				countcook++;
			}
			cokr = cokr->getNext();
		}
		if (cokr->getItem().check_aval()) {
			countcook++;
			pGUI->AddToDrawingList(&cokr->getItem());

		}
		//Let's add ALL randomly generated Ordes to GUI::DrawingList
		
		Order x;

		VIP_PQ* tempvip= new VIP_PQ;
		Queue<Order>* tempvgn= new Queue<Order>;
		Queue<Order>* tempsrv= new Queue<Order>;
		Queue<Order>* tempdone= new Queue<Order>;
		Node<Order>* p = NRMQ->getHead();
		Node<Order>* r = SRVList->getHead();
		while (VIPQ->dequeue(x)) {
			pGUI->AddToDrawingList(&x);
			tempvip->enqueue(x);
			
		}
		while (tempvip->dequeue(x)) {
			VIPQ->Add(x);
		}
		while (p&&p->getNext()) {
			pGUI->AddToDrawingList(&p->getItem());
			p = p->getNext();
			
		}
		if (p) { 
			pGUI->AddToDrawingList(&p->getItem()); 
			
		}
		
		while (VeganQ->dequeue(x)) {
			pGUI->AddToDrawingList(&x);
			tempvgn->enqueue(x);
		}
		while (tempvgn->dequeue(x)) {
			VeganQ->enqueue(x);
		}
		while (r && r->getNext()) {
			pGUI->AddToDrawingList(&r->getItem());
			r = r->getNext();

		}
		if (r) {
			pGUI->AddToDrawingList(&r->getItem());

		}
		
		while (DoneQ->dequeue(x)) {
			pGUI->AddToDrawingList(&x);
			tempdone->enqueue(x);
		}
		while (tempdone->dequeue(x)) {
			DoneQ->enqueue(x);
		}
		
		pGUI->PrintStats(CurrentTimeStep, countcook, countwaitorder);
		Node<Cook>* cok2 = Cok->getHead();
		int countmsg = 0;
		while (cok2) {
			if (cok2->getItem().getorder().getServTime() == (CurrentTimeStep - 1)) {
				countmsg++;
				if (cok2->getItem().GetType() == TYPE_NRM) {
					if (cok2->getItem().getorder().GetType() == TYPE_NRM) {
						pGUI->PrintMessage("Normal Cook #" + to_string(cok2->getItem().GetID()) + " has been assigned to Normal order #" + to_string(cok2->getItem().getorder().GetID()), countmsg);
					}
					else if (cok2->getItem().getorder().GetType() == TYPE_VIP) {
						pGUI->PrintMessage("Normal Cook #" + to_string(cok2->getItem().GetID()) + " has been assigned to VIP order #" + to_string(cok2->getItem().getorder().GetID()), countmsg);
					}
					else if (cok2->getItem().getorder().GetType() == TYPE_VGAN) {
						pGUI->PrintMessage("Normal Cook #" + to_string(cok2->getItem().GetID()) + " has been assigned to vegan order #" + to_string(cok2->getItem().getorder().GetID()), countmsg);
					}

				}
				else if (cok2->getItem().GetType() == TYPE_VIP) {
					if (cok2->getItem().getorder().GetType() == TYPE_NRM) {
						pGUI->PrintMessage("VIP Cook #" + to_string(cok2->getItem().GetID()) + " has been assigned to Normal order #" + to_string(cok2->getItem().getorder().GetID()), countmsg);
					}
					else if (cok2->getItem().getorder().GetType() == TYPE_VIP) {
						pGUI->PrintMessage("VIP Cook #" + to_string(cok2->getItem().GetID()) + " has been assigned to VIP order #" + to_string(cok2->getItem().getorder().GetID()), countmsg);
					}
					else if (cok2->getItem().getorder().GetType() == TYPE_VGAN) {
						pGUI->PrintMessage("VIP Cook #" + to_string(cok2->getItem().GetID()) + " has been assigned to vegan order #" + to_string(cok2->getItem().getorder().GetID()), countmsg);
					}

				}
				else if (cok2->getItem().GetType() == TYPE_VGAN) {
					if (cok2->getItem().getorder().GetType() == TYPE_NRM) {
						pGUI->PrintMessage("Vegan Cook #" + to_string(cok2->getItem().GetID()) + " has been assigned to Normal order #" + to_string(cok2->getItem().getorder().GetID()), countmsg);
					}
					else if (cok2->getItem().getorder().GetType() == TYPE_VIP) {
						pGUI->PrintMessage("Vegan Cook #" + to_string(cok2->getItem().GetID()) + " has been assigned to VIP order #" + to_string(cok2->getItem().getorder().GetID()), countmsg);
					}
					else if (cok2->getItem().getorder().GetType() == TYPE_VGAN) {
						pGUI->PrintMessage("Vegan Cook #" + to_string(cok2->getItem().GetID()) + " has been assigned to vegan order #" + to_string(cok2->getItem().getorder().GetID()), countmsg);
					}

				}
			}
			cok2 = cok2->getNext();
		}
		if (EventsQueue->isEmpty() && SRVList->getHead() == nullptr && VIPQ->isEmpty() && NRMQ->isEmpty() && VeganQ->isEmpty()) {
			while (DoneQ->dequeue(x)) {
				FileQ->enqueue(x);
			}
		}
		
		CookStep(CurrentTimeStep);
		AssignCooks(CurrentTimeStep);
			/////////////////////////////////////////////////////////////////////////////////////////
		   
		pGUI->UpdateInterface();
			//Sleep(1000);
 			CurrentTimeStep++;	//advance timestep
			pGUI->ResetDrawingList();
		//pGUI->PrintMessage("generation done, click to END program");
			pGUI->waitForClick();

 	}
	////////////////


	/// ==> end of DEMO-related function
	//////////////////////////////////////////////////////////////////////////////////////////////

	
}
NormalOrdersList* Restaurant::getNormList() {
	return NRMQ;
}
VIP_PQ* Restaurant::getVIPList() {
	return VIPQ;
}
Queue<Order>* Restaurant::getVeganList() {
	return VeganQ;
}
void Restaurant::Load(string filename)
{


	string a, b, c, d, e, f;
	int count = 0;
	fstream file;
	string line;
	int N, G, V, SN, SG, SV, BO, BN, BG, BV, AutoP, M;


	int x = 0;
	int y = 0;
	int z = 0;     //spaces detectors
	int l = 0;
	int m = 0;


	int j = 0;



	file.open(filename + ".txt");


	if (!file)
	{
		cout << "file open failure ! \n\n";
		exit(EXIT_FAILURE);

	}

	while (!file.eof())
	{
		j++;

		getline(file, line);

		if (j == 1)
		{

			for (int i = 0; i < line.length(); i++)
			{
				if (line[i] == ' ')
				{
					count++;
				}
				if (count == 8)
				{
					x = i - 7;
					count = 0;
					break;
				}
			}

			for (int i = x + 8; i < line.length(); i++)
			{
				if (line[i] == ' ')
				{
					count++;
				}

				if (count == 8)
				{
					y = i - 7;
					count = 0;
					break;
				}
			}

			a = line.substr(0, x);
			b = line.substr(x + 8, (y - (x + 8)));
			c = line.substr(y + 8, line.length());

			N = stoi(a);
			G = stoi(b);
			V = stoi(c);




		}

		else if (j == 2)
		{

			for (int i = 0; i < line.length(); i++)
			{
				if (line[i] == ' ')
				{
					count++;
				}
				if (count == 8)
				{
					x = i - 7;
					count = 0;
					break;
				}
			}

			for (int i = x + 8; i < line.length(); i++)
			{
				if (line[i] == ' ')
				{
					count++;
				}

				if (count == 8)
				{
					y = i - 7;
					count = 0;
					break;
				}
			}

			a = line.substr(0, x);
			b = line.substr(x + 8, (y - (x + 8)));
			c = line.substr(y + 8, line.length());

			SN = stoi(a);
			SG = stoi(b);
			SV = stoi(c);




		}

		else if (j == 3)
		{
			for (int i = 0; i < line.length(); i++)
			{
				if (line[i] == ' ')
				{
					count++;
				}
				if (count == 8)
				{
					x = i - 7;
					count = 0;
					break;
				}
			}

			for (int i = x + 8; i < line.length(); i++)
			{
				if (line[i] == ' ')
				{
					count++;
				}

				if (count == 8)
				{
					y = i - 7;
					count = 0;
					break;
				}
			}

			for (int i = y + 8; i < line.length(); i++)
			{
				if (line[i] == ' ')
				{
					count++;
				}

				if (count == 8)
				{
					z = i - 7;
					count = 0;
					break;
				}
			}

			a = line.substr(0, x);
			b = line.substr(x + 8, y - (x + 8));
			c = line.substr(y + 8, z - (y + 8));
			d = line.substr(z + 8, line.length());


			BO = stoi(a);
			BN = stoi(b);
			BG = stoi(c);
			BV = stoi(d);


		}


		else if (j == 4)
		{
			string temp;
			file >> temp;
			AutoP = stoi(temp);



		}


		else if (j == 5)
		{
			string temp;
			file >> temp;
			M = stoi(temp);



		}




		if (j > 5 || j <= 13)
		{
			Event* pE;


			for (int i = 0; i < line.length(); i++)
			{
				if (line[i] == ' ')
				{
					count++;
				}
				if (count == 8)
				{
					x = i - 7;
					count = 0;
					break;
				}
			}

			a = line.substr(0, x);

			if (a == "R")
			{

				for (int i = x + 8; i < line.length(); i++)
				{
					if (line[i] == ' ')
					{
						count++;
					}

					if (count == 8)
					{
						y = i - 7;
						count = 0;
						break;
					}
				}

				for (int i = y + 8; i < line.length(); i++)
				{
					if (line[i] == ' ')
					{
						count++;
					}

					if (count == 8)
					{
						z = i - 7;
						count = 0;
						break;
					}
				}

				for (int i = z + 8; i < line.length(); i++)
				{
					if (line[i] == ' ')
					{
						count++;
					}

					if (count == 8)
					{
						l = i - 7;
						count = 0;
						break;
					}
				}

				for (int i = l + 8; i < line.length(); i++)
				{
					if (line[i] == ' ')
					{
						count++;
					}

					if (count == 8)
					{
						m = i - 7;
						count = 0;
						break;
					}
				}

				b = line.substr(x + 8, y - (x + 8));
				c = line.substr(y + 8, z - (y + 8));
				d = line.substr(z + 8, l - (z + 8));
				e = line.substr(l + 8, m - (l + 8));
				f = line.substr(m + 8, line.length());
				ORD_TYPE o;
				if (b == "n" || b == "N") {
					o = TYPE_NRM;
				}
				else if (b == "v" || b == "V") {
					o = TYPE_VIP;
				}
				else {
					o = TYPE_VGAN;
				}
				pE = new ArrivalEvent (stoi(c), stoi(d), o, stoi(e), stod(f), AutoP);

				EventsQueue->enqueue(pE);


			}

			if (a == "X")
			{


				for (int i = x + 8; i < line.length(); i++)
				{
					if (line[i] == ' ')
					{
						count++;
					}

					if (count == 8)
					{
						y = i - 7;
						count = 0;
						break;
					}
				}

				b = line.substr(x + 8, y - (x + 8));
				c = line.substr(y + 8, z - (y + 8));

				pE = new CancelEvent (stoi(b), stoi(c));

				EventsQueue->enqueue(pE);


			}

			if (a == "P")
			{



				for (int i = x + 8; i < line.length(); i++)
				{
					if (line[i] == ' ')
					{
						count++;
					}

					if (count == 8)
					{
						y = i - 7;
						count = 0;
						break;
					}
				}

				for (int i = y + 8; i < line.length(); i++)
				{
					if (line[i] == ' ')
					{
						count++;
					}

					if (count == 8)
					{
						z = i - 7;
						count = 0;
						break;
					}
				}

				b = line.substr(x + 8, y - (x + 8));
				c = line.substr(y + 8, z - (y + 8));
				d = line.substr(z + 8, l - (z + 8));

				pE= new PromoEvent (stoi(b), stoi(c), stod(d));

				EventsQueue->enqueue(pE);

			}

		}

	}


	file.close();
	//cooks_info in_info;
	Cok->buildList(V, N, G, SV, SN, SG, BV, BN, BG, BO);
	
//	return co;

}


void Restaurant::save(string filename)
{
	ofstream file;
	file.open(filename+".txt");
	Order order;


	if (!file)
	{
		cout << "file open failure!";

		
	}
	else {
		file << "FT\tID\tAT\tWT\tST\n";
		while (FileQ->dequeue(order))
		{

			file << order.getServDur() + order.getServTime() << "\t" << order.GetID() << "\t" << order.getArrTime() << "\t" << order.getServTime() - order.getArrTime() << "\t" << order.getServDur() << "\n";

		}

		file << "Orders: " << countvgnorders + countnrmorders + countviporders << "\t" << "[Norm:" << countnrmorders << ", Veg:" << countvgnorders << ", VIP:" << countviporders << "]\n" << "Cooks: " << countnrmcooks + countvipcooks + countvgncooks << "\t[Norm" << countnrmcooks << ", Veg:" << countvgncooks << ", VIP:" << countvipcooks << "]\n" << "Avg Wait = " << sumwaittime / (countvgnorders + countnrmorders + countviporders) << ", Avg Serv = " << sumservtime / (countvgnorders + countnrmorders + countviporders) << "\n" << "Auto-promoted: " << countpromote;








	}

		file.close();
	
}


void Restaurant::intermode() {
	
	int CurrentTimeStep = 1;
	

	Load("test6");
	Node<Cook>* cokr1 = Cok->getHead();
	while (cokr1) {

		if (cokr1->getItem().GetType() == TYPE_NRM)countnrmcooks++;
		else if (cokr1->getItem().GetType() == TYPE_VIP)countvipcooks++;
		else if (cokr1->getItem().GetType() == TYPE_VGAN)countvgncooks++;
		cokr1 = cokr1->getNext();
	}
	while (!EventsQueue->isEmpty() || SRVList->getHead() != nullptr || !VIPQ->isEmpty() || !NRMQ->isEmpty() || !VeganQ->isEmpty()||!DoneQ->isEmpty())
	{
		
		char timestep[10];
		itoa(CurrentTimeStep, timestep, 10);


		ExecuteEvents(CurrentTimeStep);
		Node<Cook>* cokr = Cok->getHead();
		int countcook = 0;
		while (cokr->getNext()) {
			

			
			if (cokr->getItem().check_aval()) {
				pGUI->AddToDrawingList(&cokr->getItem());
				countcook++;
			}
			cokr = cokr->getNext();
		}
		if (cokr->getItem().check_aval()) {
			countcook++;
			pGUI->AddToDrawingList(&cokr->getItem());

		}
		//Let's add ALL randomly generated Ordes to GUI::DrawingList

		Order x;

		VIP_PQ* tempvip = new VIP_PQ;
		Queue<Order>* tempvgn = new Queue<Order>;
		Queue<Order>* tempsrv = new Queue<Order>;
		Queue<Order>* tempdone = new Queue<Order>;
		Node<Order>* p = NRMQ->getHead();
		Node<Order>* r = SRVList->getHead();
		while (VIPQ->dequeue(x)) {
			pGUI->AddToDrawingList(&x);
			tempvip->enqueue(x);

		}
		while (tempvip->dequeue(x)) {
			VIPQ->Add(x);
		}
		while (p && p->getNext()) {
			pGUI->AddToDrawingList(&p->getItem());
			p = p->getNext();

		}
		if (p) {
			pGUI->AddToDrawingList(&p->getItem());

		}

		while (VeganQ->dequeue(x)) {
			pGUI->AddToDrawingList(&x);
			tempvgn->enqueue(x);
		}
		while (tempvgn->dequeue(x)) {
			VeganQ->enqueue(x);
		}
		while (r && r->getNext()) {
			pGUI->AddToDrawingList(&r->getItem());
			r = r->getNext();

		}
		if (r) {
			pGUI->AddToDrawingList(&r->getItem());

		}

		/*
		while (SRVQ->dequeue(x)) {
			pGUI->AddToDrawingList(&x);
			tempsrv->enqueue(x);
		}
		while (tempsrv->dequeue(x)) {
			SRVQ->enqueue(x);
		}
		*/
		while (DoneQ->dequeue(x)) {
			pGUI->AddToDrawingList(&x);
			tempdone->enqueue(x);
		}
		while (tempdone->dequeue(x)) {
			DoneQ->enqueue(x);
		}

		pGUI->PrintStats(CurrentTimeStep, countcook, countwaitorder);
		CookStep(CurrentTimeStep);
		AssignCooks(CurrentTimeStep);
		Node<Cook>* cok2 = Cok->getHead();
		int countmsg = 0;
		while (cok2) {
			if (cok2->getItem().getorder().getServTime() == (CurrentTimeStep - 1)) {
				countmsg++;
				if (cok2->getItem().GetType() == TYPE_NRM) {
					if (cok2->getItem().getorder().GetType() == TYPE_NRM) {
						pGUI->PrintMessage("Normal Cook #" + to_string(cok2->getItem().GetID()) + " has been assigned to Normal order #" + to_string(cok2->getItem().getorder().GetID()), countmsg);
					}
					else if (cok2->getItem().getorder().GetType() == TYPE_VIP) {
						pGUI->PrintMessage("Normal Cook #" + to_string(cok2->getItem().GetID()) + " has been assigned to VIP order #" + to_string(cok2->getItem().getorder().GetID()), countmsg);
					}
					else if (cok2->getItem().getorder().GetType() == TYPE_VGAN) {
						pGUI->PrintMessage("Normal Cook #" + to_string(cok2->getItem().GetID()) + " has been assigned to vegan order #" + to_string(cok2->getItem().getorder().GetID()), countmsg);
					}

				}
				else if (cok2->getItem().GetType() == TYPE_VIP) {
					if (cok2->getItem().getorder().GetType() == TYPE_NRM) {
						pGUI->PrintMessage("VIP Cook #" + to_string(cok2->getItem().GetID()) + " has been assigned to Normal order #" + to_string(cok2->getItem().getorder().GetID()), countmsg);
					}
					else if (cok2->getItem().getorder().GetType() == TYPE_VIP) {
						pGUI->PrintMessage("VIP Cook #" + to_string(cok2->getItem().GetID()) + " has been assigned to VIP order #" + to_string(cok2->getItem().getorder().GetID()), countmsg);
					}
					else if (cok2->getItem().getorder().GetType() == TYPE_VGAN) {
						pGUI->PrintMessage("VIP Cook #" + to_string(cok2->getItem().GetID()) + " has been assigned to vegan order #" + to_string(cok2->getItem().getorder().GetID()), countmsg);
					}

				}
				else if (cok2->getItem().GetType() == TYPE_VGAN) {
					if (cok2->getItem().getorder().GetType() == TYPE_NRM) {
						pGUI->PrintMessage("Vegan Cook #" + to_string(cok2->getItem().GetID()) + " has been assigned to Normal order #" + to_string(cok2->getItem().getorder().GetID()), countmsg);
					}
					else if (cok2->getItem().getorder().GetType() == TYPE_VIP) {
						pGUI->PrintMessage("Vegan Cook #" + to_string(cok2->getItem().GetID()) + " has been assigned to VIP order #" + to_string(cok2->getItem().getorder().GetID()), countmsg);
					}
					else if (cok2->getItem().getorder().GetType() == TYPE_VGAN) {
						pGUI->PrintMessage("Vegan Cook #" + to_string(cok2->getItem().GetID()) + " has been assigned to vegan order #" + to_string(cok2->getItem().getorder().GetID()), countmsg);
					}

				}
			}
			cok2 = cok2->getNext();
		}
		if (EventsQueue->isEmpty() && SRVList->getHead() == nullptr && VIPQ->isEmpty() && NRMQ->isEmpty() && VeganQ->isEmpty()) {
			while (DoneQ->dequeue(x)) {
				FileQ->enqueue(x);
			}
		}
		
		/////////////////////////////////////////////////////////////////////////////////////////

		pGUI->UpdateInterface();
		//Sleep(1000);
		CurrentTimeStep++;	//advance timestep
		pGUI->ResetDrawingList();
		//pGUI->PrintMessage("generation done, click to END program");
		pGUI->waitForClick();

	}

	save("intermode file");

	
}
void Restaurant::stepmode() {
	//Now We have filled EventsQueue (randomly)
	int CurrentTimeStep = 1;
	

	Load("test5");
	//as long as events queue is not empty yet
	Node<Cook>* cokr1 = Cok->getHead();
	while (cokr1) {

		if (cokr1->getItem().GetType() == TYPE_NRM)countnrmcooks++;
		else if (cokr1->getItem().GetType() == TYPE_VIP)countvipcooks++;
		else if (cokr1->getItem().GetType() == TYPE_VGAN)countvgncooks++;
		cokr1 = cokr1->getNext();
	}

	while (!EventsQueue->isEmpty() || SRVList->getHead() != nullptr || !VIPQ->isEmpty() || !NRMQ->isEmpty() || !VeganQ->isEmpty()||!DoneQ->isEmpty())
	{
		//print current timestep
		char timestep[10];
		itoa(CurrentTimeStep, timestep, 10);



		//The next line may add new orders to the DEMO_Queue
		ExecuteEvents(CurrentTimeStep);	//execute all events at current time step


/////////////////////////////////////////////////////////////////////////////////////////
		/// The next code section should be done through function "FillDrawingList()" once you
		/// decide the appropriate list type for Orders and Cooks

		//Let's add ALL randomly generated Cooks to GUI::DrawingList

		Node<Cook>* cokr = Cok->getHead();
		int countcook = 0;
		while (cokr->getNext()) {
			

			
			
			if (cokr->getItem().check_aval()) {
				countcook++;
				pGUI->AddToDrawingList(&cokr->getItem());
				
			}
			cokr = cokr->getNext();
		}
		if (cokr->getItem().check_aval()) {
			countcook++;
			pGUI->AddToDrawingList(&cokr->getItem());

		}
		//Let's add ALL randomly generated Ordes to GUI::DrawingList

		Order x;

		VIP_PQ* tempvip = new VIP_PQ;
		Queue<Order>* tempvgn = new Queue<Order>;
		Queue<Order>* tempsrv = new Queue<Order>;
		Queue<Order>* tempdone = new Queue<Order>;
		Node<Order>* p = NRMQ->getHead();
		Node<Order>* r = SRVList->getHead();
		while (VIPQ->dequeue(x)) {
			pGUI->AddToDrawingList(&x);
			tempvip->enqueue(x);

		}
		while (tempvip->dequeue(x)) {
			VIPQ->Add(x);
		}
		while (p && p->getNext()) {
			pGUI->AddToDrawingList(&p->getItem());
			p = p->getNext();

		}
		if (p) {
			pGUI->AddToDrawingList(&p->getItem());

		}

		while (VeganQ->dequeue(x)) {
			pGUI->AddToDrawingList(&x);
			tempvgn->enqueue(x);
		}
		while (tempvgn->dequeue(x)) {
			VeganQ->enqueue(x);
		}
		while (r && r->getNext()) {
			pGUI->AddToDrawingList(&r->getItem());
			r = r->getNext();

		}
		if (r) {
			pGUI->AddToDrawingList(&r->getItem());

		}

		/*
		while (SRVQ->dequeue(x)) {
			pGUI->AddToDrawingList(&x);
			tempsrv->enqueue(x);
		}
		while (tempsrv->dequeue(x)) {
			SRVQ->enqueue(x);
		}
		*/
		while (DoneQ->dequeue(x)) {
			pGUI->AddToDrawingList(&x);
			tempdone->enqueue(x);
		}
		while (tempdone->dequeue(x)) {
			DoneQ->enqueue(x);
		}

		pGUI->PrintStats(CurrentTimeStep, countcook, countwaitorder);
		Node<Cook>* cok2 = Cok->getHead();
		int countmsg = 0;
		while (cok2) {
			if (cok2->getItem().getorder().getServTime() == (CurrentTimeStep - 1)) {
				countmsg++;
				if (cok2->getItem().GetType() == TYPE_NRM) {
					if (cok2->getItem().getorder().GetType() == TYPE_NRM) {
						pGUI->PrintMessage("Normal Cook #" + to_string(cok2->getItem().GetID()) + " has been assigned to Normal order #" + to_string(cok2->getItem().getorder().GetID()), countmsg);
					}
					else if (cok2->getItem().getorder().GetType() == TYPE_VIP) {
						pGUI->PrintMessage("Normal Cook #" + to_string(cok2->getItem().GetID()) + " has been assigned to VIP order #" + to_string(cok2->getItem().getorder().GetID()), countmsg);
					}
					else if (cok2->getItem().getorder().GetType() == TYPE_VGAN) {
						pGUI->PrintMessage("Normal Cook #" + to_string(cok2->getItem().GetID()) + " has been assigned to vegan order #" + to_string(cok2->getItem().getorder().GetID()), countmsg);
					}

				}
				else if (cok2->getItem().GetType() == TYPE_VIP) {
					if (cok2->getItem().getorder().GetType() == TYPE_NRM) {
						pGUI->PrintMessage("VIP Cook #" + to_string(cok2->getItem().GetID()) + " has been assigned to Normal order #" + to_string(cok2->getItem().getorder().GetID()), countmsg);
					}
					else if (cok2->getItem().getorder().GetType() == TYPE_VIP) {
						pGUI->PrintMessage("VIP Cook #" + to_string(cok2->getItem().GetID()) + " has been assigned to VIP order #" + to_string(cok2->getItem().getorder().GetID()), countmsg);
					}
					else if (cok2->getItem().getorder().GetType() == TYPE_VGAN) {
						pGUI->PrintMessage("VIP Cook #" + to_string(cok2->getItem().GetID()) + " has been assigned to vegan order #" + to_string(cok2->getItem().getorder().GetID()), countmsg);
					}

				}
				else if (cok2->getItem().GetType() == TYPE_VGAN) {
					if (cok2->getItem().getorder().GetType() == TYPE_NRM) {
						pGUI->PrintMessage("Vegan Cook #" + to_string(cok2->getItem().GetID()) + " has been assigned to Normal order #" + to_string(cok2->getItem().getorder().GetID()), countmsg);
					}
					else if (cok2->getItem().getorder().GetType() == TYPE_VIP) {
						pGUI->PrintMessage("Vegan Cook #" + to_string(cok2->getItem().GetID()) + " has been assigned to VIP order #" + to_string(cok2->getItem().getorder().GetID()), countmsg);
					}
					else if (cok2->getItem().getorder().GetType() == TYPE_VGAN) {
						pGUI->PrintMessage("Vegan Cook #" + to_string(cok2->getItem().GetID()) + " has been assigned to vegan order #" + to_string(cok2->getItem().getorder().GetID()), countmsg);
					}

				}
			}
			cok2 = cok2->getNext();
		}
		
		if (EventsQueue->isEmpty() && SRVList->getHead() == nullptr && VIPQ->isEmpty() && NRMQ->isEmpty() && VeganQ->isEmpty()) {
			while (DoneQ->dequeue(x)) {
				FileQ->enqueue(x);
			}
		}
		CookStep(CurrentTimeStep);
		AssignCooks(CurrentTimeStep);
		/////////////////////////////////////////////////////////////////////////////////////////

		pGUI->UpdateInterface();
		CurrentTimeStep++;	//advance timestep
		Sleep(1000);
		pGUI->ResetDrawingList();
		
		

	}

	save("stepmode file");

	

}
void Restaurant::silentmode() {
	//Now We have filled EventsQueue (randomly)
	int CurrentTimeStep = 1;


	Load("test6");
	//as long as events queue is not empty yet
	Node<Cook>* cokr1 = Cok->getHead();
	while (cokr1) {

		if (cokr1->getItem().GetType() == TYPE_NRM)countnrmcooks++;
		else if (cokr1->getItem().GetType() == TYPE_VIP)countvipcooks++;
		else if (cokr1->getItem().GetType() == TYPE_VGAN)countvgncooks++;
		cokr1 = cokr1->getNext();
	}

	while (!EventsQueue->isEmpty() || SRVList->getHead() != nullptr || !VIPQ->isEmpty() || !NRMQ->isEmpty() || !VeganQ->isEmpty() || !DoneQ->isEmpty())
	{
		//print current timestep
		char timestep[10];
		itoa(CurrentTimeStep, timestep, 10);



		//The next line may add new orders to the DEMO_Queue
		ExecuteEvents(CurrentTimeStep);	//execute all events at current time step


/////////////////////////////////////////////////////////////////////////////////////////
		/// The next code section should be done through function "FillDrawingList()" once you
		/// decide the appropriate list type for Orders and Cooks

		//Let's add ALL randomly generated Cooks to GUI::DrawingList

		Node<Cook>* cokr = Cok->getHead();
		int countcook = 0;
		while (cokr->getNext()) {



			
			if (cokr->getItem().check_aval()) {
				countcook++;
			}
			cokr = cokr->getNext();
		}
		if (cokr->getItem().check_aval()) {
			countcook++;
			

		}
		//Let's add ALL randomly generated Ordes to GUI::DrawingList

		Order x;
		if (EventsQueue->isEmpty() && SRVList->getHead() == nullptr && VIPQ->isEmpty() && NRMQ->isEmpty() && VeganQ->isEmpty()) {
			while (DoneQ->dequeue(x)) {
				
				FileQ->enqueue(x);
			}
		}

		CookStep(CurrentTimeStep);
		AssignCooks(CurrentTimeStep);
		/////////////////////////////////////////////////////////////////////////////////////////

		
		CurrentTimeStep++;	//advance timestep
		
		



	}
	
	save("silent mode file");
	pGUI->PrintMessage("Output file generated");
	


}


void Restaurant::setcountp() {
	countpromote++;
}


