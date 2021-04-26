#include "Timer.h"
//定时器
u32 TimerCnt(u32 *tc, s32 Condition)
{
	if(Condition == 0)
	{
		*tc = SysTim_GetUser();
	}
	return (SysTim_GetUser()-*tc)/10;
}
//定时器复位
void TimerRst(u32 *tc)
{
	*tc = SysTim_GetUser();
}
