#include "Timer.h"
#include "PCB.h"
#include <iostream.h>
#include "Idle.h"
#include "SCHEDULE.h"
#include "iostream.h"

extern void tick();
unsigned Tsp,Tss,Tbp,t;
PCB::Sleep* s;
Idle* Timer::idle = 0;
int Timer::requested = 0;
pInterrupt Timer::old = 0;

void Timer::TimerInic()
{
	idle= new Idle();
	old=getvect(0x8);
	setvect(0x60, old);
	setvect(0x8, TimerNew);
}

void Timer::TimerRestore()
{
	delete idle;
	setvect(0x8, old);
}

void interrupt Timer::TimerNew(...)
{
	tick();
	if (!requested)
		{
			if (PCB::count > 0) PCB::count--;
			if (PCB::sleepHead != 0)
			if (--PCB::sleepHead->time == 0)
			{
				while((PCB::sleepHead != 0)&&(PCB::sleepHead->time == 0))
				{
					PCB::sleepHead->elem->state = Ready;
					Scheduler::put(PCB::sleepHead->elem);
					s = PCB::sleepHead;
					PCB::sleepHead = PCB::sleepHead->next;
					delete s;
				}
			}

		}
		
	if (!requested) asm int 0x60;
	if (((PCB::count == 0) && (PCB::running->timeSlice !=0 )) || requested)
	{
		if (PCB::lockFlag)
		{
			requested = 0;
			asm{
				mov Tsp, sp;
				mov Tss, ss;
				mov Tbp, bp;
			}
			PCB::running->sp = Tsp;
			PCB::running->ss = Tss;
			PCB::running->bp = Tbp;
			if (PCB::running->state != Done && PCB::running->state != Blocked && PCB::running != PCB::idle)  
				Scheduler::put(PCB::running);
			PCB::running = Scheduler::get();
			if (PCB::running == 0) {PCB::running = PCB::idle;}
			Tsp = PCB::running->sp;
			Tss = PCB::running->ss;
			Tbp = PCB::running->bp;
			PCB::count = PCB::running->timeSlice;
			asm{
				mov sp, Tsp;
				mov ss, Tss;
				mov bp, Tbp;
			}
		}
		
		else requested = 1;
	}
}
