/*
*@Header:
*@File Name:	Cut.c
*@Author: Yao
*@Data: 2021/03/05
*@Description: 切带:
*					Cut()	切带 外部轮询 Logic.c
*/

#include "Logic.h"

/**
* @author 21/03/05  yao
* @Description: 切带
* @param --
* @param --  
* @return --
*/
void Cut()
{
	static u32 tm;
	int i,CutFlag;
	TASKRUN(LT.CutTask)
	{
		case 1:
			CutFlag = 0;
			for(i=0; i<10; i++)
			{
				if(GUS.RecordData.Data.PackStep[i] >= BeltTotalNum + GUS.PackData.Data.Idx_Cut)//编带步数记录 >= 总长度 + 切带位置
				{
					CutFlag = 1;
					GUS.RecordData.Data.PackStep[i] = 0;
				}
				if(GUS.RecordData.Data.PackStep[i] == GUS.PackData.Data.Idx_Cut)				//编带步数记录 == 切带位置
				{
					CutFlag = 1;
				}
			}
			if(CutFlag == 1 || my.Manual == 1)//或者手动
			{
				Q_Set(Q_CutCyd,ON);
				TimerRst(&tm);
				my.step = 2;
			}
			else
			{
				TASKOK;
			}
			break;
		
		case 2:
			if(TimerCnt(&tm,1) > 50)
			{
				GUS.RecordData.Data.Num_FinishReel++;// 已编卷数
				if(GUS.RecordData.Data.Num_FinishReel >= GUS.ProductData.Data.Num_TargetReel && GUS.ProductData.Data.Num_TargetReel != 0)
					//已编卷数 >= 目标完卷数量 并且 目标卷数不为 0 
				{
					AlarmSetBit(2,0,23);//目标卷数已到达
				}
				if(GUS.EnableData.Data.FinishStop == 1)//单卷停机
				{
					AlarmSetBit(1,0,0);//单卷完成
					FSM_CMD = STOP;
				}
				Q_Set(Q_CutCyd,OFF);//切带气缸
				Q_Set(Q_BeltCollect,OFF);//关闭收带马达
				TASKOK;
			}
			break;
	}
}

