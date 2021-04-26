#include "axismove.h"
#include "userTool.h"
#include "bsplib.h"
#include "interlayer_config.h"
#include "datadefine.h"
#include "AxisConfig.h"
#include "can_master.h"

/**********************************************************************/
/***************		轴移动接口						***************/
/**********************************************************************/
/* <1> 按比例走用户单位 */
/**
* @author 19/02/19  yang
* @Description: 轴移动接口
* @param -- 轴号 速度 位置 模式
* @param --  速度 百分比
* @return --
*/
s32 HZ_MoveUnit(u32 num,u32 speed,float pos,u32 mode)
{
    if(num>PULS_NUM+PULS_NUM_Ex)
        return -1;
    SetAxisSpdRadio(num, speed);	//设置速度
    switch(mode)
    {
    case ABSMODE:
        HZ_AxMoveAbs(num,ConverToPulse(num, pos));
        break;
    case RELMODE:
        HZ_AxMoveRel(num, ConverToPulse(num, pos));
        break;
    case SPDMODE:
        HZ_AxMoveVelocity(num, ConverToPulse(num, pos));
        break;
    case GOHOMEMODE:
        HZ_AxHome(num);
        break;
    default:
        break;
    }
    return 0;
}
/*
 * Created: 11/16
 * Description:按比例设置用户速度
 * param:
 * Modified By:yang
 */
void SetAxisSpdRadio(u8 i,u8 spd)
{
    if(spd>=100)
    {
        spd = 100;
    }
    if(spd == 0)
    {
        spd = 1;
    }
	AxspdDef Axspd;
	Axspd = GSS.axis[i].AxSpd;
	float radio = (float)spd / 100.0f;
	Axspd.runspeed = Axspd.runspeed * radio;
	AxisSetSpd(i,Axspd);
}


/**********************************************************************/
/***************		特殊定义的轴移动接口			***************/
/**********************************************************************/
/**
* @author 18/12/14  汪豪
* @Description: 获取轴状态
* @param 	AxNum:超过主卡轴数为扩展卡
* @param --
* @return --
*/
s32 UserGetStatus(u8 AxNum)
{
	if (AxNum < PULS_NUM)
	{
		return HZ_AxGetStatus(AxNum);
	}
	else
	{
		return HZ_ExAxGetStatus(AxNum - PULS_NUM);
	}
}

/**
* @author 19/02/18  wangh
* @Description: 回原点函数
* @param AxNum:超过主卡轴数为扩展卡
* @param --
* @return --
*/
void UserGoHome(u8 AxNum)
{
    if (AxNum < PULS_NUM)
    {
        HZ_AxReset(AxNum);
        HZ_AxHome(AxNum);
    }
    else
    {
		HZ_ExAxHome(AxNum - PULS_NUM);
    }
}

/**
* @author Mr.Yiu	2021.1.20
* @Description: 全速轴ABS脉冲
* @param 
* @param --
* @return --
*/
void AxAbs(u8 num, s32 pos)
{
    if(num>PULS_NUM+PULS_NUM_Ex)
        return;
    SetAxisSpdRadio(num, 100);	//设置速度
	HZ_AxMoveAbs(num,pos);
}

/**
* @author Mr.Yiu	2021.1.20
* @Description: 全速轴Rel脉冲
* @param 
* @param --
* @return --
*/
void AxRel(u8 num, s32 pos)
{
    if(num>PULS_NUM+PULS_NUM_Ex)
        return;
    SetAxisSpdRadio(num, 100);	//设置速度
	HZ_AxMoveRel(num,pos);
}

/**
* @author Mr.Yiu	2021.1.20
* @Description: 全速轴ABS脉冲
* @param 
* @param --
* @return --
*/
void AxAbsSpd(u8 num, s32 pos, u8 spd)
{
    if(num>PULS_NUM+PULS_NUM_Ex)
        return;
    SetAxisSpdRadio(num, spd);	//设置速度
	HZ_AxMoveAbs(num,pos);
}

/**
* @author Mr.Yiu	2021.1.20
* @Description: 全速轴Rel脉冲
* @param 
* @param --
* @return --
*/
void AxRelSpd(u8 num, s32 pos, u8 spd)
{
    if(num>PULS_NUM+PULS_NUM_Ex)
        return;
    SetAxisSpdRadio(num, spd);	//设置速度
	HZ_AxMoveRel(num,pos);
}

/**
* @author Mr.Yiu	2021.1.20
* @Description: 轴回零
* @param 
* @param --
* @return --
*/
void AxHome(u8 num)
{
    if(num>PULS_NUM+PULS_NUM_Ex)
        return;
	HZ_AxHome(num);
}

/**
* @author Mr.Yiu	2021.1.20
* @Description: 轴状态查询
* @param 
* @param --
* @return --
*/
AxStateTp AxSta(u8 num)
{
    if(num>PULS_NUM+PULS_NUM_Ex)
        return AXSTA_READY;
	return (AxStateTp)HZ_AxGetStatus(num);
}




