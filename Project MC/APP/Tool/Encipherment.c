#include "Encipherment.h"
#include "LockNew.h"

u32 DateNum;	//日期
LoginDataDef LoginData;

//检查锁机状态
void Logcheckin()
{
	RTC_Get_Time(&GSR.time.hour, &GSR.time.min, &GSR.time.sec, &GSR.time.ampm);	
	RTC_Get_Date(&GSR.date.year, &GSR.date.month, &GSR.date.day, &GSR.date.week);	
	GSR.year = LoginData.DateNum%100;  //到期年
	GSR.month = LoginData.DateNum/100%100;  //到期月
	GSR.day = LoginData.DateNum/10000;     //到期日
	/******		日期判断	****/	
    if(LoginData.Lockflag != 0xab || GUW.DisplayTime.Data.DateRequest!= 0)	//没有解锁过
    {
        AlarmSetBit(3,0,15);//锁机状态
    }
	else 
	{
		if(GSR.date.year>=20 )
		{
		
			if(GSR.date.year>GSR.year||\
				(GSR.date.year==GSR.year&&GSR.date.month>GSR.month)\
				||(GSR.date.year==GSR.year&&GSR.date.month==GSR.month&&GSR.date.day>GSR.day))
			{
				AlarmSetBit(3,0,15);  //锁机了
				//GUS.Config.Data.Lockflag = 0;
				//GUS.Config.Data.Date = 0;
			}else
			{
				AlarmClearBit(3,0,15);
			}
		}
	}
	
/***********注册函数***************/	
    if(GUW.FsmData.Data.Login == 1 && GUW.DisplayTime.Data.DateRequest == 0)
    {
		//GUR.RegistState = 0;
		GUW.FsmData.Data.Login = 0;
        if(Regist(GSR.CID[1],GSR.CID[0],GSW.LockPara.decode[0],&DateNum)==0)
        {   //成功
			//GUR.RegistState = 1;
            LoginData.Lockflag = 0xab;
            LoginData.DateNum = DateNum;
            FRam_Write(2000,&LoginData,sizeof(LoginData));
            while(FRAM_GetSta()!= 0)
            {
                FRam_Exec();
            }
        } 
		else
        {   //失败
			//GUR.RegistState = 2;
            //AlarmSetBit(3,0,15);//锁机状态
        }
		GSW.LockPara.refreshId = 1; 
    }
	
}
