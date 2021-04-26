#include "Encipherment.h"
#include "LockNew.h"

u32 DateNum;	//����
LoginDataDef LoginData;

//�������״̬
void Logcheckin()
{
	RTC_Get_Time(&GSR.time.hour, &GSR.time.min, &GSR.time.sec, &GSR.time.ampm);	
	RTC_Get_Date(&GSR.date.year, &GSR.date.month, &GSR.date.day, &GSR.date.week);	
	GSR.year = LoginData.DateNum%100;  //������
	GSR.month = LoginData.DateNum/100%100;  //������
	GSR.day = LoginData.DateNum/10000;     //������
	/******		�����ж�	****/	
    if(LoginData.Lockflag != 0xab || GUW.DisplayTime.Data.DateRequest!= 0)	//û�н�����
    {
        AlarmSetBit(3,0,15);//����״̬
    }
	else 
	{
		if(GSR.date.year>=20 )
		{
		
			if(GSR.date.year>GSR.year||\
				(GSR.date.year==GSR.year&&GSR.date.month>GSR.month)\
				||(GSR.date.year==GSR.year&&GSR.date.month==GSR.month&&GSR.date.day>GSR.day))
			{
				AlarmSetBit(3,0,15);  //������
				//GUS.Config.Data.Lockflag = 0;
				//GUS.Config.Data.Date = 0;
			}else
			{
				AlarmClearBit(3,0,15);
			}
		}
	}
	
/***********ע�ắ��***************/	
    if(GUW.FsmData.Data.Login == 1 && GUW.DisplayTime.Data.DateRequest == 0)
    {
		//GUR.RegistState = 0;
		GUW.FsmData.Data.Login = 0;
        if(Regist(GSR.CID[1],GSR.CID[0],GSW.LockPara.decode[0],&DateNum)==0)
        {   //�ɹ�
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
        {   //ʧ��
			//GUR.RegistState = 2;
            //AlarmSetBit(3,0,15);//����״̬
        }
		GSW.LockPara.refreshId = 1; 
    }
	
}
