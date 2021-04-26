/*
*@Header:
*@File Name:	Belt.c
*@Author: Yao
*@Data: 2021/03/05
*@Description: װ��������:
*					BeltMoveStart(u32 Num, s32 Dir, s32 Spd)	װ���ߴ�
*					BeltMove()		װ���ƶ�����
*					BeltSupply()	����Ʒ
*					BeltKeepMove()	�ش��ʹ��������
*					BeltHome()		װ������ԭ��
*					Belt()			װ�� �ⲿ��ѯ Logic.c
*/

#include "Logic.h"
#include "Pack.h"

/**
* @author 21/03/05  yao
* @Description: װ���ߴ�
* @param -- ���� ���� �ٶ�
* @param --  
* @return --
*/
void BeltMoveStart(u32 Num, s32 Dir, s32 Spd) //Dir == -1:���� 1:����
{
	Data.BeltMoveTemp.Num = Num;
	Data.BeltMoveTemp.Dir = Dir;
	Data.BeltMoveTemp.Spd = Spd;
	LT.BeltMoveTask.execute = TEXE;
}

/**
* @author 21/03/05  yao
* @Description: װ���ƶ�����
* @param --
* @task: BeltMoveTask
* @return --
*/
static void BeltMove()
{
	static u32 tm_Cilcle;
	TASKRUN(LT.BeltMoveTask)//װ���ƶ�����
	{
		case 1:
			if(Data.BeltMoveTemp.Num == 0)
			{
				TASKOK;
				break;
			}
			TimerRst(&tm_Cilcle);
			AxRelSpd(AxBelt,Data.BeltMoveTemp.Dir*GUS.PackData.Data.Pos_Feed,Data.BeltMoveTemp.Spd);//װ����һ������
			my.step = 2;
			break;
		
		case 2:
			if(AxSta(AxBelt) == AXSTA_READY)
			{
				//GUR.TimeRecord.Data.PackMotorTime = TimerCnt(&tm_Cilcle,1);ԭ�ȼ�¼װ������ʱ��
				if(my.Manual == 0)//ֻ���ڷ��ֶ�״̬��ִ�е�����
				{
					for(int i=0; i<10; i++)
					{
						if(GUS.RecordData.Data.PackStep[i])
							GUS.RecordData.Data.PackStep[i] += Data.BeltMoveTemp.Dir;
					}
				}
				GetPackedNum();
				GUW.ProcessData.Data.Num_NoBeltSupply += Data.BeltMoveTemp.Dir;
				if(GUW.ProcessData.Data.Num_NoBeltSupply > GUS.AlarmData.Data.Num_NoBeltSupply && GUS.AlarmData.Data.Num_NoBeltSupply != 0)
				{
					AlarmSetBit(2,0,16);//�������ʹ�������������ϴ��Ƿ�����
				}
				GUS.RecordData.Data.Num_PackBlet++;
				if(GUS.RecordData.Data.Num_PackBlet > GUS.AlarmData.Data.Num_NewBelt && GUS.AlarmData.Data.Num_NewBelt != 0)
				{
					AlarmSetBit(2,0,22);//�ϴ�ʹ���ѵ�����������
				}
				GUS.RecordData.Data.Num_PackFilm++;
				if(GUS.RecordData.Data.Num_PackFilm > GUS.AlarmData.Data.Num_NewFilm && GUS.AlarmData.Data.Num_NewFilm != 0)
				{
					AlarmSetBit(2,0,23);//��Ĥʹ���ѵ�����������
				}
				Data.BeltMoveTemp.Num--;
				if(Data.BeltMoveTemp.Num > 0)
					my.step = 1;
				else if(Data.BeltMoveTemp.Dir == -1)
				{
					AxRelSpd(AxBelt,-GUS.PackData.Data.Pos_Feed/2,Data.BeltMoveTemp.Spd);//����������϶
					my.step = 3;
				}
				else
				{
					LT.CutTask.execute = TEXE;
					TASKOK;
				}
			}
			break;
			
		case 3:
			if(AxSta(AxBelt) == AXSTA_READY)
			{
				AxRelSpd(AxBelt,GUS.PackData.Data.Pos_Feed/2,Data.BeltMoveTemp.Spd);//����������϶
				my.step = 4;
			}
			break;
			
		case 4:
			if(AxSta(AxBelt) == AXSTA_READY)
			{
				TASKOK;
			}
			break;
	}
}

/**
* @author 21/03/05  yao
* @Description: �ش��ʹ��������
* @param --
* @task: BeltSupplyTask
* @return --
*/
static void BeltSupply()
{
	static u32 tm,tm_start;
	if(TimerCnt(&tm_start,I_Get(I_ChkBelt) == ON && AlarmGetBit(2,0,9) == 0) > 50)
	{
		LT.BeltSupplyTask.execute = TEXE;//�ʹ�����
	}
	TASKRUN(LT.BeltSupplyTask)
	{
		case 1:
			GUW.ProcessData.Data.Num_NoBeltSupply = 0;	//��ռ���
			Q_Set(Q_BeltSupply,ON);						//�Ŵ���￪��
			TimerRst(&tm);
			my.step = 2;
			break;
			
		case 2:
			if(TimerCnt(&tm,1) > GUS.TimeData.Data.Tim_Supply)
			{
				Q_Set(Q_BeltSupply,OFF);				//�Ŵ����ر�
				TASKOK;
			}
			break;
	}
}

/**
* @author 21/03/05  yao
* @Description: �����ߴ�����
* @param --
* @return --
*/
//�����ߴ�
static void BeltKeepMove()
{
	if(FSM_STA != STOP)			//�ڷ�ֹͣ״̬�½�ֹ����
	{
		GUW.ManualData.Data.KeepBeltMove = 0;
	}
	if(GUW.ManualData.Data.KeepBeltMove)
	{
		LT.BeltKeepMoveTask.execute = TEXE;
	}
	TASKRUN(LT.BeltKeepMoveTask)
	{
		case 1:
			BeltMoveStart(1,1,100);			//�ƶ�һ��
			LT.BeltMoveTask.Manual = 1;		//��ʽΪ�ֶ�
			my.step = 2;
			break;
		
		case 2:
			if(LT.BeltMoveTask.execute == TIDLE)
			{
				LT.PressTask.execute = TEXE;	//�������
				LT.PressTask.Manual = 1;		//��ʽΪ�ֶ�
				my.step = 3;
			}
			break;
			
		case 3:
			if(LT.PressTask.execute == TIDLE)
			{
				TASKOK;
			}
			break;
	}
}

/**
* @author 21/03/05  yao
* @Description: �ߴ���������
* @param --
* @return --
*/
static void BeltHome()
{
	TASKRUN(LT.BeltHomeTask)//�ߴ���������
	{
		case 1:
			AxHome(AxBelt);	//����
			my.step = 2;
			break;
		
		case 2:
			if(AxSta(AxBelt) == AXSTA_READY)	//�ȴ����ֹͣ
			{
				AxAbs(AxBelt,GUS.PackData.Data.Pos_Ready);	//�������ߵ�Ԥ��λ
				my.step = 3;
			}
			break;
			
		case 3:
			if(AxSta(AxBelt) == AXSTA_READY)	//�ȴ����ֹͣ
			{
				TASKOK;
			}
			break;
	}
}

void Belt()
{
	BeltMove();		//װ�����ƶ�
	BeltSupply();	//�ش��ʹ��������
	BeltKeepMove();	//�����ߴ�
	BeltHome();		//װ��������
}

