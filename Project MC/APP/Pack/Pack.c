/*
*@Header:
*@File Name:	Pack.c
*@Author: Yao
*@Data: 2021/03/05
*@Description: ���:
*					PackBefore()		ǰ��
*					PackBehind()		���
*					PackSample()		����Ʒ
*					PackSampleBehind()	��Ʒ���
*					GetPackedNum()		�����Ѿ�װ�������
*					Pack()				װ��	�ⲿ��ѯ Logic.c
*/
#include "Logic.h"
#include "Plate.h"
#include "Pack.h"
#include "Belt.h"

/**
* @author 21/03/05  yao
* @Description: ǰ��
* @param -- 
* @param --  
* @return --
*/
static void PackBefore()
{
	static u32 tm;
	TASKRUN(LT.PackBeforTask)
	{
		case 1:
			if(GUS.AlarmData.Data.Num_NewBelt - GUS.RecordData.Data.Num_PackBlet < BeltTotalNum && GUS.AlarmData.Data.Num_NewBelt != 0)
				//�µ� - ���õ� < һ������  ����  һ��������Ϊ0
			{
				AlarmSetBit(2,0,20);//ʣ���ϴ��Ѳ���
			}
			if(GUS.AlarmData.Data.Num_NewFilm - GUS.RecordData.Data.Num_PackFilm < BeltTotalNum && GUS.AlarmData.Data.Num_NewFilm != 0)
				//�µ� - ���õ� < һ������  ����  һ��������Ϊ0
			{
				AlarmSetBit(2,0,21);//ʣ�ེĤ�Ѳ���
			}
			my.count = 0;
			my.step = 2;
			break;
		
		case 2:
			BeltMoveStart(1,1,100);//װ���ߴ�
			my.step = 3;
			break;
		
		case 3:
			if(LT.BeltMoveTask.execute == TIDLE)//�ߴ�
			{
				LT.PressTask.execute = TEXE;//���
				my.step = 4;
			}
			break;
		
		case 4:
			if(LT.PressTask.execute == TIDLE)//���
			{
				TimerRst(&tm);
				my.step = 5;
			}
			break;
			
		case 5:
			if(TimerCnt(&tm,1) > 50)
			{
				if(my.count == 0)
				{
					for(int i=0; i<10; i++)
					{
						if(GUS.RecordData.Data.PackStep[i] == 0)//���Ϊ0
						{
							GUS.RecordData.Data.PackStep[i] = 1;
							break;
						}
					}
				}
				my.count++;
				if(my.count < GUS.ProductData.Data.Num_BeforPack + GUS.ProductData.Data.Num_NoPress)
					// 						 ǰ�ո����� +  ��������
				{
					my.step = 2;
				}
				else
				{
					TASKOK;
				}
			}
			break;
	}
}

/**
* @author 21/03/05  yao
* @Description: ���
* @param -- 
* @param --  
* @return --
*/
static void PackBehind()
{
	static u32 tm;
	TASKRUN(LT.PackBehindTask)//���
	{
		case 1:
			my.count = 0;
			my.step = 2;
			break;
		
		case 2:
			if(my.execute == TSTOP)//������ɱ��
			{
				PARAINIT(my);
				break;
			}
			if(FSM_STA == RUN)//�豸״̬Ϊ����
			{
				
				BeltMoveStart(1,1,100);//װ���ߴ� ���� ���� �ٶ�
				my.step = 3;
			}
			break;
			
		case 3:
			if(LT.BeltMoveTask.execute == TIDLE && LT.UpCameraTask.execute == TIDLE) //�ߴ����� == ���� ��������� == ����
			{
				/************�޸ĺ����Ӿ�����*******************/
				if(LT.UpCameraTask.done == 1)	//ִ�к���ǰһ����װ���װ��ʱ�Ѿ������Ӿ����ж����һ����װ��ʱ�������Ӿ����
				{
					LT.PressTask.execute = TEXE;	//�������
					LT.UpCameraTask.execute = TEXE;	//���������
					my.step = 4;
				}
				else
				{
					LT.PressTask.execute = TEXE;	//�������
					my.step = 0x31;
				}
			}
			break;
			
		case 0x31:
			if(LT.PressTask.execute == TIDLE)		//�������
			{
				BeltMoveStart(1,-1,100);			//װ���ߴ�
				my.step = 6;
			}
			break;
		
		case 4:
			if(LT.PressTask.execute == TIDLE)		////�������
			{
				TimerRst(&tm);
				my.step = 5;
			}
			break;
			
		case 5:
			if(TimerCnt(&tm,1) > 100)
			{
				my.count++;
				if(my.count < GUS.ProductData.Data.Num_BehindPack)	//��ո�����
				{
					my.step = 2;
				}
				else
				{
					TASKOK;
				}
			}
			break;
			
		case 6:
			if(LT.BeltMoveTask.execute == TIDLE)	//�ߴ����� == ����
			{
				if(GUS.EnableData.Data.RePack == 1)//�������Ϲ�����
				{
					LT.PlateTrigTask.execute = TEXE;//ת��λ�ô���
					LT.RepackTask.execute = TEXE;	//���Ϲ��ܿ���
					my.step = 7;
				}
				else
				{
					AlarmSetBit(2,0,11);//װ���Ӿ�NG��δ�����Զ����ϣ����˹�����
					my.step = 8;
				}
			}
			break;
			
		case 7:
			if(LT.RepackTask.execute == TIDLE)	//���Ͻ���
			{
				LT.PlateTrigTask.execute = TIDLE;////ת��λ�ô���
				if(LT.RepackTask.done == 1)	//�������ʧ��
				{
					my.step = 0x71;
				}
				else
				{
					AlarmSetBit(2,0,8);		//����ʧ�ܣ����鲢�˹�����
					my.step = 8;
				}
				BeltMoveStart(1,1,100);		//����λ�û�ԭ
			}
			break;
			
		case 0x71:
			if(LT.BeltMoveTask.execute == TIDLE)	//�ߴ����� == ����
			{
				TimerRst(&tm);
				LT.PressTask.execute = TEXE;	//�������
				LT.UpCameraTask.execute = TEXE;	//���������
				my.step = 5;
			}
			break;
			
		case 8:
			if(my.execute == TSTOP)//������ɱ��
			{
				PARAINIT(my);
				break;
			}
			if(FSM_STA == RUN)//�豸״̬������
			{
				my.step = 3;
			}
			break;
	}
}

/**
* @author 21/03/05  yao
* @Description: ����Ʒ
* @param -- 
* @param --  
* @return --
*/
static void PackSample()
{
	TASKRUN(LT.PackSampleTask)//����Ʒ����
	{
		case 1:		//��ʼ������
			my.count = 0;
			my.step = 2;
			break;
		
		case 2:		
			if(my.execute == TSTOP)//�����񱻽�ͣ
			{
				LT.PlateTrigTask.execute = TSTOP;//ת��λ�ô���
				PARAINIT(my);
				break;
			}
			if(FSM_STA == RUN)//�豸������
			{
				if(GUS.ProductData.Data.Num_Sample > 0)	//��Ʒ���� > 0
				{
					LT.PlateTrigTask.execute = TEXE;	//����ת��λ�ô���
					AxRelSpd(AxPlate,PPR_PLATE,GUS.PlateData.Data.Spd_Slow);	//ת����һȦ ��λ��һ��
					my.step = 3;
				}
				else
				{
					if(AxSta(AxPlate) == AXSTA_READY)
					{
						LT.PlateTrigTask.execute = TIDLE;//�����Ʒ��������ֹͣ����ת��λ�ô���
						TASKOK;
					}
				}
			}
			break;
			
		case 3:
			if(AxSta(AxPlate) == AXSTA_READY && LT.PackTask.execute != TEXE)//�ȴ�ת�����н��� ���� װ�����񲻱������ٻ�
			{
				if(LT.PackTask.done == 1)	//װ��ɹ�
				{
					my.count++;	//����
					if(my.count < GUS.ProductData.Data.Num_Sample)//ѭ��װ��ֱ������
					{
						my.step = 2;
					}
					else
					{
						LT.PlateTrigTask.execute = TIDLE;
						TASKOK;
					}
				}
				else	//װ��ɹ�
				{
					my.step = 2;
				}
			}
			break;
	}
}

/**
* @author 21/03/05  yao
* @Description: ��Ʒ���
* @param -- 
* @param --  
* @return --
*/
static void PackSampleBehind()
{
	static u32 tm;
	TASKRUN(LT.PackSampleBehindTask)//��Ʒ���
	{
		case 1:
			my.count = 0;
			my.step = 2;
			break;
		
		case 2:
			if(my.execute == TSTOP)
			{
				PARAINIT(my);
				break;
			}
			if(FSM_STA == RUN)//�����豸����״̬
			{
				if(GUS.ProductData.Data.Num_SampleBehind > 0)//��Ʒ��ո�����
				{
					BeltMoveStart(1,1,100);//ȫ��ǰ��һ��
					my.step = 3;
				}
				else
				{
					TASKOK;
				}
			}
			break;
			
		case 3:
			if(LT.BeltMoveTask.execute == TIDLE && LT.UpCameraTask.execute == TIDLE)//�ߴ�����Ϊ���� ���� ������������
			{
				/************�޸ĺ����Ӿ�����*******************/
				if(LT.UpCameraTask.done == 1)//�鿴�Ӿ����
				{
					LT.PressTask.execute = TEXE;//�������
					LT.UpCameraTask.execute = TEXE;//���������
					my.step = 4;
				}
				else
				{
					LT.PressTask.execute = TEXE;//�������
					my.step = 0x31;
				}
			}
			break;
			
		case 0x31://ִ�в���ǰ�ȵȴ����������ɣ�����������ʱĤ����
			if(LT.PressTask.execute == TIDLE)//�������
			{
				BeltMoveStart(1,-1,100);//ȫ�ٺ���1��
				my.step = 6;
			}
			break;
		
		case 4:
			if(LT.PressTask.execute == TIDLE)//�������
			{
				TimerRst(&tm);
				my.step = 5;
			}
			break;
			
		case 5:
			if(TimerCnt(&tm,1) > 100)//��ʱ 100ms 
			{
				my.count++;
				if(my.count < GUS.ProductData.Data.Num_SampleBehind)//���� < ��Ʒ��ո����� ѭ���ƶ�ֱ�������ﵽ
				{
					my.step = 2;
				}
				else
				{
					TASKOK;
				}
			}
			break;
			
		case 6:
			if(LT.BeltMoveTask.execute == TIDLE)//�ߴ��������
			{
				if(GUS.EnableData.Data.RePack == 1)//���޿�������
				{
					LT.PlateTrigTask.execute = TEXE;	//ת��λ�ô��������ٻ�
					LT.RepackTask.execute = TEXE;		//���������ٻ�
					my.step = 7;
				}
				else
				{
					AlarmSetBit(2,0,11);//װ���Ӿ�NG��δ�����Զ����ϣ����˹�����
					my.step = 8;
				}
			}
			break;
			
		case 7:
			if(LT.RepackTask.execute == TIDLE)		//�����������
			{
				LT.PlateTrigTask.execute = TIDLE;	//ת���������
				if(LT.RepackTask.done == 1)//�鿴����������Ϊ����
				{
					my.step = 0x71;
				}
				else
				{
					AlarmSetBit(2,0,8);//����ʧ�ܣ����鲢�˹�����
					my.step = 8;
				}
				BeltMoveStart(1,1,100);//���Ͽ�ʼλ�û�ԭ
			}
			break;
			
		case 0x71:
			if(LT.BeltMoveTask.execute == TIDLE)//װ��������
			{
				TimerRst(&tm);
				LT.PressTask.execute = TEXE;	//�������
				LT.UpCameraTask.execute = TEXE;	//���������
				my.step = 5;
			}
			break;
			
		case 8:
			if(my.execute == TSTOP)	//������ɱ��
			{
				PARAINIT(my);
				break;
			}
			if(FSM_STA == RUN)		//�豸״̬Ϊ����
			{
				my.step = 3;
			}
			break;
	}
}

/**
* @author 21/03/05  yao
* @Description: �����Ѿ�װ�������
* @param -- 
* @param --  
* @return --
*/
s32 GetPackedNum()
{
	s32 StepTemp = BeltTotalNum + GUS.PackData.Data.Idx_Cut;//װ���ܳ� + �д�λ��
	for(int i=0; i< 10; i++)//Ѱ����̵�����װ����ϴ�
	{
		if(GUS.RecordData.Data.PackStep[i] > 0 && GUS.RecordData.Data.PackStep[i] < StepTemp)
		{
			StepTemp = GUS.RecordData.Data.PackStep[i];
		}
	}
	StepTemp = StepTemp - GUS.ProductData.Data.Num_BeforPack - GUS.ProductData.Data.Num_NoPress;
				//����װ����ϴ� - ǰ�ո� - ��������
	if(StepTemp < 0)	//�������ȫ��Ϊ0
	{
		StepTemp = 0;
	}
	if(StepTemp > GUS.ProductData.Data.Num_Pack)	//����װ����ϴ� > Ŀ��װ������
	{
		StepTemp = GUS.ProductData.Data.Num_Pack;	//����װ����ϴ� = Ŀ��װ������
	}
	GUS.RecordData.Data.Num_Packed = StepTemp;	//��װ���� = ����װ����ϴ�
	return GUS.RecordData.Data.Num_Packed;		//������װ����
}

/**
* @author 21/03/05  yao
* @Description: װ��
* @param -- 
* @param --  
* @return --
*/
void Pack()
{
	static u32 tm,tm_Circle;
	static TrigDef tr_Start,tr_End;
	PackBefore();	//ǰ������
	PackBehind();	//�������
	PackSample();	//��Ʒ����
	PackSampleBehind();	//��Ʒ�������
	if(PlateTrig(&tr_Start,GUS.PlateData.Data.Pos_PackStart))	//װ�ʼλ�ô���
	{
		LT.PackTask.execute = TEXE;	//װ������
	}
	if(PlateTrig(&tr_End,GUS.PlateData.Data.Pos_PackEnd))		//װ�����λ�ô���
	{
		LT.PackTask.execute = TSTOP;
	}
	TASKRUN(LT.PackTask)
	{
		case 1:
			if(
				GUS.EnableData.Data.Pack == 1 &&
				GUR.PlateLoopData.Data.Have[GUS.StationData.Data.Idx_Pack] == 1 && 
				GUR.PlateLoopData.Data.OK[GUS.StationData.Data.Idx_Pack] == 1 &&
				LT.RepackTask.execute == TIDLE)
			{
				Q_Set(Q_PackVacuo,OFF);	//װ��λ��չ�
				BeltMoveStart(1, 1, 100);
				my.step = 2;
			}
			else
			{
				TASKNG;
			}
			break;
			
		case 2:
			if(LT.BeltMoveTask.execute == TIDLE)
			{
				if(LT.UpCameraTask.done == 1 || LT.PackSampleTask.execute == TEXE)//�Ӿ����������ڱ���Ʒ������Ʒ��ʱ�����Լ��ж��Ƿ�ɹ�
				{
					LT.UpCameraTask.execute = TEXE;	//�������������
					LT.PressTask.execute = TEXE;	//�����������
					GUW.DisplayData.Data.PackSpeed = 3600/TimerCnt(&tm_Circle,1);
					TimerRst(&tm_Circle);
					GUR.PlateLoopData.Data.Have[GUS.StationData.Data.Idx_Pack] = 0;
					TimerRst(&tm);
					my.step = 3;
				}
				else//�Ӿ�NG
				{
					/*********Ƥ������һ��ִ�в��϶���ǰҪ�ȷ�����������Ĥ�������********/
					HZ_AxStop(AxPlate);//����ͣת��
					LT.PressTask.execute = TEXE;	//�����������
					my.step = 0x21;
				}
			}
			break;
			
		case 0x21://�Ӿ�NG�ȴ������ɺ���в��ϣ���ֹĤ����
			if(LT.PressTask.execute == TIDLE)
			{
				BeltMoveStart(1,-1,100);//�ϴ�����һ��
				if(GUS.EnableData.Data.RePack == 1 && I_Get(I_DetectForPack) == ON)//����������� �� װ��ǰ��Ӧû����
				{
					LT.Blow2Task.Manual = 1;	//����2 ģʽΪ�ֶ�
					LT.Blow2Task.execute = TEXE;//ǿ�ƴ���һ���ϣ����������
				}
				if(LT.UpCameraTask.execute == TEXE)
				{
					AlarmSetBit(2,0,7);//װ���Ӿ�δ��ɣ�������Ӿ��������򽵵����������ٶ�
				}
				my.step = 4;
			}
			break;
			
		case 3:
			if(LT.PressTask.execute == TIDLE)//����������
			{
				TASKOK;
			}
			break;
			
		case 4:
			if(AxSta(AxPlate) == AXSTA_READY && LT.BeltMoveTask.execute == TIDLE)//ת��ֹͣ���ϴ�������ɺ�
			{
				TimerRst(&tm);
				Q_Set(Q_PackVacuo,ON);//���Ͽ����ת����
				my.step = 5;
			}
			break;
			
		case 5:
			if(TimerCnt(&tm,1) > GUS.TimeData.Data.Tim_Blow)//��ʱ
			{
				Data.PlateRun.Mode = StopToReady;//ת��ͣ��Ԥ��λ����ʱ���ȡ�����ŵ��ǿ���
				my.step = 6;
			}
			break;
			
		case 6:
			if(LT.PlateRunTask.execute == TIDLE)
			{
				Q_Set(Q_PackVacuo,OFF);//���Ͽ��л������
				if(GUS.EnableData.Data.RePack == 0)//�������ʹ��δ��
				{
					AlarmSetBit(2,0,11);//װ���Ӿ�NG��δ�����Զ����ϣ����˹�����
					TASKNG;
				}
				else
				{
					LT.RepackTask.execute = TEXE;//��������
					my.step = 7;
				}
			}
			break;
			
		case 7:
			if(LT.RepackTask.execute == TIDLE)//�ȴ������������
			{
				if(LT.RepackTask.done == 1)//������������ɹ�
				{
					if(FSM_STA == RUN && LT.PackSampleTask.execute == TIDLE && LT.PackBehindTask.execute == TIDLE && LT.PackSampleBehindTask.execute == TIDLE)
					{
						LT.PlateRunTask.execute = TEXE;//����ת��
					}
					TASKOK;
				}
				else
				{
					AlarmSetBit(2,0,8);//����ʧ�ܣ����鲢�˹�����
					TASKNG;
				}
			}
			break;
	}
}

