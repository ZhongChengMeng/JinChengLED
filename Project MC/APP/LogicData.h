/*
 * @Author: your name
 * @Date: 2020-05-26 14:03:11
 * @LastEditTime: 2020-05-29 15:41:48
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: \YD_KZJ\Code\APP\LogicData.h
 */ 
#ifndef __LOGICDATA_H
#define __LOGICDATA_H
#include "bsplib.h"


typedef enum
{
	TIDLE,//状态，任务是否空闲
	TEXE,//指令，任务开始执行
	TSTOP,//指令，任务终止
}TaskSta;

typedef struct
{
    u8 execute;
    u8 step;
    u8 done;
	u8 Manual;	//手动标记，表示本次动作由手动触发
    u32 count;	//用于记录圈数，必须放大单位
    u32 tm;		//任务定时器
	u32 Exetime;//任务执行时间
} LogicParaDef;

#define PARAINIT(a)  memset(&a,0,sizeof(a));
#define INITTASK(a) if(a.execute==1&&a.step==0){a.step = 1;}


#define TRST(T) T->&tm = SysTim_GetUser()
#define TCNT(T) ((SysTim_GetUser() - (T->&tm)) / 10)
#define INITT(a)                      \
	if (a->execute == 1 && a->step == 0) \
	{                                    \
		a->step = 1;                      \
		a->done = 0;						\
	}
#define STEP(a) a->step 


#endif
