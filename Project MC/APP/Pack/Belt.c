/*
*@Header:
*@File Name:	Belt.c
*@Author: Yao
*@Data: 2021/03/05
*@Description: 装填相机相关:
*					BeltMoveStart(u32 Num, s32 Dir, s32 Spd)	装填走带
*					BeltMove()		装填移动任务
*					BeltSupply()	编样品
*					BeltKeepMove()	载带送带电机任务
*					BeltHome()		装填电机回原点
*					Belt()			装填 外部轮询 Logic.c
*/

#include "Logic.h"
#include "Pack.h"

/**
* @author 21/03/05  yao
* @Description: 装填走带
* @param -- 数量 方向 速度
* @param --  
* @return --
*/
void BeltMoveStart(u32 Num, s32 Dir, s32 Spd) //Dir == -1:反向 1:正向
{
	Data.BeltMoveTemp.Num = Num;
	Data.BeltMoveTemp.Dir = Dir;
	Data.BeltMoveTemp.Spd = Spd;
	LT.BeltMoveTask.execute = TEXE;
}

/**
* @author 21/03/05  yao
* @Description: 装填移动任务
* @param --
* @task: BeltMoveTask
* @return --
*/
static void BeltMove()
{
	static u32 tm_Cilcle;
	TASKRUN(LT.BeltMoveTask)//装填移动任务
	{
		case 1:
			if(Data.BeltMoveTemp.Num == 0)
			{
				TASKOK;
				break;
			}
			TimerRst(&tm_Cilcle);
			AxRelSpd(AxBelt,Data.BeltMoveTemp.Dir*GUS.PackData.Data.Pos_Feed,Data.BeltMoveTemp.Spd);//装填轴一格脉冲
			my.step = 2;
			break;
		
		case 2:
			if(AxSta(AxBelt) == AXSTA_READY)
			{
				//GUR.TimeRecord.Data.PackMotorTime = TimerCnt(&tm_Cilcle,1);原先记录装填电机的时间
				if(my.Manual == 0)//只有在非手动状态下执行的内容
				{
					for(int i=0; i<10; i++)
					{
						if(GUS.RecordData.Data.PackStep[i])
							GUS.RecordData.Data.PackStep[i] += Data.BeltMoveTemp.Dir;
					}
				}
				GetPackedNum();
				GUW.ProcessData.Data.Num_NoBeltSupply += Data.BeltMoveTemp.Dir;
				if(GUW.ProcessData.Data.Num_NoBeltSupply > GUS.AlarmData.Data.Num_NoBeltSupply && GUS.AlarmData.Data.Num_NoBeltSupply != 0)
				{
					AlarmSetBit(2,0,16);//连续不送带数量到达，请检查料带是否用完
				}
				GUS.RecordData.Data.Num_PackBlet++;
				if(GUS.RecordData.Data.Num_PackBlet > GUS.AlarmData.Data.Num_NewBelt && GUS.AlarmData.Data.Num_NewBelt != 0)
				{
					AlarmSetBit(2,0,22);//料带使用已到达设置数量
				}
				GUS.RecordData.Data.Num_PackFilm++;
				if(GUS.RecordData.Data.Num_PackFilm > GUS.AlarmData.Data.Num_NewFilm && GUS.AlarmData.Data.Num_NewFilm != 0)
				{
					AlarmSetBit(2,0,23);//胶膜使用已到达设置数量
				}
				Data.BeltMoveTemp.Num--;
				if(Data.BeltMoveTemp.Num > 0)
					my.step = 1;
				else if(Data.BeltMoveTemp.Dir == -1)
				{
					AxRelSpd(AxBelt,-GUS.PackData.Data.Pos_Feed/2,Data.BeltMoveTemp.Spd);//用来消除间隙
					my.step = 3;
				}
				else
				{
					LT.CutTask.execute = TEXE;
					TASKOK;
				}
			}
			break;
			
		case 3:
			if(AxSta(AxBelt) == AXSTA_READY)
			{
				AxRelSpd(AxBelt,GUS.PackData.Data.Pos_Feed/2,Data.BeltMoveTemp.Spd);//用来消除间隙
				my.step = 4;
			}
			break;
			
		case 4:
			if(AxSta(AxBelt) == AXSTA_READY)
			{
				TASKOK;
			}
			break;
	}
}

/**
* @author 21/03/05  yao
* @Description: 载带送带电机任务
* @param --
* @task: BeltSupplyTask
* @return --
*/
static void BeltSupply()
{
	static u32 tm,tm_start;
	if(TimerCnt(&tm_start,I_Get(I_ChkBelt) == ON && AlarmGetBit(2,0,9) == 0) > 50)
	{
		LT.BeltSupplyTask.execute = TEXE;//送带启动
	}
	TASKRUN(LT.BeltSupplyTask)
	{
		case 1:
			GUW.ProcessData.Data.Num_NoBeltSupply = 0;	//清空计数
			Q_Set(Q_BeltSupply,ON);						//放带马达开启
			TimerRst(&tm);
			my.step = 2;
			break;
			
		case 2:
			if(TimerCnt(&tm,1) > GUS.TimeData.Data.Tim_Supply)
			{
				Q_Set(Q_BeltSupply,OFF);				//放带马达关闭
				TASKOK;
			}
			break;
	}
}

/**
* @author 21/03/05  yao
* @Description: 连续走带任务
* @param --
* @return --
*/
//连续走带
static void BeltKeepMove()
{
	if(FSM_STA != STOP)			//在非停止状态下禁止启动
	{
		GUW.ManualData.Data.KeepBeltMove = 0;
	}
	if(GUW.ManualData.Data.KeepBeltMove)
	{
		LT.BeltKeepMoveTask.execute = TEXE;
	}
	TASKRUN(LT.BeltKeepMoveTask)
	{
		case 1:
			BeltMoveStart(1,1,100);			//移动一格
			LT.BeltMoveTask.Manual = 1;		//方式为手动
			my.step = 2;
			break;
		
		case 2:
			if(LT.BeltMoveTask.execute == TIDLE)
			{
				LT.PressTask.execute = TEXE;	//封带启动
				LT.PressTask.Manual = 1;		//方式为手动
				my.step = 3;
			}
			break;
			
		case 3:
			if(LT.PressTask.execute == TIDLE)
			{
				TASKOK;
			}
			break;
	}
}

/**
* @author 21/03/05  yao
* @Description: 走带回零任务
* @param --
* @return --
*/
static void BeltHome()
{
	TASKRUN(LT.BeltHomeTask)//走带回零任务
	{
		case 1:
			AxHome(AxBelt);	//回零
			my.step = 2;
			break;
		
		case 2:
			if(AxSta(AxBelt) == AXSTA_READY)	//等待电机停止
			{
				AxAbs(AxBelt,GUS.PackData.Data.Pos_Ready);	//电机相对走到预备位
				my.step = 3;
			}
			break;
			
		case 3:
			if(AxSta(AxBelt) == AXSTA_READY)	//等待电机停止
			{
				TASKOK;
			}
			break;
	}
}

void Belt()
{
	BeltMove();		//装填电机移动
	BeltSupply();	//载带送带电机任务
	BeltKeepMove();	//连续走带
	BeltHome();		//装填电机回零
}

