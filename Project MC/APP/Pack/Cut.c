/*
*@Header:
*@File Name:	Cut.c
*@Author: Yao
*@Data: 2021/03/05
*@Description: �д�:
*					Cut()	�д� �ⲿ��ѯ Logic.c
*/

#include "Logic.h"

/**
* @author 21/03/05  yao
* @Description: �д�
* @param --
* @param --  
* @return --
*/
void Cut()
{
	static u32 tm;
	int i,CutFlag;
	TASKRUN(LT.CutTask)
	{
		case 1:
			CutFlag = 0;
			for(i=0; i<10; i++)
			{
				if(GUS.RecordData.Data.PackStep[i] >= BeltTotalNum + GUS.PackData.Data.Idx_Cut)//���������¼ >= �ܳ��� + �д�λ��
				{
					CutFlag = 1;
					GUS.RecordData.Data.PackStep[i] = 0;
				}
				if(GUS.RecordData.Data.PackStep[i] == GUS.PackData.Data.Idx_Cut)				//���������¼ == �д�λ��
				{
					CutFlag = 1;
				}
			}
			if(CutFlag == 1 || my.Manual == 1)//�����ֶ�
			{
				Q_Set(Q_CutCyd,ON);
				TimerRst(&tm);
				my.step = 2;
			}
			else
			{
				TASKOK;
			}
			break;
		
		case 2:
			if(TimerCnt(&tm,1) > 50)
			{
				GUS.RecordData.Data.Num_FinishReel++;// �ѱ����
				if(GUS.RecordData.Data.Num_FinishReel >= GUS.ProductData.Data.Num_TargetReel && GUS.ProductData.Data.Num_TargetReel != 0)
					//�ѱ���� >= Ŀ��������� ���� Ŀ�������Ϊ 0 
				{
					AlarmSetBit(2,0,23);//Ŀ������ѵ���
				}
				if(GUS.EnableData.Data.FinishStop == 1)//����ͣ��
				{
					AlarmSetBit(1,0,0);//�������
					FSM_CMD = STOP;
				}
				Q_Set(Q_CutCyd,OFF);//�д�����
				Q_Set(Q_BeltCollect,OFF);//�ر��մ����
				TASKOK;
			}
			break;
	}
}

