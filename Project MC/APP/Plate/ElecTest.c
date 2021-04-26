/*
*@Header:
*@File Name:	ElecTest.c
*@Author: Yao
*@Data: 2021/03/05
*@Description: ���Բ������:
*					ElecTesk2()	��վ����
*					ElecTesk()	һվ���� ���ⲿ��ѯ Logic.c
*/


#include "Logic.h"
#include "Plate.h"

#define	MackETestResult(A,B,C) (((u32)A<<0)|((u32)B<<1)|((u32)C<<2))//λ����

/**
* @author 21/03/05  yao
* @Description: ��վ����
* @param -- 
* @return --
*/
void ElecTesk2()
{
	static u32 tm;
	static TrigDef tr_Start,tr_End;
	if(PlateTrig(&tr_Start,GUS.PlateData.Data.Pos_ETest2Start))
	{
		LT.ElecTest2Task.execute = TEXE;
	}
	if(PlateTrig(&tr_End,GUS.PlateData.Data.Pos_ETest2End))
	{
		LT.ElecTest2Task.execute = TSTOP;
	}
	Q_Set(Q_ElecTest2Sot,ON);//��վ����SOT
//	Q_Set(Q_ElecTest2Dir,(GpioSta)GUS.ETestData.Data.Dir_Target2);
	TASKRUN(LT.ElecTest2Task)
	{
		case 1:
			if(GUS.EnableData.Data.AgingMode == 1 || GUS.EnableData.Data.ElecTest2 == 0)//�ϻ�ģʽ��û�����Թ��ܣ���һ��������
			{
				TASKOK;
			}
			else if(GUR.PlateLoopData.Data.Have[GUS.StationData.Data.Idx_Etest2] == 1 || my.Manual)//����ģʽ���������ϻ��ֶ�����
			{
				TimerRst(&tm);
				my.step = 2;
			}
			else//����û���ϣ�ֱ�ӽ���
			{
				TASKOK;
			}
			break;
			
		case 2:
			if(I_Get(I_ElecTest2Ret) == ON)//��վ���Խ��
			{
				TASKOK;
			}
			else if(my.execute == TSTOP)
			{
				GUR.PlateLoopData.Data.OK[GUS.StationData.Data.Idx_Etest2] = -1;//��ʱ��û�ź�ֱ�Ӷ���ΪNG
				TASKOK;
			}
			break;
	}
}

/**
* @author 21/03/05  yao
* @Description: һվ����
* @param -- 
* @return --
*/
void ElecTesk()
{
	static u32 tm,tm_Cilcle;	//��ʱ��
	static TrigDef tr_Eot,tr_Start,tr_End;	//������
	static u32 LastResult;	//�ϴν���������ж����������ͬ����
	u32 ThisResult = MackETestResult(I_Get(BCD_I_A),I_Get(BCD_I_B),I_Get(BCD_I_C));	//ʵʱ��ȡ���ֵ
	
	ElecTesk2();	//��վ�����������
	
	if(GUW.ManualData.Data.KeepETest)	//��������
	{
		LT.ElecTestTask.Manual = 1;
		LT.ElecTestTask.execute = TEXE;	//���Բ�������
	}
	
	if(PlateTrig(&tr_Start,GUS.PlateData.Data.Pos_ETestStart))//λ������
	{
		LT.ElecTestTask.execute = TEXE;
	}
	if(PlateTrig(&tr_End,GUS.PlateData.Data.Pos_ETestEnd))//λ��ֹͣ
	{
		LT.ElecTestTask.execute = TSTOP;
	}
	TASKRUN(LT.ElecTestTask)
	{
		case 1:
			if(GUS.EnableData.Data.AgingMode == 1 || GUS.EnableData.Data.ElecTest == 0)//�ϻ�ģʽ��û�����Թ��ܣ���һ��������
			{
				if(GUS.ETestData.Data.Dir_Num == 0)//2����
				{
					GUR.PlateLoopData.Data.Elec[GUS.StationData.Data.Idx_ETest] = rand()%2? ElecTurn0:ElecTurn180;
				}
				else//4����
				{
					GUR.PlateLoopData.Data.Elec[GUS.StationData.Data.Idx_ETest] = rand()%4;
				}
				TASKOK;
			}
			else if(GUR.PlateLoopData.Data.Have[GUS.StationData.Data.Idx_ETest] == 1 || my.Manual)//����ģʽ���������ϻ��ֶ�����
			{
				Q_Set(BCD_Q_ETestSOT,ON);	//�򿪲���
				TimerRst(&tm_Cilcle);
				TimerRst(&tm);
				my.step = 2;
			}
			else//����û���ϣ�ֱ�ӽ���
			{
				TASKOK;
			}
			break;
		
		case 2:
			if(TimerCnt(&tm,1) < 5)break;
			Q_Set(BCD_Q_ETestSOT,OFF);
			TrigRst(&tr_Eot);
			TimerRst(&tm);
			my.step = 3;
			break;
			
		case 3:
			if(TimerCnt(&tm,1) > 200)
			{
				AlarmSetBit(2,0,17);//���Բ�������Ӧ
				TASKNG;
				break;
			}
			if(TrigOne(&tr_Eot,ThisResult != MackETestResult(OFF,OFF,OFF),0))//111
			{
				//GUR.TimeRecord.Data.ElecTest1Time = TimerCnt(&tm_Cilcle,1);ԭ��ʹ�õ�
				switch(GUS.ETestData.Data.Dir_Num)//�м������Է���
				{
					case 0://2��
						switch(ThisResult)
						{
							case MackETestResult(ON,OFF,OFF):
								GUW.ProcessData.Data.ETestResult = ElecTurn0;
								break;
							
							case MackETestResult(OFF,ON,OFF):
								GUW.ProcessData.Data.ETestResult = ElecTurn180;
								break;

							case MackETestResult(OFF,OFF,ON):
								GUW.ProcessData.Data.ETestResult = ElecNg;
								break;
							
							default:
								GUW.ProcessData.Data.ETestResult = ElecNg;
								break;
						}
						break;
							
					case 1://4��
						switch(ThisResult)
						{
							case MackETestResult(OFF,OFF,ON):
								GUW.ProcessData.Data.ETestResult = ElecTurn0;
								break;
							
							case MackETestResult(ON,OFF,OFF):
								GUW.ProcessData.Data.ETestResult = ElecTurn90;
								break;

							case MackETestResult(ON,OFF,ON):
								GUW.ProcessData.Data.ETestResult = ElecTurn180;
								break;

							case MackETestResult(OFF,ON,OFF):
								GUW.ProcessData.Data.ETestResult = ElecTurnN90;
								break;

							case MackETestResult(ON,ON,ON):
								GUW.ProcessData.Data.ETestResult = ElecNg;
								break;
							
							default:
								GUW.ProcessData.Data.ETestResult = ElecNg;
								break;
						}
						break;
				}
				GUR.PlateLoopData.Data.Elec[GUS.StationData.Data.Idx_ETest] = GUW.ProcessData.Data.ETestResult;//���Խ��  = ���Բ��Խ��
				if(GUW.ProcessData.Data.ETestResult == ElecNg)//���ΪNG 
				{
					GUR.PlateLoopData.Data.OK[GUS.StationData.Data.Idx_ETest] = 0;
					GUW.ProcessData.Data.Num_ETestNg++;
					if(GUW.ProcessData.Data.Num_ETestNg > GUS.AlarmData.Data.Num_ETestNg && GUS.AlarmData.Data.Num_ETestNg != 0)
					{
						GUW.ProcessData.Data.Num_ETestNg = 0;
						AlarmSetBit(2,0,18);//���Բ�����������
					}
				}
				else
				{
					GUW.ProcessData.Data.Num_ETestNg = 0;
					if(LastResult == GUW.ProcessData.Data.ETestResult)//�ϴεĽ�� == �µĽ��
					{
						GUW.ProcessData.Data.Num_ETestSame++;
						if(GUW.ProcessData.Data.Num_ETestSame > GUS.AlarmData.Data.Num_ETestSame && GUS.AlarmData.Data.Num_ETestSame != 0)
						{
							GUW.ProcessData.Data.Num_ETestSame = 0;
							AlarmSetBit(2,0,19);//���Բ��Խ��������ͬ
						}
					}
					else
					{
						GUW.ProcessData.Data.Num_ETestSame = 0;
					}
					LastResult = GUW.ProcessData.Data.ETestResult;//����
				}
				TASKOK;
			}
			if(my.execute == TSTOP)
			{
//				AlarmSetBit(2,0,1);//���Բ���δ��ɣ��������������Կ������ӣ���������ʱ��򽵵������ٶ�
				GUR.PlateLoopData.Data.OK[GUS.StationData.Data.Idx_ETest] = 0;
				TASKOK;
			}
			break;
	}
}


