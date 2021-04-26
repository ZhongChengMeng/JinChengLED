/*
*@Header:
*@File Name:	DownCamera.c
*@Author: Yao
*@Data: 2021/03/05
*@Description: �ײ��Ӿ�:
*					DownCamera()	�ײ��Ӿ�
*/

#include "Logic.h"
#include "Plate.h"

/**
* @author 21/03/05  yao
* @Description: �ײ��Ӿ�
* @param -- 
* @return --
*/
//�ײ��Ӿ�
void DownCamera()
{
	static u32 tm,tm_Circle;
	static TrigDef tr_Eot,tr_Start;
	if(PlateTrig(&tr_Start,GUS.PlateData.Data.Pos_DownCameraStart))
	{
		if(LT.DownCameraTask.execute == TEXE)
		{
			LT.DownCameraTask.execute = TSTOP;
		}
		else
		{
			LT.DownCameraTask.execute = TEXE;
		}
	}
	TASKRUN(LT.DownCameraTask)
	{
		case 1:
			if(GUS.EnableData.Data.DownCamera == 0 || GUS.EnableData.Data.AgingMode == 1)
			{
				TASKOK;
			}
			else if(GUR.PlateLoopData.Data.Have[GUS.StationData.Data.Idx_DownCamera] == 1 && 
					GUR.PlateLoopData.Data.OK[GUS.StationData.Data.Idx_DownCamera] == 1)
			{
				Q_Set(Q_DownCameraSot,ON);
				TimerRst(&tm);
				TimerRst(&tm_Circle);
				my.step = 2;
			}
			else
			{
				TASKOK;
			}
			break;
			
		case 2:
			if(TimerCnt(&tm,1) < 5)break;
			Q_Set(Q_DownCameraSot,OFF);
			TrigRst(&tr_Eot);
			my.step = 3;
			break;
		
		case 3:
			if(my.Manual)
			{
				TASKNG;
			}
			else if(my.execute == TSTOP)
			{
				AlarmSetBit(2,0,8);//�ײ��Ӿ�δ��ɣ�������Ӿ��������򽵵����������ٶ�
				GUR.PlateLoopData.Data.OK[GUS.StationData.Data.Idx_DownCamera] = 0;
				GUR.PlateLoopData.Data.OK[GUS.StationData.Data.Idx_DownCamera+1] = 0;
				TASKNG;
				break;
			}
			else if(TrigOne(&tr_Eot,I_Get(I_DownCameraEot) == ON,2))
			{
				//GUR.TimeRecord.Data.DownCameraTime = TimerCnt(&tm_Circle,1);
				if(I_Get(I_DownCameraOk) == OFF)
				{
					GUR.PlateLoopData.Data.OK[GUS.StationData.Data.Idx_DownCamera] = 0;
				}
				TASKOK;
			}
			break;
	}
}

