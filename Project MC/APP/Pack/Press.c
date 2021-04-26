/*
*@Header:
*@File Name:	Press.c
*@Author: Yao
*@Data: 2021/03/05
*@Description: �д�:
*					Press()	��� �ⲿ��ѯ Logic.c
*/

#include "Logic.h"

/**
* @author 21/03/05  yao
* @Description: ���
* @param --
* @param --  
* @return --
*/
//���
void Press()
{
	static u32 tm,tm_Cilcle;
	int i;
	TASKRUN(LT.PressTask)
	{
		case 1:
			for(i=0; i<10; i++)
			{
				if(GUS.RecordData.Data.PackStep[i])
				{
					if//�жϵ�ǰ���Ƿ���Ҫ���
					(
						GUS.RecordData.Data.PackStep[i] < GUS.PackData.Data.Idx_Press ||
						(GUS.RecordData.Data.PackStep[i] >= GUS.PackData.Data.Idx_Press + GUS.PackData.Data.Num_Press + GUS.ProductData.Data.Num_NoPress &&
						GUS.RecordData.Data.PackStep[i] <= BeltTotalNum)
					)
					{
						break;
					}
				}
			}
			if(i < 10 || my.Manual)//�ֶ�ģʽΪ1
			{
				if(Q_Get(Q_PressUpCyd) == OFF)//���������
				{
					TimerRst(&tm);
					Q_Set(Q_PressUpCyd,ON);//���������
					my.step =  2;
				}
				else
				{
					TimerRst(&tm_Cilcle);
					Q_Set(Q_PressDownCyd,ON);//���������
					TimerRst(&tm);
					my.step = 3;
				}
			}
			else
			{
				TASKOK;
			}
			break;
			
		case 2:
			if(TimerCnt(&tm,1) > 200)
			{
				Q_Set(Q_PressDownCyd,ON);//���������
				TimerRst(&tm);
				my.step = 3;
			}
			break;
			
		case 3:
			if(TimerCnt(&tm,1) >= GUS.TimeData.Data.Tim_PressDown)//�����ѹʱ��
			{
				Q_Set(Q_PressDownCyd,OFF);//���������
				TimerRst(&tm);
				my.step = 4;
			}
			break;
			
		case 4:
			if(TimerCnt(&tm,1) >= GUS.TimeData.Data.Tim_PressUp)//���̧��ʱ��
			{
				TimerRst(&tm);
				//GUR.TimeRecord.Data.PackPressTime = TimerCnt(&tm_Cilcle,1);ԭ��ʹ�ö����
				my.execute = TIDLE;
				my.done = 1;
				my.step = 5;
			}
			break;
			
		case 5:
			if(my.execute == TEXE)//�����ٻ�
			{
				my.step = 1;
				break;
			}
			if(TimerCnt(&tm,1) > 1000 && FSM_STA != RUN)//����1000ms �����ٻ� ����  �豸״̬����������
			{
				Q_Set(Q_PressUpCyd,OFF);
				TASKOK;
			}
			break;
	}
}


