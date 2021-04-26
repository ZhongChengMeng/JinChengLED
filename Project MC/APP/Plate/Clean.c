/*
*@Header:
*@File Name:	Clean.c
*@Author: Yao
*@Data: 2021/03/05
*@Description: 上料相关:
*					Clear()		转盘清料
*					Clean()		转盘吹气清洗
*/

#include "Logic.h"

/**
* @author 21/03/05  yao
* @Description: 转盘清料
* @param --  
* @return --
*/
//转盘清料
static void Clear()
{
	static TrigDef tr_Blow4Start,tr_Blow4End,tr_Blow3Start,tr_Blow3End;
	TASKRUN(LT.ClearTask)
	{
		case 1:
			if(LT.FeederFlatTask.execute == TIDLE)
			{
				LT.PlateTrigTask.execute = TSTOP;		//转盘位置触发
				AxRelSpd(AxPlate,PPR_PLATE * 12,GUS.PlateData.Data.Spd_Slow);//旋转12个工位
				TrigRst(&tr_Blow4Start);
				TrigRst(&tr_Blow4End);
				TrigRst(&tr_Blow3Start);
				TrigRst(&tr_Blow3End);
				my.step = 2;
			}
			break;
		
		case 2:
			if(AxSta(AxPlate) == AXSTA_READY)//转盘旋转完成
			{
				PARAINIT(GUR.PlateLoopData.Data);//清空所有工位的内容
				TASKOK;
			}
			else
			{
				//根据位置开启第三 、 四个吹气
				if(TrigOne(&tr_Blow4Start,GSR.AxisEncoder[0] % PPR_PLATE > GUS.PlateData.Data.Pos_Blow4Start,0))
				{
					LT.Blow4Task.execute = TEXE;
				}
				if(TrigOne(&tr_Blow4End,GSR.AxisEncoder[0] % PPR_PLATE > GUS.PlateData.Data.Pos_Blow4End,0))
				{
					LT.Blow4Task.execute = TSTOP;
				}
				if(TrigOne(&tr_Blow3Start,GSR.AxisEncoder[0] % PPR_PLATE > GUS.PlateData.Data.Pos_Blow3Start,0))
				{
					LT.Blow3Task.execute = TEXE;
				}
				if(TrigOne(&tr_Blow3End,GSR.AxisEncoder[0] % PPR_PLATE > GUS.PlateData.Data.Pos_Blow3End,0))
				{
					LT.Blow3Task.execute = TSTOP;
				}
			}
			break;
	}
}

/**
* @author 21/03/05  yao
* @Description: 转盘清料
* @param --  
* @return --
*/
//转盘清洗
void Clean()
{
	static u32 tm;
	Clear();
	TASKRUN(LT.CleanTask)
	{
		case 1:
			LT.ClearTask.execute = TEXE;		//启动转盘清料
			my.step = 2;
			break;
		
		case 2:
			if(LT.ClearTask.execute == TIDLE)	//等待转盘清料结束
			{
				Q_Set(Q_PlateClean,ON);			//吸嘴清理
				TimerRst(&tm);
				my.step = 3;
			}
			break;
			
		case 3:
			if(TimerCnt(&tm,1) > GUS.TimeData.Data.Tim_Clean)	//记录吸嘴吹气时间
			{
				Q_Set(Q_PlateClean,OFF);						//吸嘴清理关闭
				TASKOK;
			}
			break;
	}
}


