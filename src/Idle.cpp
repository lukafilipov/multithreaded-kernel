#include "Idle.h"
#include "PCB.h"
#include "Thread.h"
#include "Global.h"
#include "iostream.h"

Idle::Idle():Thread(defaultStackSize,1)
	{
		PCB* p = (PCB*)(GlobalList::get(1));
		PCB::idle = p;
	}
void Idle::run()
{
	while(1);

}


