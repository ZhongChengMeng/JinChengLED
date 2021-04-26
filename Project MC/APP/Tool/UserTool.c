/*
 * @Author: your name
 * @Date: 2020-05-26 15:09:21
 * @LastEditTime: 2020-05-29 16:35:23
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: \YD_KZJ\Code\APP\Tool\UserTool.c
 */ 
#include "UserTool.h"
#include "can_master.h"
#include "DataDefine.h"
#include "stdio.h"
#include "logicData.h"

/********************************************************/

/**
* @author 19/02/19  yang
* @Description: 每小时产出
* @param -- 产量
* @param -- 时间 （毫秒）
* @return -- units per hour(通常使用时是要除1000，把单位变成k)
*/
float UPH(u32 time,u32 products)
{
    return (float)products * 1000 * 3600 / (float)time;
}
/**
* @author 19/02/19  yang
* @Description: 单位换算 毫米转脉冲
* @param -- num 轴号
* @param -- MM 毫米
* @return -- 脉冲数
*/
s32 ConverToPulse(u32 num,float MM)
{
    //return	MM *GSS.AxisData[num].PPR / GSS.Conversion[num].MPR;
	if( GSS.axis[num].Axconver.MPR > 0.001f)
	return (MM*GSS.axis[num].Axconver.PPR/GSS.axis[num].Axconver.MPR);
	else 
	return 1;
}
/**
* @author 19/02/19  yang
* @Description: 单位转换 脉冲转毫米
* @param --
* @param --
* @return --
*/
float ConvertToUserUnit(u32 num,s32 pulse)
{
    //return (float)GSS.Conversion[num].MPR * pulse / (float)GSS.Conversion[num].PPR;
	if(GSS.axis[num].Axconver.PPR>=1)
	{
		return (float)GSS.axis[num].Axconver.MPR*pulse/(float)GSS.axis[num].Axconver.PPR;
	}
	else
	{
		return 1;
	}
}

/*****************************************************************/
//获取输入口状态
//cardAdr : 扩展卡地址  0:本体  1：从卡1 ......
//num : 输入编号
//return ：0：ON 1:OFF，其他参数错误
s32 InGet(u8 cardAdr,u8 num)
{
	if(cardAdr == 0)
	{
		return InPut_GetSta(num);
	}
	else
	{
		return HZ_ExInPutGet(cardAdr - 1,num);
	}
}


//获取输入口状态
//cardAdr : 扩展卡地址  0:本体  1：从卡1 ......
//num
//return ：0：ON 1:OFF，其他参数错误
s32 OutGet(u8 cardAdr,u8 num)
{
	if(cardAdr == 0)
	{
		return OutPut_GetSta(num);
	}
	else
	{
		return HZ_ExOutPutGet(cardAdr - 1,num);
	}
}

//获取输入口状态
//cardAdr : 扩展卡地址  0:本体  1：从卡1 ......
//num：输入编号
//return ：0：ON 1:OFF，其他参数错误
s32 OutSet(u8 cardAdr,u8 num,u8 value)
{
	if(cardAdr == 0)
	{
		return OutPut_SetSta(num,value);
	}
	else
	{
		return HZ_ExOutPutSet(cardAdr - 1,num,value);
	}
}

/**
 * @brief 软件版本号字符串自动生成 SVer: yyyy.mm.dd  hh:mm:ss，硬件版本信息暂时手动填写
 * @param
 * @return
 * @author	Mr.Yiu
 */
void Hz_AutoVision(void)
{
    char s_month[5];
    int month, day, year;
    static const char month_names[] = "JanFebMarAprMayJunJulAugSepOctNovDec";
    sscanf(__DATE__, "%s %d %d", s_month, &day, &year);
    month = (strstr(month_names, s_month)-month_names)/3;
    sprintf((char *)GSR.SoftWare_Ver,"SVer: %d.%d.%d  %s",year,month+1,day, __TIME__);

    //硬件版本信息暂时手动填写
    sprintf((char *)GSR.HardWare_Ver,"HVer: HMC-G16");
}


/**
* @berief: 参数导出到U盘
*/
int File_export(void)
{
	int testtt;
	testtt =  File_Write("Config",&GUS,0,USERSAVELEN,0);
	return testtt;
}

/**
* @berief: 参数从U盘导入
*/
int File_import(void)
{
	int testtt;
	testtt = File_Read("Config",&GUS,0,USERSAVELEN,0);
	return testtt;
}


/** 
  * @brief 软件定时器 
  */ 
u32 TimOn(TmTyp *tm,int ex,u32 timeout)
{
	if(ex)//条件满足
	{
		if(tm->start == 0)
		{
			tm->start = 1;  //开始计时
			tm->starttick = SysTim_GetUser();
		}
	}else 
	{
		PARAINIT(*tm);
		return 0;
	}
	if(tm->start == 1 && (SysTim_GetUser()-tm->starttick)/10 >= timeout)
	{
		return 1;
	}
	return 0;
}



