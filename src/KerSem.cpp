#include "KerSem.h"
#include "Thread.h"
#include "Schedule.h"
#include "PCB.h"
#include "Semaphor.h"
#include "iostream.h"
#include "Global.h"
#include "Timer.h"

KernelSem::KernelSem(int i, Semaphore* sem)
{
	ID=GlobalList::UkID++;
	val = i;
	sem->ID = ID;
	head = 0;
	GlobalList::put(this, ID);
}
void KernelSem::wait()
{
	if(--val < 0)
	{
		PCB::running->state = Blocked;
		block(PCB::running);
		Timer::requested = 1;
	}

}
void KernelSem::signal()
{
	if(++val <= 0)
	{
		PCB* a = unblock();
		a->state = Ready;
		Scheduler::put(a);
	}
}
KernelSem::~KernelSem()
{
	while(head != 0)
	{
		signal();
	}
}
void KernelSem::block(PCB* p)
{
	head = new SemBlock(p, head);
}
PCB* KernelSem::unblock()
{
		SemBlock* a = head;
		head = head->next;
		PCB* p = a->elem;
		delete a;
		return p;
}
