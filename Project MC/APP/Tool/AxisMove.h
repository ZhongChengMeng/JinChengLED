/*
 * @Author: your name
 * @Date: 2020-05-26 15:09:21
 * @LastEditTime: 2020-06-30 21:27:56
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: \YD_KZJ\Code\APP\Tool\AxisMove.h
 */
#ifndef __AXISMOVE_H
#define __AXISMOVE_H
#include "logic.h"


enum {
    GOHOMETYPE1,	     //直接回原点(平常模式)
    GOHOMETYPE2,		//先到上限位再回原点
    GOHOMETYPE3,		//先回下限位再回原点
    GOHOMETYPE4,        //z向找原点正向
    GOHOMETYPE5,        //z向找原点反向
};

typedef enum
{
    AX_ERR_AX_ALM = (1 << 0),		//轴报警
    AX_ERR_AX_TMOUT = (1 << 1),		//超时（回原点）
    AX_ERR_UP_LIM_H = (1 << 8),		//硬上限位
    AX_ERR_UP_LIM_S = (1 << 9),		//软上限位
    AX_ERR_DW_LIM_H = (1 << 12),		//硬下限位
    AX_ERR_DW_LIM_S = (1 << 13),		//软下限位
} AxErrBitTp;

typedef enum
{
    AXSTA_READY = 0,	//就绪状态
    AXSTA_STOP,			//停止状态
    AXSTA_STOPDEC,		//停止状态(减速停)
    AXSTA_DISCRETE,		//普通运动状态
    AXSTA_CONTINUOUS,	//连续运动
    AXSTA_HOMING,		//正在回原点
    AXSTA_DISABLE,		//未激活状态
    AXSTA_ERRSTOP,		//错误停
    AXSTA_SYNC,			//轴同步状态
} AxStateTp;

typedef enum	//轴号定义
{
	AxAdjust1,
	AxAdjust2,
	AxBelt,
	AxPlate,
} AxisNum;

typedef enum	//轴动模式选择
{
    ABSMODE,	//0
    RELMODE,	//1
    SPDMODE,	//2
    GOHOMEMODE,	//3
    STOPMODE,	//4
} MoveMode;

/*********  轴移动接口 *********/
s32 HZ_MoveUnit(u32 num,u32 speed,float pos,u32 mode);
void SetAxisSpdRadio(u8 i,u8 spd);
/******  特殊的轴移动接口 ******/
s32 UserGetStatus(u8 AxNum);
void UserGoHome(u8 AxNum);


//脉冲移动接口 Mr.Yiu
extern void AxAbs(u8 num, s32 pos)      ;
extern void AxRel(u8 num, s32 pos)      ;
extern void AxAbsSpd(u8 num, s32 pos, u8 spd)      ;
extern void AxRelSpd(u8 num, s32 pos, u8 spd)      ;
extern void AxHome(u8 num)              ;
extern AxStateTp AxSta(u8 num);         ;

#endif
