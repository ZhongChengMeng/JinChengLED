/*
*@Header:
*@File Name:	Blow.c
*@Author: Yao
*@Data: 2021/03/05
*@Description: ������λ���:
*					Blow1()	������λ1
*					Blow2()	������λ2
*					Blow3()	������λ3
*					Blow4()	������λ4 
*					Blow()	�ⲿ��ѯ Logic.c
*/

#include "Logic.h"
#include "Plate.h"

/**
* @author 21/03/05  yao
* @Description: ����1��λ ���Բ���NG����
* @param -- 
* @return --
*/
static void Blow1()//���Բ���NG����
{
	static u32 tm;
	static TrigDef tr_Start,tr_End;
	if(PlateTrig(&tr_Start,GUS.PlateData.Data.Pos_Blow1Start))//��ʼλ�ô���
	{
		LT.Blow1Task.execute = TEXE;
	}
	if(PlateTrig(&tr_End,GUS.PlateData.Data.Pos_Blow1End))//����λ�ô���
	{
		LT.Blow1Task.execute = TSTOP;
	}
	TASKRUN(LT.Blow1Task)
	{
		case 1:
			if((GUS.EnableData.Data.AgingMode == 0 && 								//�ϻ�ģʽ	0���������� 1���ϻ�����
				GUR.PlateLoopData.Data.Have[GUS.StationData.Data.Idx_Blow1] == 1 && //����
				GUR.PlateLoopData.Data.OK[GUS.StationData.Data.Idx_Blow1] != 1) ||	//��Ʒ
				my.Manual == 1)	//
			{
				GUR.PlateLoopData.Data.Have[GUS.StationData.Data.Idx_Blow1] = 0;
				GUS.RecordData.Data.Num_NgDiscard1++;	//NG���ϼ���
				if(GUS.RecordData.Data.Num_NgDiscard1 > GUS.AlarmData.Data.Num_DiscardFull && GUS.AlarmData.Data.Num_DiscardFull != 0)
				{
					AlarmSetBit(2,0,12);//���Բ�����Ͳ�������˹���������
				}
				Q_Set(Q_DiscardBlow1,ON);//��
				TimerRst(&tm);
				my.step= 2;
			}
			else
			{
				TASKOK;
			}
			break;
			
		case 2:
			if(my.execute == TSTOP || TimerCnt(&tm,1) >= GUS.TimeData.Data.Tim_Blow)//ʱ��  ����  ��������ͣ
			{
				Q_Set(Q_DiscardBlow1,OFF);//��
				TASKOK;
			}
			break;
	}
}

/**
* @author 21/03/05  yao
* @Description: ����2��λ ���ϴ���
* @param -- 
* @return --
*/
static void Blow2()//���ϴ���
{
	static u32 tm;
	static TrigDef tr_Start,tr_End;
	if(PlateTrig(&tr_Start,GUS.PlateData.Data.Pos_Blow2Start))//��ʼλ�ô���
	{
		LT.Blow2Task.execute = TEXE;
	}
	if(PlateTrig(&tr_End,GUS.PlateData.Data.Pos_Blow2End))//����λ�ô���
	{
		LT.Blow2Task.execute = TSTOP;
	}
	TASKRUN(LT.Blow2Task)
	{
		case 1:
			if((GUR.PlateLoopData.Data.Have[GUS.StationData.Data.Idx_Blow2] == 1 && //����
				GUR.PlateLoopData.Data.OK[GUS.StationData.Data.Idx_Blow2] != 1) ||	//����
				my.Manual == 1)
			{
				GUR.PlateLoopData.Data.Have[GUS.StationData.Data.Idx_Blow2] = 0;
				GUS.RecordData.Data.Num_OkDiscard1++;
				if(GUS.RecordData.Data.Num_OkDiscard1 > GUS.AlarmData.Data.Num_DiscardFull && GUS.AlarmData.Data.Num_DiscardFull != 0)
				{
					AlarmSetBit(2,0,13);//���ϴ�����Ͳ�������˹���������
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
			if(TimerCnt(&tm,1) > GUS.TimeData.Data.Tim_Blow || my.execute == TSTOP)//ʱ��  ����  ��������ͣ
			{
				Q_Set(Q_DiscardBlow2,OFF);
				TASKOK;
			}
			break;
	}
}

/**
* @author 21/03/05  yao
* @Description: ����3��λ ��βNG����
* @param -- 
* @return --
*/
static void Blow3()//��βNG����
{
	static u32 tm;
	static TrigDef tr_Start,tr_End;
	if(PlateTrig(&tr_Start,GUS.PlateData.Data.Pos_Blow3Start))//��ʼλ�ô���
	{
		LT.Blow3Task.execute = TEXE;
	}
	if(PlateTrig(&tr_End,GUS.PlateData.Data.Pos_Blow3End))//����λ�ô���
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
				GUS.RecordData.Data.Num_NgDiscard2++;//������Ͳ2
				if(GUS.RecordData.Data.Num_NgDiscard2 > GUS.AlarmData.Data.Num_DiscardFull && GUS.AlarmData.Data.Num_DiscardFull != 0)
				{
					AlarmSetBit(2,0,14);//ĩ�˲���Ʒ��Ͳ�������˹���������
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
			if(TimerCnt(&tm,1) > GUS.TimeData.Data.Tim_Blow || my.execute == TSTOP)//ʱ��  ����  ��������ͣ
			{
				Q_Set(Q_DiscardBlow3,OFF);
				TASKOK;
			}
			break;
	}
}

/**
* @author 21/03/05  yao
* @Description: ����4��λ ��β��Ʒ����
* @param -- 
* @return --
*/
static void Blow4()//��β��Ʒ����
{
	static u32 tm;
	static TrigDef tr_Start,tr_End;
	if(PlateTrig(&tr_Start,GUS.PlateData.Data.Pos_Blow4Start))//��ʼλ�ô���
	{
		LT.Blow4Task.execute = TEXE;
	}
	if(PlateTrig(&tr_End,GUS.PlateData.Data.Pos_Blow4End))//����λ�ô���
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
					if(GUS.RecordData.Data.Num_OkDiscard2 > GUS.AlarmData.Data.Num_DiscardFull && GUS.AlarmData.Data.Num_DiscardFull != 0)//��������ݴ�����
					{
						AlarmSetBit(2,0,15);//ĩ����Ʒ��Ͳ�������˹���������
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
			if(TimerCnt(&tm,1) > GUS.TimeData.Data.Tim_Blow || my.execute == TSTOP)//ʱ��  ����  ��������ͣ
			{
				Q_Set(Q_DiscardBlow4,OFF);
				TASKOK;
			}
			break;
	}
}

void Blow()
{
	Blow1();		//���Բ���NG����
	Blow2();		//���ϴ���
	Blow3();		//��βNG����
	Blow4();		//��β��Ʒ����
}

