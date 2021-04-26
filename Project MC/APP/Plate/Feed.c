/*
*@Header:
*@File Name:	Feed.c
*@Author: Yao
*@Data: 2021/03/05
*@Description: 上料相关:
*					FeedPlate()		直振里面的三个真空
*					FeederRound()	圆振
*					FeederFlat()	直振
*					FeederHopper()	料斗
*					KeepFeed()		手动连续送料相关
*					Feed()			外部轮询 logic.c
*/

#include "Logic.h"
#include "Feed.h"
#include "Plate.h"

/**
* @author 21/03/05  yao
* @Description: 直振里面的三个真空
* @param --  
* @return --
*/
static void FeedPlate()
{
	static TrigDef tr_Vacuo_ON[3],tr_Blow_ON,tr_InpStart;
	static TrigDef tr_Vacuo_OFF[3],tr_Blow_OFF,tr_InpEnd;
	static u32	Flag_Vacuo_ON[3],Flag_Vacuo_OFF[3],Flag_InpCheck;
	s32 Pos_Vacuo_ON[3] = {GUS.PlateData.Data.Pos_FeedVacuo1ON,GUS.PlateData.Data.Pos_FeedVacuo2ON,GUS.PlateData.Data.Pos_FeedVacuo3ON};
	s32 Pos_Vacuo_OFF[3] = {GUS.PlateData.Data.Pos_FeedVacuo1OFF,GUS.PlateData.Data.Pos_FeedVacuo2OFF,GUS.PlateData.Data.Pos_FeedVacuo3OFF};
	
	for(int i=0; i<3; i++)
	{
		if(PlateTrig(&tr_Vacuo_ON[i],Pos_Vacuo_ON[i]))
		{
			Flag_Vacuo_ON[i] = 1;
			Flag_Vacuo_OFF[i] = 0;
		}
	}
	for(int i=0; i<3; i++)
	{
		if(PlateTrig(&tr_Vacuo_OFF[i],Pos_Vacuo_OFF[i]))
		{
			Flag_Vacuo_ON[i] = 0;
			Flag_Vacuo_OFF[i] = 1;
		}
	}
	if(PlateTrig(&tr_InpStart,GUS.PlateData.Data.Pos_InpStart))
	{
		Flag_InpCheck = 1;
	}
	if(PlateTrig(&tr_InpEnd,GUS.PlateData.Data.Pos_InpEnd))
	{
		Flag_InpCheck = 0;
	}
	//真空1开
	if(Flag_Vacuo_ON[0] == 1)
	{
		Flag_Vacuo_ON[0] = 0;
		Q_Set(Q_FeedVacuo1,ON);
	}
	//真空1关
	if(Flag_Vacuo_OFF[0] == 1)
	{
		Flag_Vacuo_OFF[0] = 0;
		Q_Set(Q_FeedVacuo1,OFF);
		GUR.PlateLoopData.Data.Have[0] = 1;
		GUR.PlateLoopData.Data.OK[0] = 1;
		GUR.PlateLoopData.Data.Elec[0] = 0;
		if(I_Get(I_FlatInp) == ON)//物料到位
		{
			Data.PlateRun.IntoFastNum++;//转盘快慢速切换计数
			Data.PlateRun.IntoSlowNum = 0;
		}
		else
		{
			Data.PlateRun.IntoSlowNum++;//转盘快慢速切换计数
			Data.PlateRun.IntoFastNum = 0;
		}
	}
	//真空2开
	if(Flag_Vacuo_ON[1] == 1)
	{
		Flag_Vacuo_ON[1] = 0;
		Q_Set(Q_FeedVacuo2,ON);
	}
	//真空2关
	if(Flag_Vacuo_OFF[1] == 1)
	{
		Flag_Vacuo_OFF[1] = 0;
		Q_Set(Q_FeedVacuo2,OFF);
	}
	//真空3开
	if(Flag_Vacuo_ON[2] == 1)
	{
		Flag_Vacuo_ON[2] = 1;
		Q_Set(Q_FeedVacuo3,ON);
	}
	//真空3关
	if(Flag_Vacuo_OFF[2] == 1)
	{
		Flag_Vacuo_OFF[2] = 0;
		Q_Set(Q_FeedVacuo3,OFF);
	}
	//分离吹气开
	if(PlateTrig(&tr_Blow_ON,GUS.PlateData.Data.Pos_FeedBlowON))//分离吹开位置触发
	{
		if(I_Get(I_FlatInp) == OFF)//物料没到位才开
		{
			Q_Set(Q_FeedBlow,ON);
		}
	}
	//分离吹气关
	if(PlateTrig(&tr_Blow_OFF,GUS.PlateData.Data.Pos_FeedBlowOFF))//分离吹关位置触发
	{
		Q_Set(Q_FeedBlow,OFF);
	}
	//到位检测
	if(Flag_InpCheck)
	{
		if(I_Get(I_FlatInp) == ON)
		{
			Flag_InpCheck = 0;
			Q_Set(Q_FeedVacuo1,ON);
			Q_Set(Q_FeedVacuo2,ON);
			Q_Set(Q_FeedVacuo3,ON);
		}
	}
}

/**
* @author 21/03/05  yao
* @Description: 圆振
* @param --  
* @return --
*/
//圆振
static void FeederRound()
{
	TASKRUN(LT.FeederRoundTask)
	{
	
	}
}

/**
* @author 21/03/05  yao
* @Description: 直振
* @param --  
* @return --
*/
//直振
static void FeederFlat()
{
	TASKRUN(LT.FeederFlatTask)
	{
	
	}
}

/**
* @author 21/03/05  yao
* @Description: 料斗
* @param --  
* @return --
*/
//料斗
//static void FeederHopper()
//{
//	static u32 tm;
//	if(FSM_STA == RUN && I_Get(I_RoundLack) == ON && GUS.EnableData.Data.Hopper == 1)//运行状态 并且 圆振缺料 并且手动开启此功能
//	{
//		LT.FeederHopperTask.execute = TEXE;
//	}
//	TASKRUN(LT.FeederHopperTask)
//	{
//		case 1:
//			Q_Set(Q_FeederHopper,ON);	//料斗控制
//			TimerRst(&tm);
//			my.step = 2;
//			break;
//		
//		case 2:
//			if(TimerCnt(&tm,1) > GUS.TimeData.Data.Tim_Hopper)//料斗振动周期
//			{
//				Q_Set(Q_FeederHopper,OFF);	//料斗控制
//				TimerRst(&tm);
//				my.step = 3;
//			}
//			break;
//			
//		case 3:
//			if(TimerCnt(&tm,1) > GUS.TimeData.Data.Tim_Hopper)//料斗振动周期
//			{
//				TASKOK;
//			}
//			break;
//	}
//}
static void FeederHopper()
{
	s32 IO_RoundLack = GUS.TransWatch.Data.IO_Trans_Hopper;
	static u32 tm,tm_HopperKeep,tm_HopperTime;
	if(FSM_STA == RUN && I_Get(I_RoundLack) == IO_RoundLack && GUS.EnableData.Data.Hopper == 1)//运行状态 并且 圆振缺料 并且手动开启此功能
	{
		LT.FeederHopperTask.execute = TEXE;
	}
	TASKRUN(LT.FeederHopperTask)
	{
		case 1://延时
			TimerRst(&tm_HopperKeep);
			my.step = 2;
			break;
		
		case 2:
			if(TimerCnt(&tm_HopperKeep,InPut_GetSta(I_RoundLack) == IO_RoundLack) > GUS.TimeData.Data.Tim_HopperKeep)//料斗振动信号确认
			{
				TimerRst(&tm_HopperTime);	//清空延时
				TimerRst(&tm);				//清空计时
				my.step = 3;
			}
			break;
			
		case 3:
			if(TimerCnt(&tm,1) > GUS.TimeData.Data.Tim_Hopper)//料斗振动周期
			{
				Q_Set(Q_FeederHopper,ON);	//料斗控制
				TimerRst(&tm);
				my.step = 4;
			}
			break;
		case 4:
			if(TimerCnt(&tm,1) > GUS.TimeData.Data.Tim_Hopper)//料斗振动周期
			{
				Q_Set(Q_FeederHopper,OFF);	//料斗控制
				TimerRst(&tm);
				my.step = 5;
			}
			break;
		case 5:
			if(TimerCnt(&tm_HopperTime,1) >= GUS.TimeData.Data.Tim_HopperTime || FSM_STA != RUN || I_Get(I_RoundLack) == (1-IO_RoundLack))
				//振动时间到达 || 状态不在运行中 || 感应器没信号
			{
				TimerRst(&tm);
				TASKOK;
			}
			else
			{
				TimerRst(&tm);
				my.step = 3;
			}
			break;
	}
}

/**
* @author 21/03/05  yao
* @Description: 手动连续送料相关
* @param --  
* @return --
*/
//手动连续送料相关
void KeepFeed()
{
	static TrigDef Tr_ManualOn,Tr_ManualOff;
	if(FSM_STA != STOP)
	{
		GUW.ManualData.Data.KeepFeed = 0;
	}
	if(TrigOne(&Tr_ManualOn,GUW.ManualData.Data.KeepFeed == 1,0))//直振 圆振 开位置
	{
		Q_Set(Q_FeederFlat,ON);		//直振开
		Q_Set(Q_FeederRound,ON);	//圆振开
	}
	if(TrigOne(&Tr_ManualOff,GUW.ManualData.Data.KeepFeed == 0,0))//直振 圆振 关位置
	{
		Q_Set(Q_FeederFlat,OFF);	//直振关
		Q_Set(Q_FeederRound,OFF);   //圆振关
	}
}


void Feed()
{
	FeedPlate();
	FeederRound();
	FeederFlat();
	FeederHopper();
	KeepFeed();
}

