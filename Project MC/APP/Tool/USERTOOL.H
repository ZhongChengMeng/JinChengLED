/*
 * @Author: your name
 * @Date: 2020-05-26 15:09:21
 * @LastEditTime: 2020-05-29 16:37:37
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: \YD_KZJ\Code\APP\Tool\USERTOOL.H
 */ 
#ifndef _USERTOOL_H_
#define	_USERTOOL_H_

#include "bsplib.h"

//定时触发器
typedef struct
{
    u32 start;
	u32 starttick;
}TmTyp;
u32 TimOn(TmTyp *tm,int ex,u32 timeout);


float UPH(u32 time,u32 products);
s32 ConverToPulse(u32 num,float MM);
float ConvertToUserUnit(u32 num,s32 pulse);
void Hz_AutoVision(void);

s32 InGet(u8 cardAdr,u8 num);
s32 OutGet(u8 cardAdr,u8 num);
s32 OutSet(u8 cardAdr,u8 num,u8 value);

/*文件操作*/
#define	Wait_Fram()			do{							\
								FRam_Exec();			\
							}while(FRAM_GetSta()!= 0)	\

int File_export(void);
int File_import(void);

#endif


