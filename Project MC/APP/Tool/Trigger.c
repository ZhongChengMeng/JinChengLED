#include "Trigger.h"
//触发器
s32	TrigOne(TrigDef *this, s32 Condition, u32 Time)
{
	if(Condition == 0)//不成立
	{
		this->TrigFlag = 1;
		this->TrigTimeStamp = SysTim_GetUser();
	}
	if(Condition != 0 && this->TrigFlag == 1 && (SysTim_GetUser() - this->TrigTimeStamp)/10 > Time)//成立 并且是跳变
	{
		this->TrigFlag = 0;
		return 1;
	}
	return 0;
}
//触发器复位
void TrigRst(TrigDef *this)
{
	this->TrigFlag = 0;
	this->TrigTimeStamp = SysTim_GetUser();
}



