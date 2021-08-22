#ifndef KERNELEV_H_
#define KERNELEV_H_

#include "PCB.h"
#include "Global.h"
#include "Event.h"

class KernelEv: public GlobalList
{
	PCB *creator, *blocked;
	friend class Event;
	friend class SysCall;
	friend class IVTEntry;
	int val;
	unsigned ID;
	KernelEv(IVTNo ivtNo, Event* ev);
	void wait();
	void signal();

};





#endif /* KERNELEV_H_ */
