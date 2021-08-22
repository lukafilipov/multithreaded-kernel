#include "IVTEntry.h"
#include "dos.h"
#include "iostream.h"

IVTEntry::IVTEntries *IVTEntry::first =0;
IVTEntry::IVTEntry(IVTNo ivtno, pInterrupt inter)
{
	kerev = 0;
    ivtNo = ivtno;
    old = getvect(ivtno);
    setvect(ivtno, inter);
    first = new IVTEntries(this, first);
}
IVTEntry::~IVTEntry()
{
	setvect(ivtNo, old);
	old();
}
void IVTEntry::signal()
{
	kerev->signal();
}
IVTEntry* IVTEntry::get(IVTNo No)
{
	IVTEntries* temp = first;
	while((temp != 0) && (temp->elem->ivtNo != No))
	{
		temp = temp->next;
	}
	return temp->elem;
}
void IVTEntry::CallOld()
{
	old();
}

