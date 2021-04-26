#ifndef _PLATE_H_
#define	_PLATE_H_
	#include "bsplib.h"
	#include "Trigger.h"
	extern void Plate(void);
	extern void Plate_Goto(s32 Dir, s32 TarPos);//Dir == -1:·´  1:Õý
	extern s32 PlateTrig(TrigDef *tr, s32 TarPos);

#endif


