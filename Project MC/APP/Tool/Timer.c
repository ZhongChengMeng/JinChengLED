#include "Timer.h"
//��ʱ��
u32 TimerCnt(u32 *tc, s32 Condition)
{
	if(Condition == 0)
	{
		*tc = SysTim_GetUser();
	}
	return (SysTim_GetUser()-*tc)/10;
}
//��ʱ����λ
void TimerRst(u32 *tc)
{
	*tc = SysTim_GetUser();
}
