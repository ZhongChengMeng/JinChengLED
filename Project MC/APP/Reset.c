/*
 * @Author: your name
 * @Date: 2020-05-26 14:03:11
 * @LastEditTime: 2020-06-04 18:41:50
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: \YD_KZJ\Code\APP\Reset.c
 */
#include "reset.h"
#include "usertool.h"
#include "iodefine.h"
#include "logic.h"
#include "Feed.h"
LogicParaDef ResetTask;

void reset_handle(void)
{
    REST_TASK.execute = TEXE;
}


extern void initIO(void);

//�豸��λ����
void Reset()
{
	static u32 tm;
    if(GUR.FsmData.Data.RunStatus == D_RESET)//500��״̬
    {
        if(REST_TASK.execute == TEXE && REST_TASK.step == 0)
        {
            REST_TASK.step = 1;
        }
        switch(REST_TASK.step)
        {
		case 1:
			for(int i=0; i<GPO_NUM; i++)
			{
				if(i != 3)
				{				
					Q_Set((Gpio)i,OFF);
				}
			}
			FeedVacuo_Stop;
			if(GUS.EnableData.Data.LightIn == 1)//���Դ
			{
				Q_Set(Q_LightIn,ON);
			}
			if(GUS.EnableData.Data.LightOut == 1)//���Դ
			{
				Q_Set(Q_LightOut,ON);
			}
			GUW.ManualData.Data.ETurnHome = 1;//��תʹ��
			GSW.ClearAlarm = 1;//��ձ���
			PARAINIT(LogicTask);
			TimerRst(&tm);
			REST_TASK.step = 2;
			break;
		
		case 2:
			if(TimerCnt(&tm,1) > 1000)
			{
				if(GSR.ErrorLevel == 0)//����ȼ�
				{
					LT.PlateHomeTask.execute = TEXE;//ת�̻���
					REST_TASK.step = 3;
				}
				else//�����ձ���ʧ��
				{
					FSM_CMD = ERRSTOP;//�����ť���豸״̬����ֹͣ
					PARAINIT(REST_TASK);
				}
			}
			break;
		
		case 3:
			if(LT.PlateHomeTask.execute == TIDLE)//ת�̻������
			{
				LT.AdjustHomeTask[0].execute = TEXE;//����λ����
				LT.AdjustHomeTask[1].execute = TEXE;//
				LT.BeltHomeTask.execute = TEXE;//װ�������������ٻ�
				REST_TASK.step = 4;
			}
			break;
			
		case 4:
			if(LT.AdjustHomeTask[0].execute == TIDLE && 
				LT.AdjustHomeTask[1].execute == TIDLE && 
				LT.BeltHomeTask.execute == TIDLE)//�����������
			{
				LT.CleanTask.execute = TEXE;//�ٻ�ת��������������
				REST_TASK.step = 5;
			}
			break;
			
		case 5:
			if(LT.CleanTask.execute == TIDLE)			//ת�����������������
			{
				PARAINIT(REST_TASK);	
				GSW.ClearAlarm = 1;		
				GUR.FsmData.Data.HaveToReset = 0;
				GUW.FsmData.Data.RunCommand = STOP;		//FSM_CMD = ERRSTOP;
				GUS.TransWatch.Data.TransWatch1 = 1;	//��ת���ڵ�����1
			}
			break;
        }
    }
    else
    {
        PARAINIT(REST_TASK);
    }
}



