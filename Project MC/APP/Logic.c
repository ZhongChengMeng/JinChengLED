 /*
 * File: Logic.c
 * File Created: Tuesday, 9th October 2018 2:43:29 pm
 * version: v00.00.01 8��ײ�
 * Description:�û��߼�
 * Modified By:yang
 * -----
 * Copyright 2018 - 2018, <<hzzh>>
 */

#include "Logic.h"
#include "Plate.h"
#include "Adjust.h"
#include "Blow.h"
#include "Detect.h"
#include "DownCamera.h"
#include "ElecTest.h"
#include "ElecTurn.h"
#include "Feed.h"
#include "Clean.h"
#include "Belt.h"
#include "Cut.h"
#include "Film.h"
#include "Pack.h"
#include "Press.h"
#include "Repack.h"
#include "UpCamera.h"

#define PI 3.1415926

/*ȫ�ֱ�����ͳһ����*/
Task LogicTask; //ͳһ��������
SysData Data;  //ͳһ��������

//������
void AxisDataConfig()
{
	for(int i=0; i<PULS_NUM; i++)
	{
		GSS.axis[i].AxSpd.acctime = GUS.MotorData.Data.AccDecTime[i];//����ʱ��
		GSS.axis[i].AxSpd.dectime = GUS.MotorData.Data.AccDecTime[i];//����ʱ��
		GSS.axis[i].Axhomecfg.orglev = GUS.MotorData.Data.OriginDir[i];//ԭ���źŵ�ƽ
		GSS.axis[i].Axhomecfg.homespeedfast = GUS.MotorData.Data.HomeSpeed[i];//��ԭ�����
		if(GSW.JogForward[0] == 0 && GSW.JogBackward[0] == 0)//û�е㶯ʱ������Ϊ�����ٶ�
		{
			GSS.axis[i].AxSpd.startspeed = 1000;
			GSS.axis[i].AxSpd.endspeed = 1000;
			GSS.axis[i].AxSpd.runspeed = GUS.MotorData.Data.RunSpeed[i];
		}
		else//�е㶯ʱ������Ϊ�㶯�ٶ�
		{
			GSS.axis[i].AxSpd.startspeed = 1;
			GSS.axis[i].AxSpd.endspeed = 1;
			GSS.axis[i].AxSpd.runspeed = GUS.MotorData.Data.JogSpeed[i];
		}
		AxDirCfg(i,GUS.MotorData.Data.MotorDir[i]);
	}
	GUS.MotorData.Data.RunSpeed[AxPlate] = 100000*PPR_PLATE/3600;//ת�������ٶȹ̶�100kph�������ط��õ�ʱ�������ٶȱ�������������Kp/h
	AxisConfig(GSS.axis);
}

//�豸��ť
void Button()
{
	static TrigDef Tr_Run,Tr_Stop,Tr_ScramBtn;
	static u32 Tm_Scram;
	if(TrigOne(&Tr_Run,I_Get(I_StartBtn) == ON,50))//��ʼ��ť
	{
		FSM_CMD = RUN;
	}
	if(TrigOne(&Tr_Stop,I_Get(I_StopBtn) == ON,50))//ֹͣ��ť
	{
		GSW.ClearAlarm = 1;
		FSM_CMD = STOP;
	}
	if(TimerCnt(&Tm_Scram,I_Get(I_ScramBtn) == OFF) > 50)//��ͣ����
	{
		FSM_CMD = SCRAM;
	}
	if(TrigOne(&Tr_ScramBtn,I_Get(I_ScramBtn) == ON,50))//��ͣ����
	{
		InitFsm(&SysFsm);
	}
	
}

//��ɢ����
void Alarm()
{
	static u32 tm_ChkBelt,tm_BeltCollect;
	if(TimerCnt(&tm_ChkBelt,I_Get(I_ChkBelt) == ON) > 3000)
	{
		AlarmSetBit(2,0,9);//�ش���Ӧ�������źţ��ش����꣬���ش�����
	}
	
	if(FSM_STA == RUN)
	{
		static u32 tm_ChkFilm;
		if(TimerCnt(&tm_ChkFilm,I_Get(I_ChkFilm) == ON) > 200)
		{
			AlarmSetBit(2,0,24);//��Ĥ�����źŸ�Ӧ
		}
		
		static u32 tm_ChkPack;
		if(TimerCnt(&tm_ChkPack,I_Get(I_ChkPack) == OFF) > 60)
		{
			AlarmSetBit(2,0,25);//װ���
		}
		//�մ����δ����
		if(GUS.RecordData.Data.Num_Packed > GUW.ProcessData.Data.Num_BeltCollect && \
			GUW.ProcessData.Data.Num_BeltCollect != 0)
		{
			if(TimerCnt(&tm_BeltCollect,Q_Get(Q_BeltCollect) == OFF) > 0)
			{
				AlarmSetBit(2,0,27);//�մ����δ����
			}
		}
		static u32 tm_TempErr;
		if(TimerCnt(&tm_TempErr,I_Get(I_TempErr) == ON) > 0)
		{
			AlarmSetBit(2,0,28);//�¿��쳣
		}
	}
}

//״ָ̬ʾ�ƺͱ�����
void Indicator()
{
	static u32 tm;
	static TrigDef tr_Run,tr_Stop;
	if(TrigOne(&tr_Run,FSM_STA == RUN,0))//����״̬ʱ
	{
		Q_Set(Q_Green,ON);//�̵���
		Q_Set(Q_Yellow,OFF);//�Ƶ���
	}
	if(TrigOne(&tr_Stop,FSM_STA != RUN,0))//������״̬ʱ
	{
		Q_Set(Q_Green,OFF);//�̵���
		Q_Set(Q_Yellow,ON);//�Ƶ���
	}
	if(GSR.ErrorLevel && GUR.FsmData.Data.HaveToReset == 0)//�б���ʱ
	{
		LT.IndicatorTask.execute = TEXE;//ִ�б�����˸����
	}
	TASKRUN(LT.IndicatorTask)
	{
		case 1:
			Q_Set(Q_Red,ON);//�����
			if(GUS.EnableData.Data.Beep == 1)//�������������
			{
				Q_Set(Q_Beep,ON);//��������
			}
			TimerRst(&tm);
			my.step = 2;
			break;
			
		case 2:
			if(TimerCnt(&tm,1) > 300)//300ms��
			{
				Q_Set(Q_Red,OFF);//����
				Q_Set(Q_Beep,OFF);//ͣ������
				TimerRst(&tm);
				my.step = 3;
			}
			break;
			
		case 3:
			if(TimerCnt(&tm,1) > 300)
			{
				TASKOK;
			}
			break;
	}
}

//����ǰ׼������
void BeforeRun()
{
	static u32 tm;
	TASKRUN(LT.BeforeRunTask)
	{
		case 1:
			Q_Set(Q_PressUpCyd,ON);//��������ѹ
			Q_Set(Q_FeederFlat,ON);//ֱ����
			Q_Set(Q_FeederRound,ON);//Բ����
			TimerRst(&tm);
			my.step = 2;
			break;
		
		case 2:
			if(TimerCnt(&tm,1) > 500)//500ms��
			{
				LT.UpCameraTask.execute = TEXE;//�����ִ��
				my.step = 3;
			}
			break;
			
		case 3:
			if(LT.UpCameraTask.execute == TIDLE)//�ȴ����������
			{
				if(LT.UpCameraTask.done == 1)//�������ok
				{
					TASKOK;
				}
				else
				{
					if(GUS.EnableData.Data.RePack == 1)//���ʹ�ܲ���
					{
						LT.PlateTrigTask.execute = TEXE;//����ת�̴���
						LT.RepackTask.execute = TEXE;//����
						my.step = 4;
					}
					else
					{
						AlarmSetBit(2,0,11);//װ���Ӿ�NG��δ�����Զ����ϣ����˹�����
						TASKNG;
					}
				}
			}
			break;
			
		case 4:
			if(LT.RepackTask.execute == 0)//�ȴ����Ͻ���
			{
				if(LT.RepackTask.done == 1)//���ϳɹ�
				{
					TASKOK;
				}
				else//����ʧ��
				{
					TASKNG;
				}
			}
			break;
	}
}

void FileNameRead()
{
	
	
}
//ֹͣ����
void AfterStop()
{
	static TrigDef tr;
	if(TrigOne(&tr,FSM_STA != RUN,1000))//����ֹͣ��1s��
	{
		LT.AfterStopTask.execute = TEXE;
	}
	TASKRUN(LT.AfterStopTask)
	{
		case 1:
			Q_Set(Q_PressUpCyd,OFF);//�������������
			Q_Set(Q_FeederFlat,OFF);//�ر�����
			Q_Set(Q_FeederRound,OFF);//�ر�Բ��
			my.execute = TASKOK;
			break;
	}
}

//�豸������
void MainProcess()
{
	u32 i;
	if(FSM_STA == RUN)
	{
		LT.MainProcessTask.execute = TEXE;
	}
	TASKRUN(LT.MainProcessTask)
	{
		case 1:
			for(i=0; i<10; i++)
			{
				//������ϴ��ߵ�
				if(GUS.RecordData.Data.PackStep[i] > 0 && GUS.RecordData.Data.PackStep[i] < BeltTotalNum)
				{
					break;
				}
			}
			if(i >= 10)//û�����ڽ��е��ϴ�
			{
				LT.PackBeforTask.execute = TEXE;//��ǰ�࣬ǰ��ʱ����¾�����
			}
			my.step = 2;
			break;
		
		case 2:
			if(LT.PackBeforTask.execute == TIDLE)
			{
				LT.BeforeRunTask.execute = TEXE;//����ǰ����
				my.step = 3;
			}
			break;
		
		case 3:
			if(LT.BeforeRunTask.execute == TIDLE && FSM_STA == RUN)
			{
				if(LT.BeforeRunTask.done == 1)//����ǰ׼�����
				{
					LT.PlateTrigTask.execute = TEXE;//����ת�̴���
					LT.PlateRunTask.execute = TEXE;//ת�̿��ƿ�ʼ
					my.step = 4;
				}
				else//����ǰ׼���쳣
				{
					TASKNG;
				}
			}
			break;
		
		case 4:
			//�ж�ֹͣ������ͣ����װ������
			if((FSM_STA != RUN || GUS.RecordData.Data.Num_Packed >= (GUS.ProductData.Data.Num_Pack-1)) && LT.PackTask.execute != TEXE)
			{			
				Data.PlateRun.Mode = StopToReady;//ת��ͣ��Ԥ��λ
				my.step = 5;
			}
			break;
			
		case 5:
			if(LT.PlateRunTask.execute == TIDLE)//ת�̿��ƽ���
			{
				LT.PlateTrigTask.execute = TIDLE;//�ر�ת�̴���
				if(GUS.RecordData.Data.Num_Packed >= GUS.ProductData.Data.Num_Pack)//�ж�����
				{
					my.step = 6;
				}
				else
				{
					TASKOK;
				}
			}
			break;
			
		case 6:
			if(FSM_STA == RUN)
			{
				LT.PackBehindTask.execute = TEXE;//���
				my.step = 7;
			}
			break;
			
		case 7:
			if(my.execute == TSTOP)//���ʱ�ӵ�ֹͣ����
			{
				LT.PackBehindTask.execute = TSTOP;//������ද��
				my.step = 11;//������������
				break;
			}
			if(LT.PackBehindTask.execute == TIDLE)//�����������
			{
				LT.PackSampleTask.execute = TEXE;//����Ʒ
				my.step = 8;
			}
			break;
			
		case 8:
			if(my.execute == TSTOP)//����Ʒʱ�յ�ֹͣ����
			{
				LT.PackSampleTask.execute = TSTOP;//������Ʒ����
				my.step = 11;//������������
				break;
			}
			if(LT.PackSampleTask.execute == TIDLE)//��Ʒ��������
			{
				LT.PackSampleBehindTask.execute = TEXE;//��Ʒ���
				my.step = 9;
			}
			break;
			
		case 9:
			if(my.execute == TSTOP)//��Ʒ���ʱ�յ�ֹͣ����
			{
				LT.PackSampleBehindTask.execute = TSTOP;//������Ʒ��ද��
				my.step = 11;
				break;
			}
			if(LT.PackSampleBehindTask.execute == TIDLE)//��Ʒ�����������
			{
				LT.CleanTask.execute = TEXE;//��������
				my.step = 10;
			}
			break;
			
		case 10:
			if(LT.CleanTask.execute == TIDLE)//���������������
			{
				TASKOK;//һ���������
			}
			break;
			
		case 11:
			if( LT.PackBehindTask.execute == TIDLE &&
				LT.PackSampleTask.execute == TIDLE &&
				LT.PackSampleBehindTask.execute == TIDLE)
			{
				TASKNG;
			}
			break;
	}
}

//��ȡ����ʱ��
void TaskTime()
{
	GUR.TimeRecord.Data.AdjustTime[0] = LT.AdjustFullTask[0].Exetime;//����1ʱ��
	GUR.TimeRecord.Data.AdjustTime[1] = LT.AdjustFullTask[1].Exetime;//����2ʱ��
	GUR.TimeRecord.Data.DownCameraTime = LT.DownCameraTask.Exetime;//������Ӿ����ʱ��
	GUR.TimeRecord.Data.ElecTest1Time = LT.ElecTestTask.Exetime;//����1ʱ��		����1����ʱ��
	GUR.TimeRecord.Data.ElecTest2Time = LT.ElecTest2Task.Exetime;//����1ʱ��		����1����ʱ��
	GUR.TimeRecord.Data.ElecTurn90Time = LT.ElecTurnTask.Exetime;//��ת90ʱ��		��ת��λ90��ʱ��
	GUR.TimeRecord.Data.PackMotorTime = LT.BeltMoveTask.Exetime;//װ����ʱ��	 װ����һ��
	GUR.TimeRecord.Data.PackPressTime = LT.PressTask.Exetime;//װ����ʱ��	 �������
	GUR.TimeRecord.Data.UpCameraTime = LT.UpCameraTask.Exetime;//װ��������ʱ�� �������
	static TrigDef tr_PlateDownStart,tr_PlateDownEnd;
	static u32 tm_PlateDown;
	if(PlateTrig(&tr_PlateDownStart,GUS.PlateData.Data.Pos_DownCameraStart))
	{
		TimerRst(&tm_PlateDown);
	}
	if(PlateTrig(&tr_PlateDownEnd,GUS.PlateData.Data.Pos_DownCameraEnd))
	{
		GUR.TimeRecord.Data.PlateDownTime = TimerCnt(&tm_PlateDown,1);
	}//��¼תһȦ��ʱ��
	
	GUS.ProductData.Data.Num_ReelPack = GUS.ProductData.Data.Num_BeforPack + GUS.ProductData.Data.Num_BehindPack + GUS.ProductData.Data.Num_NoPress + \
										GUS.ProductData.Data.Num_Pack + GUS.ProductData.Data.Num_Sample + GUS.ProductData.Data.Num_SampleBehind;
}
//2020.7.20
//�豸�߼�ѭ��
void Logic()
{
	
	AxisDataConfig();		//���������
	Button();				//��ť����
	Alarm();				//���󱨾�
	Indicator();			//�� ��  ������
	TaskTime();				//ʱ���¼
	
	BeforeRun();			//����ǰ����
	AfterStop();			//ֹͣ����
	MainProcess();			//������	
	
	/**** ת�̹�λ��� ****/
	Plate();				//ת��
	Adjust();				//����
	Blow();					//����
	Detect();				//ת�̹�λ��Ӧ��
	DownCamera();			//��������
	ElecTesk();				//���Բ���
	ElecTurn();				//������ת
	Feed();					//�϶� Բ�� ֱ��
	Clean();				//ת�����
	
	/**** ������ ****/		
	Belt();					//�ϴ�
	Cut();					//�д�
	Pack();					//װ��
	Press();				//���
	Repack();				//����
	UpCamera();				//��������
	
	/****�ļ����****/
	File_operator();		//��ȡU���ļ�
}

