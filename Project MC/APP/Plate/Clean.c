/*
*@Header:
*@File Name:	Clean.c
*@Author: Yao
*@Data: 2021/03/05
*@Description: �������:
*					Clear()		ת������
*					Clean()		ת�̴�����ϴ
*/

#include "Logic.h"

/**
* @author 21/03/05  yao
* @Description: ת������
* @param --  
* @return --
*/
//ת������
static void Clear()
{
	static TrigDef tr_Blow4Start,tr_Blow4End,tr_Blow3Start,tr_Blow3End;
	TASKRUN(LT.ClearTask)
	{
		case 1:
			if(LT.FeederFlatTask.execute == TIDLE)
			{
				LT.PlateTrigTask.execute = TSTOP;		//ת��λ�ô���
				AxRelSpd(AxPlate,PPR_PLATE * 12,GUS.PlateData.Data.Spd_Slow);//��ת12����λ
				TrigRst(&tr_Blow4Start);
				TrigRst(&tr_Blow4End);
				TrigRst(&tr_Blow3Start);
				TrigRst(&tr_Blow3End);
				my.step = 2;
			}
			break;
		
		case 2:
			if(AxSta(AxPlate) == AXSTA_READY)//ת����ת���
			{
				PARAINIT(GUR.PlateLoopData.Data);//������й�λ������
				TASKOK;
			}
			else
			{
				//����λ�ÿ������� �� �ĸ�����
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
* @Description: ת������
* @param --  
* @return --
*/
//ת����ϴ
void Clean()
{
	static u32 tm;
	Clear();
	TASKRUN(LT.CleanTask)
	{
		case 1:
			LT.ClearTask.execute = TEXE;		//����ת������
			my.step = 2;
			break;
		
		case 2:
			if(LT.ClearTask.execute == TIDLE)	//�ȴ�ת�����Ͻ���
			{
				Q_Set(Q_PlateClean,ON);			//��������
				TimerRst(&tm);
				my.step = 3;
			}
			break;
			
		case 3:
			if(TimerCnt(&tm,1) > GUS.TimeData.Data.Tim_Clean)	//��¼���촵��ʱ��
			{
				Q_Set(Q_PlateClean,OFF);						//��������ر�
				TASKOK;
			}
			break;
	}
}


