#ifndef _COOKSLIST
#define _COOKSLIST
#include"../Generic_DS/Node.h"
#include"Cook.h"
#include<iostream>
using namespace std;
#include"string.h"
class CooksList
{
private:
	Node <Cook>* Head;

	int count;
public:
	CooksList()
	{
		count = 0;
		Head = nullptr;
	}
	
	
void buildList(int const noV, int const noN, int const noG, int S_V, int S_N, int S_G, int B_V, int B_N, int B_G, int B_O)
	{
		cooks_info in_info; in_info.BN = B_N; in_info.BV = B_V; in_info.BG = B_G; in_info.SN = S_N; in_info.SV = S_V; in_info.SG = S_G;
		
		count = noV + noN + noG;
		Cook C1;
		C1.readcookinfo(in_info); 
		C1.setType(TYPE_VIP);
		C1.setID(1);
		C1.setBO(B_O);
		Node<Cook>* C = new Node<Cook>(C1);
		Head = C;
		Node<Cook>* p = Head;
		for (int i = 1; i < count; i++)
		{
			C1.readcookinfo(in_info);
			C1.setID(i + 1);
			C1.setBO(B_O);
			if (i < noV) {
				C1.setType(TYPE_VIP);
				//C1.setBD(in_info.BV);
			}
			if (i >= noV && i < (noV + noN)) {
				C1.setType(TYPE_NRM);
			}
			if (i >= (noV + noN)) {
				C1.setType(TYPE_VGAN);
			}

			Node<Cook>* C = new Node<Cook>(C1);
			p->setNext(C);
			p = p->getNext();

		}
	}

	~CooksList()
	{

	}


	
	void PrintList()	const
	{
		cout << "\nList has " << count << " cooks";
		cout << "\nprinting list contents:\n\n";
		Node<Cook>* C= Head;

		while (C)
		{
			//cout << "[ " << C->getItem() << " ]";
			cout <<C->getItem() ;
			cout << "--->";
			C = C->getNext();
		}
		cout << "*\n";
	}

	
	void DeleteAll()
	{
		Node<Cook>* C= Head;
		while (Head)
		{
			C = Head->getNext();
			delete Head;
			Head = C;
		}
		count = 0;
	}


	Node<Cook>* getHead() 
	{
		return Head;
	}

	void InsertEnd(const Cook& data);

	bool Find(Cook* Key);

	int CountOccurance(const Cook& value);
};

#endif
/*
	CooksList(int const noV, int const noN, int const noG, int S_V, int S_N, int S_G, int B_V, int B_N, int B_G,int B_O)
	{
		cooks_info in_info; in_info.BN = B_N; in_info.BV = B_V; in_info.BG = B_G; in_info.SN = S_N; in_info.SV = S_V; in_info.SG = S_G;
		count = noV + noN + noG;
		Cook C1;
		C1.readcookinfo(in_info); C1.setType(TYPE_VIP);
		C1.setID(1);
		C1.setBO(B_O);
		Node<Cook>* C = new Node<Cook>(C1);
		Head = C;
		Node<Cook>* p = Head;
		for (int i = 1; i < count; i++)
		{
			C1.readcookinfo(in_info);
			C1.setID(i + 1);
			C1.setBO(B_O);
			if (i < noV) C1.setType(TYPE_VIP);
			if (i >= noV && i < (noV + noN)) C1.setType(TYPE_NRM);
			if (i >= (noV + noN)) C1.setType(TYPE_VGAN);



				Node<Cook>* C = new Node<Cook>(C1);
				p->setNext(C);
				p = p->getNext();


		}
		/*
		Cook* C_arr=new Cook[count-1];
		for (int i = 0; i < count; i++)
		{
			C_arr[i].readcookinfo(in_info);
			if (i < noV) C_arr[i].setType(TYPE_VIP);
			if(i>=noV && i< noV+noN) C_arr[i].setType(TYPE_NRM);
			else if(i>= noV+noN) C_arr[i].setType(TYPE_VGAN);
		}

		Node<Cook*>* C = new Node<Cook*>(C_arr[0]);
			Head = C;
			Node<Cook*>* p =Head;
			for (int j = 1; j < count; j++)
			{
			Node<Cook*> *C = new Node<Cook*>(C_arr[j]);
			p->setNext(C);
			p = p->getNext();
			}
			*/