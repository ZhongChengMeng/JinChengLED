#include "InterLayer.h"
#include "Algorithm.h"
#include "can_master.h"
#include "usertool.h"
#include "iodefine.h"

void Axis_pos(void);

//每次轮询的接口函数
void InterLayer()
{
    //获取rtc时间
    RTC_Get_Time(&GSR.time.hour, &GSR.time.min, &GSR.time.sec, &GSR.time.ampm);
    RTC_Get_Date(&GSR.date.year,&GSR.date.month,&GSR.date.day,&GSR.date.week);
    //底层轴动流程
    HZ_AxMotion();
    //modbus通讯流程
    HZ_ModBus(uart_232, 1);
    HZ_ModBus(uart_485,1);
    //软件定时器
    HZ_TimerThread();
    //启用多轴插补
    //HZ_MultiAxMotion();
    //地址表中的报警函数
    HZ_Alarm();
    //获取轴当前状态
    Axis_pos();
}
/**
* @author  yang
* @Description: 获取轴当前状态
* @param -- 用户自己定义内部需要获取的内容
* @param --
* @return --
*/
void Axis_pos()
{
    int i;
    for(i=0; i<PULS_NUM; i++)//6以内
    {
        GSR.AxisPosition[i]= HZ_AxGetCurPos(i);
        GSR.AxisUnitPosition[i] = ConvertToUserUnit(i, GSR.AxisPosition[i]);
    }
    for(i=0; i<PULS_NUM_Ex; i++)//扩展轴
    {
        GSR.AxisPosition[i+PULS_NUM] = HZ_ExAxGetCurPos(i);
        GSR.AxisUnitPosition[i] = ConvertToUserUnit(i, GSR.AxisPosition[i]);
    }
	for(i=0; i<ECD_NUM; i++)//3
	{
		GSR.AxisEncoder[i] = EnCode_Get32(i)/2;
	}
	GUW.DisplayData.Data.PlatePosCmd = (u32)GSR.AxisPosition[AxPlate]%PPR_PLATE;
	GUW.DisplayData.Data.PlatePosEnc = (u32)GSR.AxisEncoder[0]%PPR_PLATE;
}

/**
* @author  yang
* @Description: 自带点动调用函数。当执行点动时调用
* @param --
* @param --
* @return --
*/
void JogGo(u8 axisnum,s32 pos,u32 spd)
{
	float movepos = pos+GSR.AxisUnitPosition[axisnum];
	
	if(movepos>GSS.axis[axisnum].Axlimitcfg.softmaxlimit)
	{
		movepos = GSS.axis[axisnum].Axlimitcfg.softmaxlimit;
	}else if(movepos<0)
	{
		movepos = 0;
	}
	SetAxisSpdRadio(axisnum, spd);	//设置速度
    if(axisnum<PULS_NUM)	//只有主卡部分点动函数
    {
        if(pos > 0)	//正向点动
        {
            if(AXSTA_ERRSTOP == HZ_AxGetStatus(axisnum))
            {
                //只有下限错误
                if(0 == (0x0fff & HZ_AxGetAxisErr(axisnum)))
                {
                    HZ_AxReset(axisnum);
                    HZ_AxMoveRel(axisnum,pos);
                }
            } else
            {
                //没有报警,正常运动
                HZ_AxMoveRel(axisnum,pos);
            }
        }
        else	//反向点动
        {
            if(AXSTA_ERRSTOP == HZ_AxGetStatus(axisnum))
            {
                //只有上限错误
                if(0 == (0xf0ff & HZ_AxGetAxisErr(axisnum)))
                {
                    HZ_AxReset(axisnum);
                    HZ_AxMoveRel(axisnum,pos);
                }
            } else {
                //没有报警,正常运动
                HZ_AxMoveRel(axisnum,pos);
            }
        }
    }else 
	{
		if(pos > 0)	//正向点动
        {
            if(AXSTA_ERRSTOP == HZ_ExAxGetStatus(axisnum-PULS_NUM))
            {
                //只有下限错误
                if(0 == (0x0fff & HZ_ExAxGetErr(axisnum-PULS_NUM)))
                {
                    HZ_ExAxMoveRel(axisnum-PULS_NUM,pos);
                }
            } else {
                //没有报警,正常运动
                HZ_ExAxMoveRel(axisnum-PULS_NUM,pos);
            }
        }
        else	//反向点动
        {
            if(AXSTA_ERRSTOP == HZ_ExAxGetStatus(axisnum-PULS_NUM))
            {
                //只有上限错误
                if(0 == (0xf0ff & HZ_ExAxGetErr(axisnum-PULS_NUM)))
                {
                    HZ_ExAxMoveRel(axisnum-PULS_NUM,pos);
                }
            } else {
                //没有报警,正常运动
                HZ_ExAxMoveRel(axisnum-PULS_NUM,pos);
            }
        }
	}
}
//系统点动停止调用函数
void jogstop(u32 axisnum)
{
    if(axisnum<PULS_NUM)
        HZ_AxStop(axisnum);
    else
        HZ_ExAxStop(axisnum-PULS_NUM);
}
//系统回零调用函数
void joghome(u32 axisnum)
{
	if(axisnum<PULS_NUM)
	{
		HZ_AxReset(axisnum);
		HZ_AxHome(axisnum);
	}else 
	{
		HZ_ExAxHome(axisnum-PULS_NUM);
	}
}
/**
* @author  yang
* @Description: 扩展卡输入输出状态的Modbus更新，当Modbus咨询到指定地址后调用
* @param --
* @param --
* @return --
*/
void ex_inputupdata()
{
    u8 i;
    //扩展卡1
    GSR.InputStatus[4] = 0;
    GSR.InputStatus[5] = 0;
    GSR.InputStatus[6] = 0;
    GSR.InputStatus[7] = 0;
    for(i=0; i<32; i++){
        GSR.InputStatus[4] |=(u32) HZ_ExInPutGet(0,i)<<i;
    }

}

void ex_outputstatusupdata()
{
    u8 i;
    //扩展输出板1
    GSW.OutputStatus[4] = 0;
    for(i = 0; i < 32; i ++){
        GSW.OutputStatus[4]	|= (u32)HZ_ExOutPutGet(0,i) <<	i;
	}

}

void ex_outputupdata()
{
    u8 i;
    //扩展输出板1
    for(i = 0; i < 32; i ++){
        HZ_ExOutPutSet(0,i, (GSW.OutputStatus[4]>>(i)&0x01));
    }
}

