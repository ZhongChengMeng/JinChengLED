/*
*@Header:
*@File Name:	Plate.c
*@Author: Yao
*@Data: 2021/03/05
*@Description: ת��λ�����:
*					PlateTrig(TrigDef *tr, s32 TarPos)	ת�̹�λ������
*					PlateDataMove()	ת��������������
*					Plate_Goto(s32 Dir, s32 TarPos)	ת�̰�ָ�������ߵ�ָ��һȦ������
*					PlateRun()		ת�����п��ƣ��Զ��л����١�����״̬�������ⲿ����ͣ������ָͣ��
*					PlateHome()		ת�����л���
*					Plate()			�ⲿlogic.cʹ��
*/

#include "Logic.h"
#include "Feed.h"

/**
* @author 21/03/05  yao
* @Description: ת�̹�λ������
* @param -- TrigDef������ ����
* @return --
*/
//һȦ�ڵ�λ�ô���������Ҫ�ⲿ����һ��������
s32 PlateTrig(TrigDef *tr, s32 TarPos)
{
	static s32	LatestPos;//��¼����λ�ã������λ����������
	if(LT.PlateTrigTask.execute == TEXE)//������λ�ô���ʱ ת��λ�ô���
	{
		if(GSR.AxisEncoder[0] > LatestPos)//ֻ�е�ת����ת���Ž���λ�ø��£���ֹ����������ش���
		{
			LatestPos = GSR.AxisEncoder[0];
		}
	}
	else
	{
		TrigRst(tr);//���ô�����
		LatestPos = GSR.AxisEncoder[0];//�ر�λ�ô���ʱ��һֱ�������λ�ã�������λ�����ʱ������λ��Ҳ������
	}
	if(TrigOne(tr,LatestPos%PPR_PLATE >= TarPos,0))
	{
		return 1;
	}
	return 0;
}

/**
* @author 21/03/05  yao
* @Description: ת��������������
* @param --
* @return --
*/
//ת������������ֻ����������
void PlateDataMove()
{
	s32	Temp;
	static TrigDef tr;
	if(PlateTrig(&tr,50))//���ݴ��ݷ�����Ԥ��λ��50������
	{
		Temp = GUR.PlateLoopData.Data.OK[STATIONNUM-1];
		memmove(GUR.PlateLoopData.Data.OK+1,GUR.PlateLoopData.Data.OK,STATIONNUM*4);
		GUR.PlateLoopData.Data.OK[0] = Temp;

		Temp = GUR.PlateLoopData.Data.Elec[STATIONNUM-1];
		memmove(GUR.PlateLoopData.Data.Elec+1,GUR.PlateLoopData.Data.Elec,STATIONNUM*4);
		GUR.PlateLoopData.Data.Elec[0] = Temp;

		Temp = GUR.PlateLoopData.Data.Have[STATIONNUM-1];
		memmove(GUR.PlateLoopData.Data.Have+1,GUR.PlateLoopData.Data.Have,STATIONNUM*4);
		GUR.PlateLoopData.Data.Have[0] = Temp;
	}

//	if(Dir == -1)
//	{
//		Temp = GUR.PlateLoopData.Data.OK[0];
//		memmove(GUR.PlateLoopData.Data.OK,GUR.PlateLoopData.Data.OK+1,STATIONNUM*4);
//		GUR.PlateLoopData.Data.OK[STATIONNUM-1] = Temp;

//		Temp = GUR.PlateLoopData.Data.Elec[0];
//		memmove(GUR.PlateLoopData.Data.Elec,GUR.PlateLoopData.Data.Elec+1,STATIONNUM*4);
//		GUR.PlateLoopData.Data.Elec[STATIONNUM-1] = Temp;

//		Temp = GUR.PlateLoopData.Data.Have[0];
//		memmove(GUR.PlateLoopData.Data.Have,GUR.PlateLoopData.Data.Have+1,STATIONNUM*4);
//		GUR.PlateLoopData.Data.Have[STATIONNUM-1] = Temp;
//	}
}

/**
* @author 21/03/05  yao
* @Description: ת�̰�ָ�������ߵ�ָ��һȦ������
* @param --	���� һȦ��������
* @return --
*/
//ת�̰�ָ�������ߵ�ָ��һȦ������
void Plate_Goto(s32 Dir, s32 TarPos)//Dir == -1:��  1:��
{
	if(TarPos > PPR_PLATE || AxSta(AxPlate) != AXSTA_READY)return;// λ�ô���1000 �� ��״̬����׼��״̬
	s32	CurPos = GSR.AxisPosition[AxPlate] % PPR_PLATE;//���浱ǰ�����ڵ�λ��
	if(Dir == -1)
	{
		if(CurPos > TarPos)
		{
			AxRelSpd(AxPlate,TarPos-CurPos,GUS.PlateData.Data.Spd_Slow);//Ŀ�� - ��ǰ
		}
		else
		{
			AxRelSpd(AxPlate,TarPos-CurPos-PPR_PLATE,GUS.PlateData.Data.Spd_Slow);//Ŀ�� - ��ǰ - 1000
		}
	}
	if(Dir == 1)
	{
		if(CurPos > TarPos)
		{
			AxRelSpd(AxPlate,TarPos-CurPos+PPR_PLATE,GUS.PlateData.Data.Spd_Slow);//Ŀ�� - ��ǰ + 1000
		}
		else
		{
			AxRelSpd(AxPlate,TarPos-CurPos,GUS.PlateData.Data.Spd_Slow);//Ŀ�� - ��ǰ
		}
	}
}

/**
* @author 21/03/05  yao
* @Description: ת�����п��ƣ��Զ��л����١�����״̬�������ⲿ����ͣ������ָͣ��
* @param --	
* @return --
*/
static void PlateRun()
{
	static u32 tm,tm_LackFast,tm_LackSlow;
	TASKRUN(LT.PlateRunTask)
	{
		case 1:
			Data.PlateRun.Mode = Slow;		//������ģʽΪ����
			Data.PlateRun.IntoFastNum = 0;	//������ٵ�����
			Data.PlateRun.IntoSlowNum = 0;	//�������ٵ�����
			TimerRst(&tm);					//��λtm
			GUW.DisplayData.Data.RunSpeed = GUS.PlateData.Data.Spd_Slow;	//�����ٶ� = ͹������
			AxRelSpd(AxPlate,0x7ffffff,GUS.PlateData.Data.Spd_Slow);		//������һ��
			my.step = 2;
			break;
		
		case 2:
			switch(Data.PlateRun.Mode)
			{
				case Slow:
					if(TimerCnt(&tm_LackFast,I_Get(I_FlatLack) == ON) > 1000)
//					if(Data.PlateRun.IntoFastNum >= GUS.PlateData.Data.Num_SwitchToFast && TimerCnt(&tm_LackFast,I_Get(I_FlatLack) == OFF) < 300)
						//���ٵ����� > ���õ�����  ����  ֱ������ʱ�� < 300
					{
						Data.PlateRun.Mode = Fast;
						my.step = 3;
					}
					else if(GUW.DisplayData.Data.RunSpeed > GUS.PlateData.Data.Spd_Slow && TimerCnt(&tm,1) > 100)//100ms��һ���ٶȣ�ÿ�θ�2k/h
						//�����ٶ� > ���� ����  ��ʱ > 100
					{
						Data.PlateRun.Mode = Slow;
						my.step = 3;
					}
					break;
				
				case Fast:
					if(TimerCnt(&tm_LackSlow,I_Get(I_FlatLack) == OFF) > 300)
//					if(Data.PlateRun.IntoSlowNum >= GUS.PlateData.Data.Num_SwitchToSlow || TimerCnt(&tm_LackSlow,I_Get(I_FlatLack) == OFF) > 300)
					//
					{
						Data.PlateRun.Mode = Slow;
						my.step = 3;
					}
					else if(GUW.DisplayData.Data.RunSpeed < GUS.PlateData.Data.Spd_Fast && TimerCnt(&tm,1) > 100)//100ms��һ���ٶȣ�ÿ�θ�2k/h
					//�����ٶ� < ͹�ֿ����ٶ� ���� ��ʱ100ms
					{
						Data.PlateRun.Mode = Fast;
						my.step = 3;
					}
					break;
				
				case StopToReady:
					HZ_AxStopDec(AxPlate);//����ֹͣ
					my.step = 4;
					break;
				
				case StopRightNow:
					HZ_AxStop(AxPlate);//����ֹͣ
					my.step = 4;
					break;
			}
			break;
			
		case 3:
			switch(Data.PlateRun.Mode)
			{
				case Slow:
					TimerRst(&tm);
					GUW.DisplayData.Data.RunSpeed -= 2;
					if(GUW.DisplayData.Data.RunSpeed < GUS.PlateData.Data.Spd_Slow)
					{
						GUW.DisplayData.Data.RunSpeed = GUS.PlateData.Data.Spd_Slow;
					}
					HZ_AxReloadSpd(AxPlate,GUW.DisplayData.Data.RunSpeed*1000000/3600);
					my.step = 2;
					break;
				
				case Fast:
					TimerRst(&tm);
					GUW.DisplayData.Data.RunSpeed += 2;
					if(GUW.DisplayData.Data.RunSpeed > GUS.PlateData.Data.Spd_Fast)
					{
						GUW.DisplayData.Data.RunSpeed = GUS.PlateData.Data.Spd_Fast;
					}
					HZ_AxReloadSpd(AxPlate,GUW.DisplayData.Data.RunSpeed*1000000/3600);
					my.step = 2;
					break;
					
				default:
					my.step = 2;
					break;
			}
			break;
		
		case 4:
			switch(Data.PlateRun.Mode)
			{
				case StopToReady:
					if(AxSta(AxPlate) == AXSTA_READY)
					{
						Plate_Goto(1,0);
						my.step = 5;
					}
					break;
				
				case StopRightNow:
					if(AxSta(AxPlate) == AXSTA_READY)
					{
						TASKOK;
					}
					break;
					
				default:
					my.step = 2;
					break;
			}
			break;
			
		case 5:
			if(AxSta(AxPlate) == AXSTA_READY)
			{
				TASKOK;
			}
			break;
	}
}

/**
* @author 21/03/05  yao
* @Description: ת�����л���
* @param --	
* @return --
*/
//ת�̸�λ
static void PlateHome()
{
	static u32 tm;
	TASKRUN(LT.PlateHomeTask)
	{
		case 1:
			AxHome(AxPlate);
			my.step = 2;
			break;
		
		case 2:
			if(AxSta(AxPlate) == AXSTA_READY)
			{
				AxAbsSpd(AxPlate,GUS.PlateData.Data.Pos_Ready,GUS.PlateData.Data.Spd_Slow);//ȥ��Ԥ��λ
				my.step = 3;
			}
			break;
			
		case 3:
			if(AxSta(AxPlate) == AXSTA_READY)//�ȴ���ֹͣ
			{
				TimerRst(&tm);
				my.step = 4;
			}
			break;
			
		case 4:
			if(TimerCnt(&tm,1) > 100)
			{
				EnCode_SetVal(0,0);//���λ��
				HZ_AxSetCurPos(AxPlate,0);//���λ��
				TASKOK;
			}
			break;
	}
}

void Plate()
{
	PlateDataMove();//ת��������������
	PlateRun();		//ת������
	PlateHome();	//����
}


