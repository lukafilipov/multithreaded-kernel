
#ifndef IVTENTRY_H_
#define IVTENTRY_H_

#include "KernelEv.h"
#include "Timer.h"

class KernelEv;

class IVTEntry
{
	struct IVTEntries
	{
		IVTEntry* elem;
		IVTEntries* next;
		IVTEntries(IVTEntry* e, IVTEntries* n = 0)
		{
			elem = e;
			next = n;
		}
	};
	static IVTEntries *first;
	KernelEv* kerev;
	IVTNo ivtNo;
	pInterrupt old;
	static IVTEntry* get(IVTNo No);
	friend class KernelEv;
public:
	IVTEntry(IVTNo ivtNo, pInterrupt inter);
	~IVTEntry();
	void signal();
	void CallOld();

};



#endif /* IVTENTRY_H_ */
