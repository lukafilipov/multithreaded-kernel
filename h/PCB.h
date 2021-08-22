#ifndef PCB_H_
#define PCB_H_

#include "Thread.h"
#include "Global.h"
#include "Idle.h"

class Thread;
class SysCall;
class Timer;
class Idle;
class Nit;
typedef enum {Blocked,Done,Ready,New} States;

#define lock() {asm{pushf; cli;}}
#define unlock() {asm{popf}}


class PCB: public GlobalList
{
	States state;
	unsigned int *stack, sp, ss, bp, timeSlice, ID, stackSize;
	static PCB* running;
	static PCB* idle;

	static int lockFlag;
	static unsigned int count;
	struct Block
	{
		PCB *elem;
		Block *next;
		Block(PCB* p, Block* n = 0)
		{
			elem = p;
			next = n;
		}
	};
	Block *blockedHead;
	struct Sleep
	{
		PCB *elem;
		Time time;
		Sleep *next;
		Sleep(PCB* p,Time t)
		{
			elem = p;
			time = t;
			next = 0;
		}
	};
	static Sleep* sleepHead;
	PCB(StackSize stacksize = defaultStackSize, Time timeslice = defaultTimeSlice, Thread* t);
	~PCB();
	void static put(Sleep* s);
	void waitToComplete();
	void start();
	void static sleep(Time timeToSleep);
	friend class Thread;
	friend class SysCall;
	friend class Timer;
	friend class Idle;
	friend class KernelSem;
	friend class KernelEv;
public:
	void static interrupt change(...);
};



#endif /* PCB_H_ */
