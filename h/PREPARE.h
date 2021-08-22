#ifndef PREPARE_H_
#define PREPARE_H_

#include "IVTEntry.h"

#define PREPAREENTRY(IVTnum, oldrout)\
	void interrupt rout##IVTnum(...);\
	IVTEntry entry##IVTnum(IVTnum, rout##IVTnum);\
	void interrupt rout##IVTnum(...)\
    {\
	if (oldrout == 1) entry##IVTnum.CallOld();\
		entry##IVTnum.signal();\
	}





#endif /* PREPARE_H_ */
