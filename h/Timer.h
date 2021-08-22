#ifndef TIMER_H_
#define TIMER_H_
#include <dos.h>
#include "Idle.h"

typedef void interrupt (*pInterrupt)(...);
class Timer
{
public:
	static Idle *idle;
	static int requested;
	static pInterrupt old;
	static void interrupt TimerNew(...);
	static void TimerInic();
	static void TimerRestore();
};



#endif /* TIMER_H_ */
