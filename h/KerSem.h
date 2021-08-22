#ifndef KERNELSEM_H_
#define KERNELSEM_H_

#include "PCB.h"
#include "Global.h"
#include "Semaphor.h"

class PCB;
class Semaphore;

class KernelSem: public GlobalList
{
	struct SemBlock
	{
		PCB* elem;
		SemBlock *next;
		SemBlock(PCB* e, SemBlock* n = 0)
		{
			elem=e;
			next=n;
		}
	};
	SemBlock* head;
	friend class Semaphore;
	friend class SysCall;
	int val;
	unsigned ID;
	KernelSem(int i, Semaphore* sem);
	~KernelSem();
	void wait();
	void  signal();
	void block(PCB*);
	PCB* unblock();

};



#endif /* KERNELSEM_H_ */

