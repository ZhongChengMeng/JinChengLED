/*
*@Header:
*@File Name:	Repack.c
*@Author: Yao
*@Data: 2021/03/05
*@Description: �������:
*					Repack()	���� �ⲿ��ѯ Logic.c
*/

#include "Logic.h"
#include "Plate.h"
#include "Pack.h"
#include "Belt.h"

/**
* @author 21/03/05  yao
* @Description: ����
* @param --
* @param --  
* @return --
*/
//����
void Repack()
{
	static u32 tm;
	TASKRUN(LT.RepackTask)
	{
		case 1:
			/*************���ֱ����*****************/
			Q_Set(Q_FeederFlat,ON);
			if(AxSta(AxPlate) == AXSTA_READY)	//ת��ֹͣ
			{
				if(FSM_STA != RUN)//�ж��Ƿ�������״̬��
				{
					TASKOK;
					break;
				}
				if(GUR.PlateLoopData.Data.Have[GUS.StationData.Data.Idx_Pack-1] == 0)//�����һ�������ǿ� װ������
				{
					BeltMoveStart(2,-1,5);//�ش����ٺ���2
					my.step = 2;
				}
				else
				{
					GUR.PlateLoopData.Data.OK[GUS.StationData.Data.Idx_Blow2-1] = 0;//�ô���2����һ�������� 2�Ŵ������ϸ���λ
					AxRelSpd(AxPlate,PPR_PLATE,GUS.PlateData.Data.Spd_Slow);		//������תһ����λ
				}
				/*************��ӷ�������׹ر�*****************/
				Q_Set(Q_PressUpCyd,OFF);
			}
			break;
			
		case 2:
			if(LT.BeltMoveTask.execute == TIDLE)//�ߴ�����
			{
				Plate_Goto(1,GUS.PlateData.Data.Pos_PackStart);//ת�̵�װ�ʼλ
				my.step = 3;
			}
			break;
			
		case 3:
			if(AxSta(AxPlate) == AXSTA_READY)//ת�̵��׼��
			{
				Q_Set(Q_PackVacuo,ON);//װ�����ת����
				TimerRst(&tm);
				my.step = 4;
			}
			break;
			
		case 4:
			if(TimerCnt(&tm,1) > GUS.TimeData.Data.Tim_Blow)//					��������ʱ��
			{
				GUR.PlateLoopData.Data.OK[GUS.StationData.Data.Idx_Pack] = 0;	//װ�λ
				AxRelSpd(AxPlate,PPR_PLATE,GUS.PlateData.Data.Spd_Slow);		//������תһ��
				my.step = 5;
			}
			break;
			
		case 5:
			if(AxSta(AxPlate) == AXSTA_READY)//ת�̵��׼��
			{
				Q_Set(Q_PackVacuo,OFF);//װ����л����
				if(GUR.PlateLoopData.Data.Have[GUS.StationData.Data.Idx_Detect2] == 0 && I_Get(I_DetectTakeOut) == ON)//�������have==1���Ҽ�⵽���ϣ��Ѿ�ȡ������
				{
					my.step = 6;
					GUS.AlarmData.Data.Num_RepackNum ++;//ͳ�Ʋ�������
				}
				else
				{
					BeltMoveStart(2,1,5);
					my.step = 0x51;
				}
			}
			break;
			
		case 0x51:
			if(LT.BeltMoveTask.execute == TIDLE)//�ߴ�����
			{
				Plate_Goto(1,0);//ת�̵�Ԥ��λ
				my.step = 0x52;
			}
			break;
			
		case 0x52:
			if(AxSta(AxPlate) == AXSTA_READY)//ת�̵��׼��
			{
				AlarmSetBit(2,0,26);//����ȡNG����ʧ��
				TASKNG;
			}
			break;
			
		case 6:
			if(AxSta(AxPlate) == AXSTA_READY)//ת�̵��׼��
			{
				if((GUR.PlateLoopData.Data.Have[GUS.StationData.Data.Idx_Pack] == 1 &&
					GUR.PlateLoopData.Data.OK[GUS.StationData.Data.Idx_Pack] == 1) ||
					FSM_STA != RUN)
				{
					BeltMoveStart(2,1,5);//���������װ��ȥ��ֹͣ��ʱ����Ϊ�˻ָ����ϳ�ʼλ��
					my.step = 7;
				}
				else
				{
					AxRelSpd(AxPlate,PPR_PLATE,GUS.PlateData.Data.Spd_Slow);//ת�̵����תһȦ
				}
			}
			break;
			
		case 7:
			if(LT.BeltMoveTask.execute == TIDLE)//�ߴ�����
			{
				Plate_Goto(1,0);//ת�̵�Ԥ��λ
				LT.UpCameraTask.execute = TEXE;//ִ�����Ӿ�
				my.step = 8;
			}
			break;
			
		case 8:
			if(LT.UpCameraTask.execute == TIDLE && AxSta(AxPlate) == AXSTA_READY)//�ȴ����Ӿ���λ���� ���� ת��ֹͣ
			{
				if(LT.UpCameraTask.done == 1|| FSM_STA != RUN)//����Ӿ�ok
				{
					/*************��ӷ�������״�*****************/
					Q_Set(Q_PressUpCyd,ON);//���������
					TASKOK;
				}
				else
				{
					my.step = 1;
				}
			}
			break;
	}
}
