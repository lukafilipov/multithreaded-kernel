#include "Event.h"
#include "KernelEv.h"
#include "dos.h"
#include "SysCall.h"
#include "iostream.h"

unsigned tmpcx, tmpdx;
Event::Event(IVTNo ivtNo)
{
	SysInfo info;
	info.type = _Event;
	info.ivtNo = ivtNo;
	info.e = this;
	tmpcx = FP_SEG(&info);
	tmpdx = FP_OFF(&info);
	asm{
		push cx;
		push dx;
		mov cx, tmpcx;
		mov dx, tmpdx;
	    int 0x61;
		pop dx;
		pop cx;
	}
}
Event::~Event()
{
	SysInfo info;
	info.type = _DesEvent;
	info.ID = ID;
	tmpcx = FP_SEG(&info);
	tmpdx = FP_OFF(&info);
	asm{
		push cx;
		push dx;
		mov cx, tmpcx;
		mov dx, tmpdx;
	    int 0x61;
		pop dx;
		pop cx;
	}
}
void Event::wait()
{
	SysInfo info;
	info.type = _EventWait;
	info.ID = ID;
	tmpcx = FP_SEG(&info);
	tmpdx = FP_OFF(&info);
	asm{
		push cx;
		push dx;
		mov cx, tmpcx;
		mov dx, tmpdx;
	}
	asm int 0x61;
		  asm{
			pop dx;
			pop cx;
		}
}
void Event::signal()
{
	SysInfo info;
	info.type = _EventSignal;
	info.ID = ID;
	tmpcx = FP_SEG(&info);
	tmpdx = FP_OFF(&info);
	asm{
		push cx;
		push dx;
		mov cx, tmpcx;
		mov dx, tmpdx;
		int 0x61;
		pop dx;
		pop cx;
	}
}



