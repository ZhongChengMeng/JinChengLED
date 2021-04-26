/*
 * @Author: your name
 * @Date: 2020-05-26 14:03:11
 * @LastEditTime: 2020-05-27 14:03:30
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: \YD_KZJ\Code\APP\main.c
 */ 
#include "interlayer.h"
#include "bsp_config.h"
#include "appinit.h"
#include "maintask.h"
#include "can_master.h"
#include "DataDefine.h"
#include "Encipherment.h"

#define ZYG 1234 //客户码 四位数

#if __USE_IAP__
const uint32_t Code_Versions __attribute__((at(0x08040800))) = 0xA06;
#endif

int main()
{
#if __USE_IAP__
 	sys_init_IAP();
#else
	sys_init();
#endif
	
    bsp_init();
    bsp_exec();
    AppInit();
	
	/** 加密相关 **/
	FRam_Read(2000,&LoginData,sizeof(LoginData));
	CID_init(ZYG,GSR.CID);//初始化申请码
	GUW.DisplayTime.Data.DateRequest = 1;//加密有无标志位
	/** 加密相关 **/
	
	while(2 != HZ_ExCanNetStateGet()){
        bsp_exec();
	}
    while(1)
    {
		
		
		MainTask();
        InterLayer();
        bsp_exec();
		
		/** 加密相关 **/
		Refresh(&GSW.LockPara.refreshId,GSR.CID);
		//用来触发繁易触摸屏宏代码，下发时间，条件是DateRequest变化
		if(GUW.DisplayTime.Data.DateRequest != 0)
		{
			GUW.DisplayTime.Data.DateRequest++;
		}
		Logcheckin();
		if(GUW.DisplayTime.Data.settime == 1)
		{
			GUW.DisplayTime.Data.settime = 0;
			RTC_Set_Date((GUW.DisplayTime.Data.y-2000),GUW.DisplayTime.Data.m,GUW.DisplayTime.Data.d,0);
			RTC_Set_Time(GUW.DisplayTime.Data.hour,GUW.DisplayTime.Data.min,GUW.DisplayTime.Data.sec,0);
		}
		/** 加密相关 **/
    }
}


int pwr_Task(void)
{
	FRam_Write(0,&GSS,8000);
	Wait_Fram();
    return 0;
}







