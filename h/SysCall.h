#ifndef SYSCALL_H_
#define SYSCALL_H_
#include "Thread.h"
#include "Semaphor.h"
#include "Event.h"

typedef enum {_Thread, _DesThread, _WaitToComp, _Sleep, _Start, _Dispatch, _Semaphor, _DesSemaphor, _SemWait, _SemSignal, _SemVal, _Event, _DesEvent, _EventWait, _EventSignal} CallType;

class SysCall;

struct SysInfo
{
	CallType type;
	unsigned ID;
	Thread *t;
	StackSize stackSize;
	Time timeSlice;
	Time timeToSleep;
	Semaphore* s;
	int val;
	IVTNo ivtNo;
	Event* e;
};

class SysCall
{
	friend class Semaphore;
	friend class Event;
	static SysInfo *sysInfo;
public:
	static Thread* initial;
	static void SysCallInic();
	static void SysCallEnd();
	static void interrupt SysCallRout(...);
};



#endif /* SYSCALL_H_ */
