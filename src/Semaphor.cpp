#include "Semaphor.h"
#include "Global.h"
#include "KerSem.h"
#include "SysCall.h"
#include "dos.h"
#include "iostream.h"

unsigned temcx, temdx;
Semaphore::Semaphore(int i)
{
	SysInfo info;
	info.type = _Semaphor;
	info.val = i;
	info.s = this;
	temcx = FP_SEG(&info);
	temdx = FP_OFF(&info);
	asm{
		push cx;
		push dx;
		mov cx, temcx;
		mov dx, temdx;
	    int 0x61;
		pop dx;
		pop cx;
	}
}
Semaphore::~Semaphore()
{
	SysInfo info;
	info.type = _DesSemaphor;
	info.ID = ID;
	temcx = FP_SEG(&info);
	temdx = FP_OFF(&info);
	asm{
		push cx;
		push dx;
		mov cx, temcx;
		mov dx, temdx;
		int 0x61;
		pop dx;
		pop cx;
	}
}
void Semaphore::wait()
{
	SysInfo info;
	info.type = _SemWait;
	info.ID = ID;
	temcx = FP_SEG(&info);
	temdx = FP_OFF(&info);
	asm{
		push cx;
		push dx;
		mov cx, temcx;
		mov dx, temdx;
	    int 0x61;
		pop dx;
		pop cx;
	}
}
void Semaphore::signal()
{
	SysInfo info;
	info.type = _SemSignal;
	info.ID = ID;
	temcx = FP_SEG(&info);
	temdx = FP_OFF(&info);
	asm{
		push cx;
		push dx;
		mov cx, temcx;
		mov dx, temdx;
	    int 0x61;
		pop dx;
		pop cx;
	}
}
int Semaphore::val() const
{
	SysInfo info;
	info.type = _SemVal;
	info.ID = ID;
	temcx = FP_SEG(&info);
	temdx = FP_OFF(&info);
	asm{
		push cx;
		push dx;
		mov cx, temcx;
		mov dx, temdx;
	    int 0x61;
		pop dx;
		pop cx;
	}
	return info.val;
}



