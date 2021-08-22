#include "Thread.h"
#include "PCB.h"
#include "Global.h"
#include "Timer.h"
#include "SCHEDULE.h"
#include "SysCall.h"
#include "iostream.h"
#include <dos.h>

class Timer;

unsigned pomcx,pomdx,b;

Thread::Thread(StackSize stackSize, Time timeSlice)
{
	SysInfo info;
		info.type = _Thread;
		info.stackSize = stackSize;
		info.timeSlice = timeSlice;
		info.t = this;
		pomcx = FP_SEG(&info);
		pomdx = FP_OFF(&info);
		asm{
			push cx;
			push dx;
			mov cx, pomcx;
			mov dx, pomdx;
		}
		asm	int 0x61;
		asm{
			pop dx;
			pop cx;
		}
}

Thread::~Thread()
{
	SysInfo info;
		info.type = _DesThread;
	    info.ID = ID;
			pomcx = FP_SEG(&info);
			pomdx = FP_OFF(&info);
			asm{
				push cx;
				push dx;
				mov cx, pomcx;
				mov dx, pomdx;
			int 0x61;
				pop dx;
				pop cx;
			}
}

void Thread::waitToComplete()
{
		SysInfo info;
			info.type = _WaitToComp;
		    info.ID = ID;
				pomcx = FP_SEG(&info);
				pomdx = FP_OFF(&info);
				asm{
					push cx;
					push dx;
					mov cx, pomcx;
					mov dx, pomdx;
				}
				asm int 0x61;
				asm{
					pop dx;
					pop cx;
				}
}

void dispatch()
{
	SysInfo info;
			info.type = _Dispatch;
				pomcx = FP_SEG(&info);
				pomdx = FP_OFF(&info);
				asm{
					push cx;
					push dx;
					mov cx, pomcx;
					mov dx, pomdx;
				}
				asm int 0x61;
				asm{
					pop dx;
					pop cx;
				}
}

void Thread::start()
{
	SysInfo info;
		info.type = _Start;
		info.ID = ID;
			//#ifndef BCC_BLOCK_IGNORE
				pomcx = FP_SEG(&info);
				pomdx = FP_OFF(&info);
				asm{
					push cx;
					push dx;
					mov cx, pomcx;
					mov dx, pomdx;
				int 0x61;
					pop dx;
					pop cx;
				}
			//#endif
}

void Thread::wrapper(Thread* running)
{
	running->run();
	PCB::Block *q;
	while (PCB::running->blockedHead != 0)
	{
		PCB::running->blockedHead->elem->state = Ready;
		asm cli;
		Scheduler::put(PCB::running->blockedHead->elem);
		asm sti;
		q = PCB::running->blockedHead;
		PCB::running->blockedHead = PCB::running->blockedHead->next;
		delete q;
	}
	PCB::running->state = Done;
	dispatch();
}

void Thread::sleep(Time timeToSleep)
{
	SysInfo info;
			info.type = _Sleep;
		    info.timeToSleep = timeToSleep;
			//#ifndef BCC_BLOCK_IGNORE
				pomcx = FP_SEG(&info);
				pomdx = FP_OFF(&info);
				asm{
					push cx;
					push dx;
					mov cx, pomcx;
					mov dx, pomdx;
				}
				asm int 0x61;
				asm{
					pop dx;
					pop cx;
				}
}
