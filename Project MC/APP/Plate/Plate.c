/*
*@Header:
*@File Name:	Plate.c
*@Author: Yao
*@Data: 2021/03/05
*@Description: 转盘位置相关:
*					PlateTrig(TrigDef *tr, s32 TarPos)	转盘工位触发器
*					PlateDataMove()	转盘数据流动任务
*					Plate_Goto(s32 Dir, s32 TarPos)	转盘按指定方向走到指定一圈内坐标
*					PlateRun()		转盘运行控制，自动切换慢速、快速状态，接收外部立即停、慢速停指令
*					PlateHome()		转盘运行回零
*					Plate()			外部logic.c使用
*/

#include "Logic.h"
#include "Feed.h"

/**
* @author 21/03/05  yao
* @Description: 转盘工位触发器
* @param -- TrigDef触发器 脉冲
* @return --
*/
//一圈内的位置触发器，需要外部定义一个触发器
s32 PlateTrig(TrigDef *tr, s32 TarPos)
{
	static s32	LatestPos;//记录最新位置，用这个位置来做触发
	if(LT.PlateTrigTask.execute == TEXE)//当开启位置触发时 转盘位置触发
	{
		if(GSR.AxisEncoder[0] > LatestPos)//只有当转盘正转，才进行位置更新，防止电机抖动来回触发
		{
			LatestPos = GSR.AxisEncoder[0];
		}
	}
	else
	{
		TrigRst(tr);//重置触发器
		LatestPos = GSR.AxisEncoder[0];//关闭位置触发时，一直更新这个位置，这样复位清零的时候最新位置也清零了
	}
	if(TrigOne(tr,LatestPos%PPR_PLATE >= TarPos,0))
	{
		return 1;
	}
	return 0;
}

/**
* @author 21/03/05  yao
* @Description: 转盘数据流动任务
* @param --
* @return --
*/
//转盘数据流动，只考虑正向传送
void PlateDataMove()
{
	s32	Temp;
	static TrigDef tr;
	if(PlateTrig(&tr,50))//数据传递发生在预备位后50个脉冲
	{
		Temp = GUR.PlateLoopData.Data.OK[STATIONNUM-1];
		memmove(GUR.PlateLoopData.Data.OK+1,GUR.PlateLoopData.Data.OK,STATIONNUM*4);
		GUR.PlateLoopData.Data.OK[0] = Temp;

		Temp = GUR.PlateLoopData.Data.Elec[STATIONNUM-1];
		memmove(GUR.PlateLoopData.Data.Elec+1,GUR.PlateLoopData.Data.Elec,STATIONNUM*4);
		GUR.PlateLoopData.Data.Elec[0] = Temp;

		Temp = GUR.PlateLoopData.Data.Have[STATIONNUM-1];
		memmove(GUR.PlateLoopData.Data.Have+1,GUR.PlateLoopData.Data.Have,STATIONNUM*4);
		GUR.PlateLoopData.Data.Have[0] = Temp;
	}

//	if(Dir == -1)
//	{
//		Temp = GUR.PlateLoopData.Data.OK[0];
//		memmove(GUR.PlateLoopData.Data.OK,GUR.PlateLoopData.Data.OK+1,STATIONNUM*4);
//		GUR.PlateLoopData.Data.OK[STATIONNUM-1] = Temp;

//		Temp = GUR.PlateLoopData.Data.Elec[0];
//		memmove(GUR.PlateLoopData.Data.Elec,GUR.PlateLoopData.Data.Elec+1,STATIONNUM*4);
//		GUR.PlateLoopData.Data.Elec[STATIONNUM-1] = Temp;

//		Temp = GUR.PlateLoopData.Data.Have[0];
//		memmove(GUR.PlateLoopData.Data.Have,GUR.PlateLoopData.Data.Have+1,STATIONNUM*4);
//		GUR.PlateLoopData.Data.Have[STATIONNUM-1] = Temp;
//	}
}

/**
* @author 21/03/05  yao
* @Description: 转盘按指定方向走到指定一圈内坐标
* @param --	方向 一圈内脉冲数
* @return --
*/
//转盘按指定方向走到指定一圈内坐标
void Plate_Goto(s32 Dir, s32 TarPos)//Dir == -1:反  1:正
{
	if(TarPos > PPR_PLATE || AxSta(AxPlate) != AXSTA_READY)return;// 位置大于1000 或 轴状态不在准备状态
	s32	CurPos = GSR.AxisPosition[AxPlate] % PPR_PLATE;//保存当前周期内的位置
	if(Dir == -1)
	{
		if(CurPos > TarPos)
		{
			AxRelSpd(AxPlate,TarPos-CurPos,GUS.PlateData.Data.Spd_Slow);//目标 - 当前
		}
		else
		{
			AxRelSpd(AxPlate,TarPos-CurPos-PPR_PLATE,GUS.PlateData.Data.Spd_Slow);//目标 - 当前 - 1000
		}
	}
	if(Dir == 1)
	{
		if(CurPos > TarPos)
		{
			AxRelSpd(AxPlate,TarPos-CurPos+PPR_PLATE,GUS.PlateData.Data.Spd_Slow);//目标 - 当前 + 1000
		}
		else
		{
			AxRelSpd(AxPlate,TarPos-CurPos,GUS.PlateData.Data.Spd_Slow);//目标 - 当前
		}
	}
}

/**
* @author 21/03/05  yao
* @Description: 转盘运行控制，自动切换慢速、快速状态，接收外部立即停、慢速停指令
* @param --	
* @return --
*/
static void PlateRun()
{
	static u32 tm,tm_LackFast,tm_LackSlow;
	TASKRUN(LT.PlateRunTask)
	{
		case 1:
			Data.PlateRun.Mode = Slow;		//先设置模式为慢速
			Data.PlateRun.IntoFastNum = 0;	//进入快速的数量
			Data.PlateRun.IntoSlowNum = 0;	//进入慢速的数量
			TimerRst(&tm);					//复位tm
			GUW.DisplayData.Data.RunSpeed = GUS.PlateData.Data.Spd_Slow;	//运行速度 = 凸轮慢速
			AxRelSpd(AxPlate,0x7ffffff,GUS.PlateData.Data.Spd_Slow);		//轴配置一次
			my.step = 2;
			break;
		
		case 2:
			switch(Data.PlateRun.Mode)
			{
				case Slow:
					if(TimerCnt(&tm_LackFast,I_Get(I_FlatLack) == ON) > 1000)
//					if(Data.PlateRun.IntoFastNum >= GUS.PlateData.Data.Num_SwitchToFast && TimerCnt(&tm_LackFast,I_Get(I_FlatLack) == OFF) < 300)
						//快速的数量 > 设置的数量  并且  直振无料时间 < 300
					{
						Data.PlateRun.Mode = Fast;
						my.step = 3;
					}
					else if(GUW.DisplayData.Data.RunSpeed > GUS.PlateData.Data.Spd_Slow && TimerCnt(&tm,1) > 100)//100ms改一次速度，每次改2k/h
						//运行速度 > 慢速 并且  计时 > 100
					{
						Data.PlateRun.Mode = Slow;
						my.step = 3;
					}
					break;
				
				case Fast:
					if(TimerCnt(&tm_LackSlow,I_Get(I_FlatLack) == OFF) > 300)
//					if(Data.PlateRun.IntoSlowNum >= GUS.PlateData.Data.Num_SwitchToSlow || TimerCnt(&tm_LackSlow,I_Get(I_FlatLack) == OFF) > 300)
					//
					{
						Data.PlateRun.Mode = Slow;
						my.step = 3;
					}
					else if(GUW.DisplayData.Data.RunSpeed < GUS.PlateData.Data.Spd_Fast && TimerCnt(&tm,1) > 100)//100ms改一次速度，每次改2k/h
					//运行速度 < 凸轮快速速度 并且 计时100ms
					{
						Data.PlateRun.Mode = Fast;
						my.step = 3;
					}
					break;
				
				case StopToReady:
					HZ_AxStopDec(AxPlate);//慢速停止
					my.step = 4;
					break;
				
				case StopRightNow:
					HZ_AxStop(AxPlate);//快速停止
					my.step = 4;
					break;
			}
			break;
			
		case 3:
			switch(Data.PlateRun.Mode)
			{
				case Slow:
					TimerRst(&tm);
					GUW.DisplayData.Data.RunSpeed -= 2;
					if(GUW.DisplayData.Data.RunSpeed < GUS.PlateData.Data.Spd_Slow)
					{
						GUW.DisplayData.Data.RunSpeed = GUS.PlateData.Data.Spd_Slow;
					}
					HZ_AxReloadSpd(AxPlate,GUW.DisplayData.Data.RunSpeed*1000000/3600);
					my.step = 2;
					break;
				
				case Fast:
					TimerRst(&tm);
					GUW.DisplayData.Data.RunSpeed += 2;
					if(GUW.DisplayData.Data.RunSpeed > GUS.PlateData.Data.Spd_Fast)
					{
						GUW.DisplayData.Data.RunSpeed = GUS.PlateData.Data.Spd_Fast;
					}
					HZ_AxReloadSpd(AxPlate,GUW.DisplayData.Data.RunSpeed*1000000/3600);
					my.step = 2;
					break;
					
				default:
					my.step = 2;
					break;
			}
			break;
		
		case 4:
			switch(Data.PlateRun.Mode)
			{
				case StopToReady:
					if(AxSta(AxPlate) == AXSTA_READY)
					{
						Plate_Goto(1,0);
						my.step = 5;
					}
					break;
				
				case StopRightNow:
					if(AxSta(AxPlate) == AXSTA_READY)
					{
						TASKOK;
					}
					break;
					
				default:
					my.step = 2;
					break;
			}
			break;
			
		case 5:
			if(AxSta(AxPlate) == AXSTA_READY)
			{
				TASKOK;
			}
			break;
	}
}

/**
* @author 21/03/05  yao
* @Description: 转盘运行回零
* @param --	
* @return --
*/
//转盘复位
static void PlateHome()
{
	static u32 tm;
	TASKRUN(LT.PlateHomeTask)
	{
		case 1:
			AxHome(AxPlate);
			my.step = 2;
			break;
		
		case 2:
			if(AxSta(AxPlate) == AXSTA_READY)
			{
				AxAbsSpd(AxPlate,GUS.PlateData.Data.Pos_Ready,GUS.PlateData.Data.Spd_Slow);//去到预备位
				my.step = 3;
			}
			break;
			
		case 3:
			if(AxSta(AxPlate) == AXSTA_READY)//等待轴停止
			{
				TimerRst(&tm);
				my.step = 4;
			}
			break;
			
		case 4:
			if(TimerCnt(&tm,1) > 100)
			{
				EnCode_SetVal(0,0);//清空位置
				HZ_AxSetCurPos(AxPlate,0);//清空位置
				TASKOK;
			}
			break;
	}
}

void Plate()
{
	PlateDataMove();//转盘数据流动任务
	PlateRun();		//转盘运行
	PlateHome();	//回零
}


