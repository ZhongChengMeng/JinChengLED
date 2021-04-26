/*
*@Header:
*@File Name:	Adjust.c
*@Author: Yao
*@Data: 2021/03/05
*@Description: ���������:
*					Adjust_Half()	�������벽
*					Adjust_Full()	����������
*					AdjustHome()	��������λ
*					Adjust()		�������ⲿ��ѯ Logic.c
*/

#include "Logic.h"
#include "Plate.h"

u32 AxAdjust[2] = {AxAdjust1,AxAdjust2};

/**
* @author 21/03/05  yao
* @Description: �������벽
* @param -- 
* @return --
*/
static void Adjust_Half()
{
	for(int i=0; i<2; i++)
	{
		TASKRUN(LT.AdjustHalfTask[i])
		{
			case 1:
				AxRel(AxAdjust[i],PPR_ADJUST/8);	//1000 / 8
				my.step = 2;
				break;
			
			case 2:
				if(AxSta(AxAdjust[i]) == AXSTA_READY)
				{
					TASKOK;
				}
				break;
		}
	}
}

/**
* @author 21/03/05  yao
* @Description: ����������
* @param -- 
* @return --
*/
static void Adjust_Full()
{
	static u32 tm_Cilcle[2];
	static TrigDef tr_Start[2],tr_End[2];
	for(int i=0; i<2; i++)
	{
		//��ʼλ�ô���
		if(PlateTrig(&tr_Start[i],GUS.PlateData.Data.Pos_AdjustStart))
		{
			LT.AdjustFullTask[i].execute = TEXE;
		}
		//����λ�ô���
		if(PlateTrig(&tr_End[i],GUS.PlateData.Data.Pos_AdjustEnd))
		{
			if(LT.AdjustFullTask[i].execute == TEXE)
			{
				AlarmSetBit(2,0,5+i);//��λδ��ɣ��������λ����ٶȻ򽵵����������ٶ�
			}
		}
		TASKRUN(LT.AdjustFullTask[i])
		{
			case 1:
				TimerRst(&tm_Cilcle[i]);
				AxRel(AxAdjust[i],PPR_ADJUST/4);//1000 / 4
				my.step = 2;
				break;
			
			case 2:
				if(AxSta(AxAdjust[i]) == AXSTA_READY)
				{
					//GUR.TimeRecord.Data.AdjustTime[i] = TimerCnt(&tm_Cilcle[i],1);ԭ��ʹ�ö����ʱ��洢
					TASKOK;
				}
				break;
		}
	}
}

/**
* @author 21/03/05  yao
* @Description: ��������λ
* @param -- 
* @return --
*/
void AdjustHome()
{
	for(int i=0; i<2; i++)
	{
		TASKRUN(LT.AdjustHomeTask[i])
		{
			case 1:
				AxHome(AxAdjust[i]);
				my.step = 2;
				break;
			
			case 2:
				if(AxSta(AxAdjust[i]) == AXSTA_READY)
				{
					AxAbs(AxAdjust[i],GUS.AdjustData.Data.Pos_Ready[i]);
					my.step = 3;
				}
				break;
				
			case 3:
				if(AxSta(AxAdjust[i]) == AXSTA_READY)
				{
					TASKOK;
				}
				break;
		}
	}
}

void Adjust()
{
	AdjustHome();	//��λ
	Adjust_Half();	//�벽
	Adjust_Full();	//����
}

