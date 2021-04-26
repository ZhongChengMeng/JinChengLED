/*
*@Header:
*@File Name:	Press.c
*@Author: Yao
*@Data: 2021/03/05
*@Description: 切带:
*					Press()	封带 外部轮询 Logic.c
*/

#include "Logic.h"

/**
* @author 21/03/05  yao
* @Description: 封带
* @param --
* @param --  
* @return --
*/
//封带
void Press()
{
	static u32 tm,tm_Cilcle;
	int i;
	TASKRUN(LT.PressTask)
	{
		case 1:
			for(i=0; i<10; i++)
			{
				if(GUS.RecordData.Data.PackStep[i])
				{
					if//判断当前步是否需要封带
					(
						GUS.RecordData.Data.PackStep[i] < GUS.PackData.Data.Idx_Press ||
						(GUS.RecordData.Data.PackStep[i] >= GUS.PackData.Data.Idx_Press + GUS.PackData.Data.Num_Press + GUS.ProductData.Data.Num_NoPress &&
						GUS.RecordData.Data.PackStep[i] <= BeltTotalNum)
					)
					{
						break;
					}
				}
			}
			if(i < 10 || my.Manual)//手动模式为1
			{
				if(Q_Get(Q_PressUpCyd) == OFF)//封带上气缸
				{
					TimerRst(&tm);
					Q_Set(Q_PressUpCyd,ON);//封带上气缸
					my.step =  2;
				}
				else
				{
					TimerRst(&tm_Cilcle);
					Q_Set(Q_PressDownCyd,ON);//封带下气缸
					TimerRst(&tm);
					my.step = 3;
				}
			}
			else
			{
				TASKOK;
			}
			break;
			
		case 2:
			if(TimerCnt(&tm,1) > 200)
			{
				Q_Set(Q_PressDownCyd,ON);//封带下气缸
				TimerRst(&tm);
				my.step = 3;
			}
			break;
			
		case 3:
			if(TimerCnt(&tm,1) >= GUS.TimeData.Data.Tim_PressDown)//封带下压时间
			{
				Q_Set(Q_PressDownCyd,OFF);//封带下气缸
				TimerRst(&tm);
				my.step = 4;
			}
			break;
			
		case 4:
			if(TimerCnt(&tm,1) >= GUS.TimeData.Data.Tim_PressUp)//封带抬起时间
			{
				TimerRst(&tm);
				//GUR.TimeRecord.Data.PackPressTime = TimerCnt(&tm_Cilcle,1);原来使用定义的
				my.execute = TIDLE;
				my.done = 1;
				my.step = 5;
			}
			break;
			
		case 5:
			if(my.execute == TEXE)//有人召唤
			{
				my.step = 1;
				break;
			}
			if(TimerCnt(&tm,1) > 1000 && FSM_STA != RUN)//超过1000ms 无人召唤 或者  设备状态不在运行中
			{
				Q_Set(Q_PressUpCyd,OFF);
				TASKOK;
			}
			break;
	}
}


