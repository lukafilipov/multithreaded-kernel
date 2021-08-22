#include "iostream.h"
#include "Global.h"
#include "PCB.h"

unsigned int GlobalList::UkID = 0;
GlobalList::Elem *GlobalList::first = 0;
GlobalList::Elem *GlobalList::last = 0;

void GlobalList::put(GlobalList *c, unsigned int d)
{
	Elem *a = new Elem(d, c);
	if (first == 0) {first = a; last = a;}
	
	else
	{
		a->next = first;
		first->prev = a;
		first = a;
	}
}

GlobalList* GlobalList::get(unsigned int id)
{
	Elem* i;
	for(i = first; (id != i->id) && (i != 0); i = i->next);
	if (i == 0) { return 0;}
	else return i->elem;
}

GlobalList* GlobalList::remove(unsigned int id)
{
	Elem* i;
	GlobalList* pom;
	for(i = first; (id != i->id) && (i != 0); i = i->next);
	if (i == 0) { return 0;}
	else
	{
		if(i->prev != 0) i->prev->next = i->next;
		else first = i->next;
		if(i->next != 0) i->next->prev = i->prev;
		else last = i->prev;
		pom = i->elem;
		delete i;
		return pom;
	}
}

GlobalList::Elem::Elem(unsigned int a, GlobalList* b)
{
	id = a;
	elem = b;
	next = 0;
	prev = 0;
}
