#include "SysCall.h"
#include "Thread.h"
#include "PCB.h"
#include "Global.h"
#include <dos.h>
#include "Timer.h"
#include "SCHEDULE.h"
#include "iostream.h"
#include "KerSem.h"
#include "KernelEv.h"

unsigned tcx, tdx, tsp, tss, tbp, tmpsp, tmpss, tmpbp, a, *stack;
SysInfo *SysCall::sysInfo = 0;
Thread *SysCall::initial = 0;
void SysCall::SysCallInic()
{
	setvect(0x61,SysCallRout);
	stack = new unsigned[defaultStackSize / 2];
	tmpsp = FP_OFF(stack + defaultStackSize / 2);
	tmpss = FP_SEG(stack + defaultStackSize / 2);
	tmpbp = tmpsp;
	initial = new Thread(defaultStackSize, 1);
    PCB* p = (PCB*)GlobalList::get(initial->ID);
	p->state = Ready;
	PCB::running = p;
}

void SysCall::SysCallEnd()
{
	delete initial;
	delete stack;
}

void interrupt SysCall::SysCallRout(...)
{
	PCB::lockFlag = 0;
#ifndef BCC_BLOCK_IGNORE

	 asm{
	     mov tsp, sp;
	     mov tss, ss;
	     mov tbp, bp;
	   }
	if (initial != 0)
	{
	    PCB::running->sp = tsp;
        PCB::running->ss = tss;
	    PCB::running->bp = tbp;
	}
	asm{
		mov tcx, cx;
		mov tdx, dx;
	}
	sysInfo = (SysInfo*)MK_FP(tcx, tdx);
	
	asm{
		mov sp, tmpsp;
		mov ss, tmpss;
		mov bp, tmpbp;
	}
#endif

	asm sti;
	
	switch(sysInfo->type)
	{
	case _Thread:
		    new PCB(sysInfo->stackSize,sysInfo->timeSlice, sysInfo->t);
			break;
	case _DesThread:
	    	 delete (PCB*)GlobalList::remove(sysInfo->ID);
	    	 break;
	case _WaitToComp:
	    	// PCB* p=(PCB*)(GlobalList::get(sysInfo->ID));
		    ((PCB*)(GlobalList::get(sysInfo->ID)))->waitToComplete();
	    	 break;
	case _Sleep:
	    	 PCB::sleep(sysInfo->timeToSleep);
	    	 break;
	case _Start:
	    	((PCB*)(GlobalList::get(sysInfo->ID)))->start();
	    	break;
	case _Dispatch:
			Timer::requested = 1;
			break;
	case _Semaphor:
		new KernelSem(sysInfo->val, sysInfo->s);
		break;
	case _DesSemaphor:
		delete (KernelSem*)GlobalList::remove(sysInfo->ID);
		break;
	case _SemWait:
		((KernelSem*)(GlobalList::get(sysInfo->ID)))->wait();
	    break;
	case _SemSignal:
		((KernelSem*)(GlobalList::get(sysInfo->ID)))->signal();
		break;
	case _SemVal:
		sysInfo->val=((KernelSem*)(GlobalList::get(sysInfo->ID)))->val;
		break;
	case _Event:
		new KernelEv(sysInfo->ivtNo, sysInfo->e);
		break;
	case _DesEvent:
		delete (KernelEv*)GlobalList::remove(sysInfo->ID);
		break;
	case _EventWait:
		((KernelEv*)(GlobalList::get(sysInfo->ID)))->wait();
		break;
	case _EventSignal:
		((KernelEv*)(GlobalList::get(sysInfo->ID)))->signal();

	}
	
	asm cli;
	
	if (Timer::requested == 1)
	{
		if ((PCB::running->state != Done) && (PCB::running->state!=Blocked) && (PCB::running!=PCB::idle)) {Scheduler::put(PCB::running); }
		PCB::running=Scheduler::get();
		if (PCB::running == 0)
		{
			PCB::running = PCB::idle;
		}
		PCB::count = PCB::running->timeSlice;
		Timer::requested = 0;
	}
	if (initial != 0)
	{
		tsp = PCB::running->sp;
		tss = PCB::running->ss;
		tbp = PCB::running->bp;
	}
	asm{
	    mov sp, tsp;
		mov ss, tss;
		mov bp, tbp;
	   }
	PCB::lockFlag = 1;
}
