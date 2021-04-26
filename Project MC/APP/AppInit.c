/*
 * @Descripttion:
 * @version:
 * @Author: yfs
 * @Date: 2019-12-27 09:17:32
 * @LastEditors: Please set LastEditors
 * @LastEditTime: 2020-06-01 15:22:32
 */
/*
 * File: AppInit.c
 * File Created: Tuesday, 9th October 2018 2:43:29 pm
 * version:v1.1
 * Description:Ӧ�ó����ʼ������
 * param:
 * Modified By:yang
 * -----
 * Copyright 2018 - 2018 <<hzzh>>, <<hzzh>>
 */

#include "AppInit.h"
#include "Algorithm.h"
#include "can_master.h"
#include "UserTool.h"
#include "stdio.h"
#include "Logic.h"
#include "Feed.h"

/**** ��������  ����app����ĳ�ʼ����ʵ�������� ****/
GlobalDataDef GlobalData = {0}; //�����û�ͨѶ���ݿ�
u8 uart_sendbuf[2][300];        //����ͨѶ���պͷ��ͻ���
u8 uart_recvbuf[2][300];
u8 eth_txbuf[300];
u8 eth_rxbuf[300];

/*******************end define******************/

/**
 * @author: yfs
 * @Date: 2020-01-14 16:50:27
 * @description: �忨�����ϵ�flashȫ-1������Ҫ��ʼ��
 * @para:
 * @return:
 */
void DataInit(void)
{
	u32 i;
	if(GSS.SaveCheck.checkflag != DATACHAECK)
	{
		/*�˴���ӳ�ʼ���洢�����ݣ���֤�����ϵ�ûʹ�õ����ݰ�ȫ*/
		memset(&GSS,0,sizeof(GSS));
		memset(&GUS,0,sizeof(GUS));
		for(i = 0;i<PULS_NUM+PULS_NUM_Ex;i++)
		{
			GSS.axis[i].Axconver.MPR = 1;
			GSS.axis[i].Axconver.PPR = 1;
			
			GSS.axis[i].AxSpd.startspeed = 20;	//mm/s
			GSS.axis[i].AxSpd.acctime = 100; 	//ms
			GSS.axis[i].AxSpd.runspeed = 60;	//mm/s
			GSS.axis[i].AxSpd.dectime = 100;	//ms
			GSS.axis[i].AxSpd.endspeed = 20;	//mm/s
			
			GSS.axis[i].Axhomecfg.homespeedfast = 10;	//mm/s
			GSS.axis[i].Axhomecfg.homespeedslow = 5;	//mm/s
		}
		/* ��ʼ����־ */
		GSS.SaveCheck.checkflag = DATACHAECK;
	}
}

//��ʼ���Ѿ��̶��������
void InitAxisConfig(int num)
{
    int i;
    for(i=0; i<num; i++)
    {   
		//ԭ������
		GSS.axis[i].Axhomecfg.homemode = GOHOMETYPE1;
		GSS.axis[i].Axhomecfg.orgnum = i;
		GSS.axis[i].Axhomecfg.orglev = ON;
		GSS.axis[i].Axhomecfg.homespeedslow = 200;//mm/s
		GSS.axis[i].Axhomecfg.homespeedoffset = 0;
		
		//�����ٶ�����
		GSS.axis[i].AxSpd.startspeed = 1000;
		GSS.axis[i].AxSpd.endspeed = 1000;
		
		//ת��
		GSS.axis[i].Axconver.MPR = 1;
		GSS.axis[i].Axconver.PPR = 1;
		
		//��λ����
		GSS.axis[i].Axlimitcfg.limitMode = 0;
		//��������
		GSS.axis[i].Axlimitcfg.alarmmode = 2;
    }
	GSS.axis[AxPlate].Axhomecfg.homemode = GOHOMETYPE4;
	GSS.axis[AxPlate].Axhomecfg.orgnum = 147;
	GSS.axis[AxPlate].Axhomecfg.orglev = ON;
	/*�����������Ч*/
	AxisConfig(GSS.axis);
	//�㶯
	GSW.jogSpd = 100;
	GSW.JogMode = 2;

}

void initIO(void)
{
    int i;
    for (i = 0; i < GPO_NUM; i++) //��ʼ�� �����
        OutPut_SetSta(i, OFF);
    for (i = 0; i <GPO_NUM_Ex ; i++)
        HZ_ExOutPutSet(1,i,OFF);
    for (i = 0; i < PULS_NUM; i++) //���ʹ��,��ƽ����ͨ����෴
        EN_SetSta(i, GUS.MotorData.Data.EnableDir[i]);
    for (i = 0; i < PULS_NUM_Ex; i++) //���ʹ��,��ƽ����ͨ����෴
        HZ_ExEnSet(1, i, 1);
	Q_Set(Q_BeltCollect,ON);	  //�����մ����
}

void AppInit()
{
    //���õ�ַ�����modbus
    init_config(&GlobalData, GSR.ErrorCode, &GlobalData.SpaceType.Save, GLOBALDATALEN, USERWRITELEN, USERREADLEN, USERSAVELEN);
	/* �������ж�ȡ���� */
    FRam_Read(0,&GSS,SAVELEN);
	Wait_Fram();
	/* ���ݼ���Ƿ�Ϊ�տ� */
	DataInit();
	/* ��ʼ���ϵ����Ч�������� */
	InitAxisConfig(PULS_NUM+PULS_NUM_Ex);
	/* ��ʼ��io */
    initIO();
	/*�ϵ���Ҫ��λ*/
    GUR.FsmData.Data.HaveToReset = 1;
    /*��ʼ��״̬��*/
    InitFsm(&SysFsm);
	/* ��ȡ�汾��Ϣ */
    Hz_AutoVision();
}
