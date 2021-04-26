/*
 * @Author: your name
 * @Date: 2020-05-26 14:03:11
 * @LastEditTime: 2020-06-30 22:40:14
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: \...\MainTask.c
 */
#include "maintask.h"
#include "teach.h"
#include "usertool.h"
#include "Datadefine.h"
#include "FSM.h"
#include "iodefine.h"

/**
* @berief: ������
*/
void AppErrHandle(void)
{
    if(GUR.FsmData.Data.HaveToReset == 1 && FSM_STA != D_RESET)
    {
        AlarmSetBit(3,0,0);//�豸�踴λ
    }
    if(GUR.FsmData.Data.HaveToRepower == 1 && FSM_STA != D_RESET)
    {
        AlarmSetBit(4,0,4);//�豸������
    }
    if(GSR.ErrorLevel<4&&GSR.ErrorLevel>0)
    {
        if(GUR.FsmData.Data.RunStatus == RUN)
        {
            GUW.FsmData.Data.RunCommand = ERRSTOP;
        }	
    }
    if(GSR.ErrorLevel ==4 )
    {
        GUR.FsmData.Data.HaveToRepower = 1;
        GUW.FsmData.Data.RunCommand = ERRSTOP;
    }
    if(GSR.ErrorLevel == 3)
    {
        GUR.FsmData.Data.HaveToReset = 1;
    }
	if(SysTim_GetUser() > 30000)
	{
		for(int i=0;i<PULS_NUM;i++)
		{
			if(ALM_GetSta(i) == GUS.MotorData.Data.AlarmDir[i])
			{
				AlarmSetBit(3, 0, i+1); //�������������
			}
		}
	}
}

/**
* �л���ֹͣ�����Ļص�
*/
void stop_handle(void)
{
    
}
/**
* �л�����̬ͣ�����Ļص�
*/
void scram_handle(void)
{
	for(int i=0;i<4;i++)
	{
		HZ_AxStop(i);
	}
	GUR.FsmData.Data.HaveToReset = 1;
	PARAINIT(LogicTask);
}


void MainTask(void)
{
    AppErrHandle();
    FSM(&GUW.FsmData.Data.RunCommand,&SysFsm);
    Logic();
    Reset();
    Teach();
	/* �㶯���� */
    if(GUR.FsmData.Data.RunStatus != RUN && GUR.FsmData.Data.RunStatus != D_RESET && GUR.FsmData.Data.RunStatus != SCRAM)
    {
        HZ_JogOperate(16);
    }
	else
	{
		PARAINIT(GSW.JogBackward);
		PARAINIT(GSW.JogForward);
		PARAINIT(GSW.JogGohome);
	}

}

