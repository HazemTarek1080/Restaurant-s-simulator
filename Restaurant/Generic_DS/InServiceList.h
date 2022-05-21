#ifndef _SERVICELIST
#define _SERVICELIST
#include"../Generic_DS/Node.h"
#include"../Rest/Order.h"
#include<iostream>
using namespace std;
#include"string.h"


class InServiceList
{
private:
	Node<Order>* Head;
	int count;

public:
	InServiceList()
	{
		count = 0;
		Head = nullptr;
	}
	bool AddToList(Order O)
	{
		Node<Order>* newOrder = new Node<Order>(O);
		if (Head == nullptr)
		{
			Head = newOrder;
			Head->setNext(nullptr);                  //List is Empty
			count++;
			return true;
		}
		

		else
		{
			Node<Order>* P = Head;
			while (P->getNext() != nullptr)
			{
				P = P->getNext();
			}
			P->setNext(newOrder);
			newOrder->setNext(nullptr);
			count++;
			return true;
		}
	}
	
	Node<Order>* getHead() 
	{
		return Head;
	}
	
	
	bool DeleteFromList(Node<Order>* O)
	{
		if (Head == nullptr) {
			return false;
		}
		if (Head == O)
		{
			if (O->getNext() == nullptr)
			{
				Head = nullptr;
				delete Head;
				return true;
			}
			else
			{
				Head = Head->getNext();
				O = nullptr;
				delete O; 
				return true;
			}
		}
		else
		{
			Node<Order>* p = Head;
			while (p->getNext() != nullptr&&p->getNext() != O)
			{
				p = p->getNext();
			}
			if (p->getNext() == nullptr)
			{
				return false;
			}
			Node<Order>* r = p->getNext();
			p->setNext(r->getNext());
			//Order* O = new Order(r->getItem());
			r = nullptr;
			delete r;
			count--;
			return true;
		}
		return false;
	}
};
#endif
