#include "Thread.h"
#include "PCB.h"
#include "Global.h"
#include "SCHEDULE.h"
#include "iostream.h"
#include "Timer.h"
#include "SysCall.h"
#include <dos.h>


PCB *PCB::running = 0;
PCB::Sleep *PCB::sleepHead = 0;
PCB* PCB::idle = 0;
int PCB::lockFlag = 1;
unsigned int PCB::count = 0, TSP , TSS, TBP, c;
PCB::PCB(StackSize stacksize, Time timeslice, Thread* t)
{
	blockedHead = 0;
	ID=GlobalList::UkID++;
	t->ID = ID;
	timeSlice = timeslice;
	stackSize = stacksize;
	stackSize /= 2;
	stack = new unsigned[stackSize];
	stack[stackSize - 1] = FP_SEG(t);
	stack[stackSize - 2] = FP_OFF(t);
	stack[stackSize - 5] = 0x200;
	stack[stackSize - 6] = FP_SEG(Thread::wrapper);
	stack[stackSize - 7] = FP_OFF(Thread::wrapper);
	ss = FP_SEG(stack + stackSize - 16);
	sp = FP_OFF(stack + stackSize - 16);
	bp = sp;
	state = New;
	GlobalList::put(this, ID);

}
PCB::~PCB()
{
	delete [] stack;
}

void PCB::sleep(Time timeToSleep)
{
	if(timeToSleep > 0){
	PCB::running->state = Blocked;
	Sleep *s = new Sleep(PCB::running,timeToSleep);
	Sleep *temp = sleepHead, *old = 0;
	while ((temp != 0) && (s->time >= temp->time))
	{
		s->time -= temp->time;
		old = temp;
		temp = temp->next;
	}
	if ((sleepHead != 0) && (temp != 0) && (old != 0))
	{
		old->next = s;
		s->next = temp;
		temp->time -= s->time;
	}
	else if(sleepHead == 0)
	{
		sleepHead = s;
	}
	else if(temp == 0)
	{
		old->next = s;
	}
	else if(old == 0)
	{
		temp->time -= s->time;
		s->next = sleepHead;
		sleepHead = s;
	}
	Timer::requested = 1;
	}
}
void PCB::waitToComplete()
{
	if(state != Done)
	{
		PCB::running->state = Blocked;
		blockedHead = new Block(PCB::running, blockedHead);
	}
	Timer::requested = 1;
}
void PCB::start()
{
	if (state == New)
	{
		state = Ready;
		Scheduler::put(this);
	}
}
