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
 * Description:应用程序初始化配置
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

/**** 变量声明  所有app程序的初始变量实例及声明 ****/
GlobalDataDef GlobalData = {0}; //定义用户通讯数据库
u8 uart_sendbuf[2][300];        //定义通讯接收和发送缓存
u8 uart_recvbuf[2][300];
u8 eth_txbuf[300];
u8 eth_rxbuf[300];

/*******************end define******************/

/**
 * @author: yfs
 * @Date: 2020-01-14 16:50:27
 * @description: 板卡初次上电flash全-1数据需要初始化
 * @para:
 * @return:
 */
void DataInit(void)
{
	u32 i;
	if(GSS.SaveCheck.checkflag != DATACHAECK)
	{
		/*此处添加初始化存储区数据，保证初次上电没使用的数据安全*/
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
		/* 初始化标志 */
		GSS.SaveCheck.checkflag = DATACHAECK;
	}
}

//初始化已经固定的轴参数
void InitAxisConfig(int num)
{
    int i;
    for(i=0; i<num; i++)
    {   
		//原点配置
		GSS.axis[i].Axhomecfg.homemode = GOHOMETYPE1;
		GSS.axis[i].Axhomecfg.orgnum = i;
		GSS.axis[i].Axhomecfg.orglev = ON;
		GSS.axis[i].Axhomecfg.homespeedslow = 200;//mm/s
		GSS.axis[i].Axhomecfg.homespeedoffset = 0;
		
		//运行速度配置
		GSS.axis[i].AxSpd.startspeed = 1000;
		GSS.axis[i].AxSpd.endspeed = 1000;
		
		//转换
		GSS.axis[i].Axconver.MPR = 1;
		GSS.axis[i].Axconver.PPR = 1;
		
		//限位配置
		GSS.axis[i].Axlimitcfg.limitMode = 0;
		//报警配置
		GSS.axis[i].Axlimitcfg.alarmmode = 2;
    }
	GSS.axis[AxPlate].Axhomecfg.homemode = GOHOMETYPE4;
	GSS.axis[AxPlate].Axhomecfg.orgnum = 147;
	GSS.axis[AxPlate].Axhomecfg.orglev = ON;
	/*轴参数配置生效*/
	AxisConfig(GSS.axis);
	//点动
	GSW.jogSpd = 100;
	GSW.JogMode = 2;

}

void initIO(void)
{
    int i;
    for (i = 0; i < GPO_NUM; i++) //初始化 输出口
        OutPut_SetSta(i, OFF);
    for (i = 0; i <GPO_NUM_Ex ; i++)
        HZ_ExOutPutSet(1,i,OFF);
    for (i = 0; i < PULS_NUM; i++) //轴口使能,电平和普通输出相反
        EN_SetSta(i, GUS.MotorData.Data.EnableDir[i]);
    for (i = 0; i < PULS_NUM_Ex; i++) //轴口使能,电平和普通输出相反
        HZ_ExEnSet(1, i, 1);
	Q_Set(Q_BeltCollect,ON);	  //开启收带电机
}

void AppInit()
{
    //配置地址表完成modbus
    init_config(&GlobalData, GSR.ErrorCode, &GlobalData.SpaceType.Save, GLOBALDATALEN, USERWRITELEN, USERREADLEN, USERSAVELEN);
	/* 从铁电中读取数据 */
    FRam_Read(0,&GSS,SAVELEN);
	Wait_Fram();
	/* 数据检查是否为空卡 */
	DataInit();
	/* 初始化上电后有效的轴配置 */
	InitAxisConfig(PULS_NUM+PULS_NUM_Ex);
	/* 初始化io */
    initIO();
	/*上电需要复位*/
    GUR.FsmData.Data.HaveToReset = 1;
    /*初始化状态机*/
    InitFsm(&SysFsm);
	/* 获取版本信息 */
    Hz_AutoVision();
}
