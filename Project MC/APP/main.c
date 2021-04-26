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

#define ZYG 1234 //�ͻ��� ��λ��

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
	
	/** ������� **/
	FRam_Read(2000,&LoginData,sizeof(LoginData));
	CID_init(ZYG,GSR.CID);//��ʼ��������
	GUW.DisplayTime.Data.DateRequest = 1;//�������ޱ�־λ
	/** ������� **/
	
	while(2 != HZ_ExCanNetStateGet()){
        bsp_exec();
	}
    while(1)
    {
		
		
		MainTask();
        InterLayer();
        bsp_exec();
		
		/** ������� **/
		Refresh(&GSW.LockPara.refreshId,GSR.CID);
		//�����������״���������룬�·�ʱ�䣬������DateRequest�仯
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
		/** ������� **/
    }
}


int pwr_Task(void)
{
	FRam_Write(0,&GSS,8000);
	Wait_Fram();
    return 0;
}







