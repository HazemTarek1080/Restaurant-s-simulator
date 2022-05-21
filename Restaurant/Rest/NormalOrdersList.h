
#ifndef _ORDERSLIST
#define _ORDERSLIST
#include"../Generic_DS/Node.h"
#include"Order.h"
#include<iostream>
using namespace std;
#include"string.h"
class NormalOrdersList
{
private:
	Node<Order>* Head;
	int count;

public:
	NormalOrdersList()
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
			Head->setNext(nullptr);
			count++;
			return true;
		}//List is Empty
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
	
	Order* MoveNormalOrder(int ID)
	{
		if (Head == nullptr) {
			return nullptr;
		}
		Node<Order>* p = Head;

		if (p->getItem().GetID() == ID)
		{
			Head = p->getNext();
			Order* O =new Order (p->getItem());
			delete p;
			count--;
			return O;
		}
		else
		{
			while (p->getNext() != nullptr && p->getNext()->getItem().GetID() != ID)
			{
				p = p->getNext();
			}
			if (p->getNext() == nullptr)
			{
				return nullptr;
			}
			Node<Order>* r = p->getNext();
			p->setNext(r->getNext());
			Order* O = new Order(r->getItem());
			delete r;
			count--;
			return O;
		}
		return nullptr;
	}
	void PrintList()	const
	{
		cout << "\nList has " << count << " Normal Orders";
		cout << "\nprinting list contents:\n\n";
		Node<Order>* O = Head;

		while (O)
		{
			cout << O->getItem();
			cout << "--->";
			O = O->getNext();
		}
		cout << "*\n";
	}
	bool isEmpty() {
		if (Head == nullptr) {
			return true;
		}
		else {
			return false;
		}
	}
	Node<Order>* getHead() {
		return Head;
	}
	/*
	Order *RemoveFirst()
	{
		if (Head == nullptr) {
			return nullptr;
		}
		Node<Order>* P = Head;
		Head = Head->getNext();
		Order O = P->getItem();
		delete P;
		return &O;
	}
	*/
	void RemoveFirst()
	{
		if (Head == nullptr) {
			return ;
		}
		Node<Order>* P = Head;
		Head = Head->getNext();
		//delete P;
		count--;
	}
};
#endif
