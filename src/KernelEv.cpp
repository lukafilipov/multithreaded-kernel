#include "SysCall.h"
#include "KernelEv.h"
#include "Schedule.h"
#include "Timer.h"
#include "IVTEntry.h"
#include "iostream.h"

KernelEv::KernelEv(IVTNo ivtNo, Event* ev)
{
	ID = GlobalList::UkID++;
	val = 0;
	ev->ID = ID;
	creator = PCB::running;
	blocked = 0;
	(IVTEntry::get(ivtNo))->kerev = this;
	GlobalList::put(this, ID);
}
void KernelEv::signal()
{
	if (blocked == 0) val = 1;
	else
	{
		blocked->state = Ready;
		Scheduler::put(blocked);
		blocked = 0;
		if (PCB::lockFlag == 0)
		{
			Timer::requested = 1;
		}
		else{
			dispatch();
		}
	}
}
void KernelEv::wait()
{
	if (PCB::running == creator)
		if (val == 0)
		{
			blocked=PCB::running;
			blocked->state = Blocked;
			Timer::requested = 1;
		}
		else val = 0;
}
