/*
*@Header:
*@File Name:	Detect.c
*@Author: Yao
*@Data: 2021/03/05
*@Description: 转盘工位上感应相关:
*					Detect1()	装填前感应
*					Detect2()	装填后感应 感应2
*					Detect()	外部轮询 Logic.c
*/

#include "Logic.h"
#include "Plate.h"

/**
* @author 21/03/05  yao
* @Description: 装填前感应 感应1
* @param -- 
* @return --
*/
//装填前感应 感应1
static void Detect1()
{
	static u32 tm;
	static TrigDef tr_Start,tr_End;
	if(PlateTrig(&tr_Start,GUS.PlateData.Data.Pos_Detect1Start))
	{
		LT.Detect1Task.execute = TEXE;
	}
	if(PlateTrig(&tr_End,GUS.PlateData.Data.Pos_Detect1End))
	{
		LT.Detect1Task.execute = TSTOP;
	}
	TASKRUN(LT.Detect1Task)
	{
		case 1:
			TimerRst(&tm);
			if(GUS.EnableData.Data.AgingMode == 0)//正常生产模式情况下
			{
				GUR.PlateLoopData.Data.Have[GUS.StationData.Data.Idx_Detect1] = 0;//初始化为0
			}
			my.step = 2;
			break;
			
		case 2:
			if(my.execute == TSTOP)
			{
				TASKOK;
				break;
			}
			if(I_Get(I_DetectForPack) == ON)//感应有料
			{
				GUR.PlateLoopData.Data.Have[GUS.StationData.Data.Idx_Detect1] = 1;//说明有料
				TASKOK;
			}
			break;
	}
}

/**
* @author 21/03/05  yao
* @Description: 装填后感应 感应2
* @param -- 
* @return --
*/
//装填后感应 感应2
static void Detect2()
{
	static u32 tm;
	static TrigDef tr_Start,tr_End;
	if(PlateTrig(&tr_Start,GUS.PlateData.Data.Pos_Detect2Start))
	{
		LT.Detect2Task.execute = TEXE;
	}
	if(PlateTrig(&tr_End,GUS.PlateData.Data.Pos_Detect2End))
	{
		LT.Detect2Task.execute = TSTOP;
	}
	TASKRUN(LT.Detect2Task)
	{
		case 1:
			TimerRst(&tm);
			GUR.PlateLoopData.Data.Have[GUS.StationData.Data.Idx_Detect2] = 0;
			my.step = 2;
			break;
		
		case 2:
			if(my.execute == TSTOP)
			{
				TASKOK;
				break;
			}
			if(I_Get(I_DetectTakeOut) == ON)//感应有料
			{
				GUR.PlateLoopData.Data.Have[GUS.StationData.Data.Idx_Detect2] = 1;
				TASKOK;
			}
			break;
	}
}

void Detect()
{
	Detect1();	//装填前感应
	Detect2();	//装填后感应
}

