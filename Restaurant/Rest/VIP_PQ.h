#ifndef _VIP_PQ
#define _VIP_PQ
#include"../Generic_DS/Queue.h"
#include"Order.h"
#include<iostream>
using namespace std;
#include"string.h"
class VIP_PQ : public Queue<Order>
{

public:
	bool VIP_PQ::Add(const Order& newEntry)
	{
		int x = newEntry.getPriority();

		Node<Order>* newNodePtr = new Node<Order>(newEntry);
		// Insert the new node
		if (isEmpty())
		{
			frontPtr = newNodePtr;
			return true;
		}// The queue is empty
		else if (x >= frontPtr->getItem().getPriority())
		{
			newNodePtr->setNext(frontPtr);
			frontPtr = newNodePtr;
			return true;
		} // VIP order is placed at the begining of the queue
		else if (frontPtr->getNext()== nullptr && x <= frontPtr->getItem().getPriority())
		{
			frontPtr->setNext(newNodePtr);
			newNodePtr->setNext(nullptr);
			return true;
		} // VIP order is placed after the head
		else
		{
			Node<Order>* P = frontPtr;
			while (P->getNext()!= nullptr && x < P->getNext()->getItem().getPriority())
			{
				P = P->getNext();
			}
			P->setNext(newNodePtr);
			newNodePtr->setNext(P->getNext());
		}// VIP order is placed according to its priority
	}
	/*
	void VIP_PQ::Print()
	{
		VIP_PQ* temp;
		Node<Order>* p ;
		while (!this->isEmpty())
		{
			this->dequeue(p->getItem());
			temp->enqueue(p->getItem());
			cout << p->getItem() << " ";
		}
		while (!temp->isEmpty())
		{
			temp->dequeue(p->getItem());
			this->enqueue(p->getItem());
		}

	}
	*/
};
#endif
