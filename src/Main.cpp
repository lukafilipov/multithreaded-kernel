#include <iostream.h>
#include "thread.h"
#include "semaphor.h"
#include "event.h"
#include "Timer.h"
#include "SysCall.h"


extern int userMain(int argc,char *argv[]);


int main(int argc, char** argv)
{
	SysCall::SysCallInic();
	Timer::TimerInic();
	int value = userMain(argc, argv);
	Timer::TimerRestore();
	SysCall::SysCallEnd();
	return value;
}


