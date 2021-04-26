/*
*@Header:
*@File Name:	Blow.c
*@Author: Yao
*@Data: 2021/03/05
*@Description: 吹气工位相关:
*					Blow1()	吹气工位1
*					Blow2()	吹气工位2
*					Blow3()	吹气工位3
*					Blow4()	吹气工位4 
*					Blow()	外部轮询 Logic.c
*/

#include "Logic.h"
#include "Plate.h"

/**
* @author 21/03/05  yao
* @Description: 吹气1工位 电性测试NG吹料
* @param -- 
* @return --
*/
static void Blow1()//电性测试NG吹料
{
	static u32 tm;
	static TrigDef tr_Start,tr_End;
	if(PlateTrig(&tr_Start,GUS.PlateData.Data.Pos_Blow1Start))//开始位置触发
	{
		LT.Blow1Task.execute = TEXE;
	}
	if(PlateTrig(&tr_End,GUS.PlateData.Data.Pos_Blow1End))//结束位置触发
	{
		LT.Blow1Task.execute = TSTOP;
	}
	TASKRUN(LT.Blow1Task)
	{
		case 1:
			if((GUS.EnableData.Data.AgingMode == 0 && 								//老化模式	0：正常生产 1：老化空跑
				GUR.PlateLoopData.Data.Have[GUS.StationData.Data.Idx_Blow1] == 1 && //有料
				GUR.PlateLoopData.Data.OK[GUS.StationData.Data.Idx_Blow1] != 1) ||	//良品
				my.Manual == 1)	//
			{
				GUR.PlateLoopData.Data.Have[GUS.StationData.Data.Idx_Blow1] = 0;
				GUS.RecordData.Data.Num_NgDiscard1++;	//NG吹料计数
				if(GUS.RecordData.Data.Num_NgDiscard1 > GUS.AlarmData.Data.Num_DiscardFull && GUS.AlarmData.Data.Num_DiscardFull != 0)
				{
					AlarmSetBit(2,0,12);//电性不良料筒满，请人工清理后继续
				}
				Q_Set(Q_DiscardBlow1,ON);//开
				TimerRst(&tm);
				my.step= 2;
			}
			else
			{
				TASKOK;
			}
			break;
			
		case 2:
			if(my.execute == TSTOP || TimerCnt(&tm,1) >= GUS.TimeData.Data.Tim_Blow)//时间  或者  别人让他停
			{
				Q_Set(Q_DiscardBlow1,OFF);//关
				TASKOK;
			}
			break;
	}
}

/**
* @author 21/03/05  yao
* @Description: 吹气2工位 补料吹料
* @param -- 
* @return --
*/
static void Blow2()//补料吹料
{
	static u32 tm;
	static TrigDef tr_Start,tr_End;
	if(PlateTrig(&tr_Start,GUS.PlateData.Data.Pos_Blow2Start))//开始位置触发
	{
		LT.Blow2Task.execute = TEXE;
	}
	if(PlateTrig(&tr_End,GUS.PlateData.Data.Pos_Blow2End))//结束位置触发
	{
		LT.Blow2Task.execute = TSTOP;
	}
	TASKRUN(LT.Blow2Task)
	{
		case 1:
			if((GUR.PlateLoopData.Data.Have[GUS.StationData.Data.Idx_Blow2] == 1 && //有料
				GUR.PlateLoopData.Data.OK[GUS.StationData.Data.Idx_Blow2] != 1) ||	//良料
				my.Manual == 1)
			{
				GUR.PlateLoopData.Data.Have[GUS.StationData.Data.Idx_Blow2] = 0;
				GUS.RecordData.Data.Num_OkDiscard1++;
				if(GUS.RecordData.Data.Num_OkDiscard1 > GUS.AlarmData.Data.Num_DiscardFull && GUS.AlarmData.Data.Num_DiscardFull != 0)
				{
					AlarmSetBit(2,0,13);//补料吹空料筒满，请人工清理后继续
				}
				Q_Set(Q_DiscardBlow2,ON);
				TimerRst(&tm);
				my.step = 2;
			}
			else
			{
				TASKOK;
			}
			break;
			
		case 2:
			if(TimerCnt(&tm,1) > GUS.TimeData.Data.Tim_Blow || my.execute == TSTOP)//时间  或者  别人让他停
			{
				Q_Set(Q_DiscardBlow2,OFF);
				TASKOK;
			}
			break;
	}
}

/**
* @author 21/03/05  yao
* @Description: 吹气3工位 结尾NG吹料
* @param -- 
* @return --
*/
static void Blow3()//结尾NG吹料
{
	static u32 tm;
	static TrigDef tr_Start,tr_End;
	if(PlateTrig(&tr_Start,GUS.PlateData.Data.Pos_Blow3Start))//开始位置触发
	{
		LT.Blow3Task.execute = TEXE;
	}
	if(PlateTrig(&tr_End,GUS.PlateData.Data.Pos_Blow3End))//结束位置触发
	{
		LT.Blow3Task.execute = TSTOP;
	}
	TASKRUN(LT.Blow3Task)
	{
		case 1:
			if((GUR.PlateLoopData.Data.Have[GUS.StationData.Data.Idx_Blow3] == 1 && 
				GUR.PlateLoopData.Data.OK[GUS.StationData.Data.Idx_Blow3] != 1) ||
				my.Manual == 1)
			{
				GUR.PlateLoopData.Data.Have[GUS.StationData.Data.Idx_Blow3] = 0;
				GUS.RecordData.Data.Num_NgDiscard2++;//不良料筒2
				if(GUS.RecordData.Data.Num_NgDiscard2 > GUS.AlarmData.Data.Num_DiscardFull && GUS.AlarmData.Data.Num_DiscardFull != 0)
				{
					AlarmSetBit(2,0,14);//末端不良品料筒满，请人工清理后继续
				}
				Q_Set(Q_DiscardBlow3,ON);
				TimerRst(&tm);
				my.step = 2;
			}
			else
			{
				TASKOK;
			}
			break;
			
		case 2:
			if(TimerCnt(&tm,1) > GUS.TimeData.Data.Tim_Blow || my.execute == TSTOP)//时间  或者  别人让他停
			{
				Q_Set(Q_DiscardBlow3,OFF);
				TASKOK;
			}
			break;
	}
}

/**
* @author 21/03/05  yao
* @Description: 吹气4工位 结尾良品吹料
* @param -- 
* @return --
*/
static void Blow4()//结尾良品吹料
{
	static u32 tm;
	static TrigDef tr_Start,tr_End;
	if(PlateTrig(&tr_Start,GUS.PlateData.Data.Pos_Blow4Start))//开始位置触发
	{
		LT.Blow4Task.execute = TEXE;
	}
	if(PlateTrig(&tr_End,GUS.PlateData.Data.Pos_Blow4End))//结束位置触发
	{
		LT.Blow4Task.execute = TSTOP;
	}
	TASKRUN(LT.Blow4Task)
	{
		case 1:
			if((GUR.PlateLoopData.Data.Have[GUS.StationData.Data.Idx_Blow4] == 1 && 
				GUR.PlateLoopData.Data.OK[GUS.StationData.Data.Idx_Blow4] == 1) ||
				LT.ClearTask.execute == TEXE || 
				my.Manual == 1)
			{
				if(GUR.PlateLoopData.Data.Have[GUS.StationData.Data.Idx_Blow4] == 1)
				{
					GUS.RecordData.Data.Num_OkDiscard2++;
					if(GUS.RecordData.Data.Num_OkDiscard2 > GUS.AlarmData.Data.Num_DiscardFull && GUS.AlarmData.Data.Num_DiscardFull != 0)//触摸屏快捷处清零
					{
						AlarmSetBit(2,0,15);//末端良品料筒满，请人工清理后继续
					}
				}
				GUR.PlateLoopData.Data.Have[GUS.StationData.Data.Idx_Blow4] = 0;
				Q_Set(Q_DiscardBlow4,ON);
				TimerRst(&tm);
				my.step = 2;
			}
			else
			{
				TASKOK;
			}
			break;
			
		case 2:
			if(TimerCnt(&tm,1) > GUS.TimeData.Data.Tim_Blow || my.execute == TSTOP)//时间  或者  别人让他停
			{
				Q_Set(Q_DiscardBlow4,OFF);
				TASKOK;
			}
			break;
	}
}

void Blow()
{
	Blow1();		//电性测试NG吹料
	Blow2();		//补料吹料
	Blow3();		//结尾NG吹料
	Blow4();		//结尾良品吹料
}

