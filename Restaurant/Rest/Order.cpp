#include "Order.h"
#include<iostream>
using namespace std;
Order::Order()
{
}
Order::Order(int id, ORD_TYPE r_Type)
{
	ID = (id>0&&id<1000)?id:0;	//1<ID<999
	type = r_Type;
	status = WAIT;
}
Order::Order(int id) {
	ID = id;
}
Order::~Order()
{

}

int Order::GetID()const
{
	return ID;
}
ORD_TYPE Order::GetType() const
{
	return type;
}
void Order::SetDistance(int d)
{
	Distance = d>0?d:0;
}
int Order::GetDistance() const
{
	return Distance;
}
void Order::setStatus(ORD_STATUS s) 

{
	this->status = s;
}
ORD_STATUS Order::getStatus() const
{
	return status;
}
double Order:: getPriority()const
{
	if (this->getStatus() == TYPE_VGAN || this->getStatus() == TYPE_NRM)
		return 0;
	else
		return 2*totalMoney+size-ArrTime;
}
void Order :: setMoney(double m)
{
	totalMoney = m;
}
double Order:: getMoney() const
{
	return totalMoney;
}
void Order::setSize(int s)
{
	size = s;
}
int Order::getSize() const
{
	return size;
}
void Order:: setArrTime(int T)
{
	ArrTime = T;
}
int Order::getArrTime()const
{
	return ArrTime;
}
ostream& operator<<(ostream& out, const Order& O)
{
	out << O.GetID() ;
	return out;
}
void Order::setLimit(int i) {
	Limit = i;
}
int Order::getLimit() {
	return Limit;
}
void Order::setType(ORD_TYPE o) {
	type = o;
}
ORD_TYPE Order::getType() {
	return type;
}
void Order::setServTime(int t)
{
	ServTime = t;
}
int Order:: getServTime()
{
	return ServTime;
}
void Order::setServDur(int t)
{
	ServDuration = t;
}
int Order::getServDur()
{
	return ServDuration;
}