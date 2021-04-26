/*
*@Header:
*@File Name:	Feed.c
*@Author: Yao
*@Data: 2021/03/05
*@Description: �������:
*					FeedPlate()		ֱ��������������
*					FeederRound()	Բ��
*					FeederFlat()	ֱ��
*					FeederHopper()	�϶�
*					KeepFeed()		�ֶ������������
*					Feed()			�ⲿ��ѯ logic.c
*/

#include "Logic.h"
#include "Feed.h"
#include "Plate.h"

/**
* @author 21/03/05  yao
* @Description: ֱ��������������
* @param --  
* @return --
*/
static void FeedPlate()
{
	static TrigDef tr_Vacuo_ON[3],tr_Blow_ON,tr_InpStart;
	static TrigDef tr_Vacuo_OFF[3],tr_Blow_OFF,tr_InpEnd;
	static u32	Flag_Vacuo_ON[3],Flag_Vacuo_OFF[3],Flag_InpCheck;
	s32 Pos_Vacuo_ON[3] = {GUS.PlateData.Data.Pos_FeedVacuo1ON,GUS.PlateData.Data.Pos_FeedVacuo2ON,GUS.PlateData.Data.Pos_FeedVacuo3ON};
	s32 Pos_Vacuo_OFF[3] = {GUS.PlateData.Data.Pos_FeedVacuo1OFF,GUS.PlateData.Data.Pos_FeedVacuo2OFF,GUS.PlateData.Data.Pos_FeedVacuo3OFF};
	
	for(int i=0; i<3; i++)
	{
		if(PlateTrig(&tr_Vacuo_ON[i],Pos_Vacuo_ON[i]))
		{
			Flag_Vacuo_ON[i] = 1;
			Flag_Vacuo_OFF[i] = 0;
		}
	}
	for(int i=0; i<3; i++)
	{
		if(PlateTrig(&tr_Vacuo_OFF[i],Pos_Vacuo_OFF[i]))
		{
			Flag_Vacuo_ON[i] = 0;
			Flag_Vacuo_OFF[i] = 1;
		}
	}
	if(PlateTrig(&tr_InpStart,GUS.PlateData.Data.Pos_InpStart))
	{
		Flag_InpCheck = 1;
	}
	if(PlateTrig(&tr_InpEnd,GUS.PlateData.Data.Pos_InpEnd))
	{
		Flag_InpCheck = 0;
	}
	//���1��
	if(Flag_Vacuo_ON[0] == 1)
	{
		Flag_Vacuo_ON[0] = 0;
		Q_Set(Q_FeedVacuo1,ON);
	}
	//���1��
	if(Flag_Vacuo_OFF[0] == 1)
	{
		Flag_Vacuo_OFF[0] = 0;
		Q_Set(Q_FeedVacuo1,OFF);
		GUR.PlateLoopData.Data.Have[0] = 1;
		GUR.PlateLoopData.Data.OK[0] = 1;
		GUR.PlateLoopData.Data.Elec[0] = 0;
		if(I_Get(I_FlatInp) == ON)//���ϵ�λ
		{
			Data.PlateRun.IntoFastNum++;//ת�̿������л�����
			Data.PlateRun.IntoSlowNum = 0;
		}
		else
		{
			Data.PlateRun.IntoSlowNum++;//ת�̿������л�����
			Data.PlateRun.IntoFastNum = 0;
		}
	}
	//���2��
	if(Flag_Vacuo_ON[1] == 1)
	{
		Flag_Vacuo_ON[1] = 0;
		Q_Set(Q_FeedVacuo2,ON);
	}
	//���2��
	if(Flag_Vacuo_OFF[1] == 1)
	{
		Flag_Vacuo_OFF[1] = 0;
		Q_Set(Q_FeedVacuo2,OFF);
	}
	//���3��
	if(Flag_Vacuo_ON[2] == 1)
	{
		Flag_Vacuo_ON[2] = 1;
		Q_Set(Q_FeedVacuo3,ON);
	}
	//���3��
	if(Flag_Vacuo_OFF[2] == 1)
	{
		Flag_Vacuo_OFF[2] = 0;
		Q_Set(Q_FeedVacuo3,OFF);
	}
	//���봵����
	if(PlateTrig(&tr_Blow_ON,GUS.PlateData.Data.Pos_FeedBlowON))//���봵��λ�ô���
	{
		if(I_Get(I_FlatInp) == OFF)//����û��λ�ſ�
		{
			Q_Set(Q_FeedBlow,ON);
		}
	}
	//���봵����
	if(PlateTrig(&tr_Blow_OFF,GUS.PlateData.Data.Pos_FeedBlowOFF))//���봵��λ�ô���
	{
		Q_Set(Q_FeedBlow,OFF);
	}
	//��λ���
	if(Flag_InpCheck)
	{
		if(I_Get(I_FlatInp) == ON)
		{
			Flag_InpCheck = 0;
			Q_Set(Q_FeedVacuo1,ON);
			Q_Set(Q_FeedVacuo2,ON);
			Q_Set(Q_FeedVacuo3,ON);
		}
	}
}

/**
* @author 21/03/05  yao
* @Description: Բ��
* @param --  
* @return --
*/
//Բ��
static void FeederRound()
{
	TASKRUN(LT.FeederRoundTask)
	{
	
	}
}

/**
* @author 21/03/05  yao
* @Description: ֱ��
* @param --  
* @return --
*/
//ֱ��
static void FeederFlat()
{
	TASKRUN(LT.FeederFlatTask)
	{
	
	}
}

/**
* @author 21/03/05  yao
* @Description: �϶�
* @param --  
* @return --
*/
//�϶�
//static void FeederHopper()
//{
//	static u32 tm;
//	if(FSM_STA == RUN && I_Get(I_RoundLack) == ON && GUS.EnableData.Data.Hopper == 1)//����״̬ ���� Բ��ȱ�� �����ֶ������˹���
//	{
//		LT.FeederHopperTask.execute = TEXE;
//	}
//	TASKRUN(LT.FeederHopperTask)
//	{
//		case 1:
//			Q_Set(Q_FeederHopper,ON);	//�϶�����
//			TimerRst(&tm);
//			my.step = 2;
//			break;
//		
//		case 2:
//			if(TimerCnt(&tm,1) > GUS.TimeData.Data.Tim_Hopper)//�϶�������
//			{
//				Q_Set(Q_FeederHopper,OFF);	//�϶�����
//				TimerRst(&tm);
//				my.step = 3;
//			}
//			break;
//			
//		case 3:
//			if(TimerCnt(&tm,1) > GUS.TimeData.Data.Tim_Hopper)//�϶�������
//			{
//				TASKOK;
//			}
//			break;
//	}
//}
static void FeederHopper()
{
	s32 IO_RoundLack = GUS.TransWatch.Data.IO_Trans_Hopper;
	static u32 tm,tm_HopperKeep,tm_HopperTime;
	if(FSM_STA == RUN && I_Get(I_RoundLack) == IO_RoundLack && GUS.EnableData.Data.Hopper == 1)//����״̬ ���� Բ��ȱ�� �����ֶ������˹���
	{
		LT.FeederHopperTask.execute = TEXE;
	}
	TASKRUN(LT.FeederHopperTask)
	{
		case 1://��ʱ
			TimerRst(&tm_HopperKeep);
			my.step = 2;
			break;
		
		case 2:
			if(TimerCnt(&tm_HopperKeep,InPut_GetSta(I_RoundLack) == IO_RoundLack) > GUS.TimeData.Data.Tim_HopperKeep)//�϶����ź�ȷ��
			{
				TimerRst(&tm_HopperTime);	//�����ʱ
				TimerRst(&tm);				//��ռ�ʱ
				my.step = 3;
			}
			break;
			
		case 3:
			if(TimerCnt(&tm,1) > GUS.TimeData.Data.Tim_Hopper)//�϶�������
			{
				Q_Set(Q_FeederHopper,ON);	//�϶�����
				TimerRst(&tm);
				my.step = 4;
			}
			break;
		case 4:
			if(TimerCnt(&tm,1) > GUS.TimeData.Data.Tim_Hopper)//�϶�������
			{
				Q_Set(Q_FeederHopper,OFF);	//�϶�����
				TimerRst(&tm);
				my.step = 5;
			}
			break;
		case 5:
			if(TimerCnt(&tm_HopperTime,1) >= GUS.TimeData.Data.Tim_HopperTime || FSM_STA != RUN || I_Get(I_RoundLack) == (1-IO_RoundLack))
				//��ʱ�䵽�� || ״̬���������� || ��Ӧ��û�ź�
			{
				TimerRst(&tm);
				TASKOK;
			}
			else
			{
				TimerRst(&tm);
				my.step = 3;
			}
			break;
	}
}

/**
* @author 21/03/05  yao
* @Description: �ֶ������������
* @param --  
* @return --
*/
//�ֶ������������
void KeepFeed()
{
	static TrigDef Tr_ManualOn,Tr_ManualOff;
	if(FSM_STA != STOP)
	{
		GUW.ManualData.Data.KeepFeed = 0;
	}
	if(TrigOne(&Tr_ManualOn,GUW.ManualData.Data.KeepFeed == 1,0))//ֱ�� Բ�� ��λ��
	{
		Q_Set(Q_FeederFlat,ON);		//ֱ��
		Q_Set(Q_FeederRound,ON);	//Բ��
	}
	if(TrigOne(&Tr_ManualOff,GUW.ManualData.Data.KeepFeed == 0,0))//ֱ�� Բ�� ��λ��
	{
		Q_Set(Q_FeederFlat,OFF);	//ֱ���
		Q_Set(Q_FeederRound,OFF);   //Բ���
	}
}


void Feed()
{
	FeedPlate();
	FeederRound();
	FeederFlat();
	FeederHopper();
	KeepFeed();
}

