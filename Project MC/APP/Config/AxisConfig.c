/*
 * @Author: your name
 * @Date: 2020-05-26 15:09:14
 * @LastEditTime: 2020-05-27 15:13:51
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: \YD_KZJ\Code\APP\Config\AxisConfig.c
 */ 
#include "axisconfig.h"
#include "usertool.h"
#include "can_master.h"

/**
* @berief: ���������
*/
void AxisConfig(AxisData ax[])
{
    int i;
    for(i=0; i<PULS_NUM; i++)
    {
        switch(ax[i].Axlimitcfg.limitMode)
        {
        case 0: //û����λ
            HZ_AxSetAxiSig(i,1,ax[i].Axhomecfg.orgnum,ax[i].Axhomecfg.orglev,0,0,0,0,0,0);
            HZ_AxSetLimSoft(i,0,ax[i].Axlimitcfg.softmaxlimit,0,ax[i].Axlimitcfg.softminlimit);
            break;
        case 1: //ֻ�������λ
            HZ_AxSetAxiSig(i,1,ax[i].Axhomecfg.orgnum,ax[i].Axhomecfg.orglev,0,0,0,0,0,0);
            HZ_AxSetLimSoft(i,1,ConverToPulse(i,ax[i].Axlimitcfg.softmaxlimit),1,ConverToPulse(i, ax[i].Axlimitcfg.softminlimit));
            break;
        case 2:  //ֻ��Ӳ����λ
            HZ_AxSetAxiSig(i,1,ax[i].Axhomecfg.orgnum,ax[i].Axhomecfg.orglev,\
                           1,ax[i].Axlimitcfg.poslimitsig,ax[i].Axlimitcfg.poslimitlev,\
                           1,ax[i].Axlimitcfg.neglimitsig,ax[i].Axlimitcfg.neglimitlev);
            HZ_AxSetLimSoft(i,0,ConverToPulse(i, ax[i].Axlimitcfg.softmaxlimit),0,ConverToPulse(i, ax[i].Axlimitcfg.softminlimit));
            break;
        case 3:  //������λ����Ч
			HZ_AxSetAxiSig(i,1,ax[i].Axhomecfg.orgnum,ax[i].Axhomecfg.orglev,\
                           1,ax[i].Axlimitcfg.poslimitsig,ax[i].Axlimitcfg.poslimitlev,\
                           1,ax[i].Axlimitcfg.neglimitsig,ax[i].Axlimitcfg.neglimitlev);
            HZ_AxSetLimSoft(i,1,ConverToPulse(i, ax[i].Axlimitcfg.softmaxlimit),1,ConverToPulse(i, ax[i].Axlimitcfg.softminlimit));
            break;
        }
		HZ_AxSetAlm(i,ax[i].Axlimitcfg.alarmmode);   //�����ᱨ��
		AxisHomeCfg(i,ax[i].Axhomecfg);              //���û������
		AxisSetSpd(i,ax[i].AxSpd);                   //�����ٶ�
    }
	for(i=PULS_NUM;i<(PULS_NUM_Ex+PULS_NUM);i++)
	{
		switch(ax[i].Axlimitcfg.limitMode)
        {
        case 0: //û����λ
			HZ_ExAxSetLimit_H(i-PULS_NUM,0,0,0,0,0,0);
			HZ_ExAxSetLimit_S(i-PULS_NUM,0,0,0,0);
			break;
        case 1: //ֻ�������λ
			HZ_ExAxSetLimit_S(i-PULS_NUM,ConverToPulse(i,ax[i].Axlimitcfg.softmaxlimit),\
							  1,ConverToPulse(i,ax[i].Axlimitcfg.softminlimit),1);
			HZ_ExAxSetLimit_H(i-PULS_NUM,0,0,0,0,0,0);
            break;
        case 2:  //ֻ��Ӳ����λ
			HZ_ExAxSetLimit_S(i-PULS_NUM,0,0,0,0);
			HZ_ExAxSetLimit_H(i-PULS_NUM,1,ax[i].Axlimitcfg.poslimitsig,ax[i].Axlimitcfg.poslimitlev,\
							  1,ax[i].Axlimitcfg.neglimitsig,ax[i].Axlimitcfg.neglimitlev);
            break;
		case 3:  //������λ������
			HZ_ExAxSetLimit_H(i-PULS_NUM,1,ax[i].Axlimitcfg.poslimitsig,ax[i].Axlimitcfg.poslimitlev,\
							  1,ax[i].Axlimitcfg.neglimitsig,ax[i].Axlimitcfg.neglimitlev);
			HZ_ExAxSetLimit_S(i-PULS_NUM,ConverToPulse(i,ax[i].Axlimitcfg.softmaxlimit),\
							  1,ConverToPulse(i,ax[i].Axlimitcfg.softminlimit),1);
			break;
		}
		//���û������
		HZ_ExAxSetHomMod(i-PULS_NUM,1,ax[i].Axhomecfg.orgnum,ax[i].Axhomecfg.orglev,ax[i].Axhomecfg.homemode);
		//�����ٶ�
		AxisSetSpd(i-PULS_NUM,ax[i].AxSpd);
		//�᷽��
	}
	
}
/**
* @berief: ������Ļ����ٶȣ��û���λ��
*/
void AxisHomeCfg(u8 num, AxHomeDef home)
{
	if(num > PULS_NUM)
	 return ;
	HZ_AxSetHomePara(num,home.orgnum,home.orglev,home.homemode,\
							ConverToPulse(num,home.homespeedfast),\
							ConverToPulse(num,home.homespeedslow),\
							ConverToPulse(num,home.homespeedoffset));
}


/**
* @berief: ��������ٶ����ߣ��û���λ��
*/
void AxisSetSpd(u8 AxNum, AxspdDef spd)
{
	u32 start,acc,run,dec,end,Hfast,Hslow,Hoffset;
	start = ConverToPulse(AxNum,spd.startspeed);
	acc = spd.acctime;
	run = ConverToPulse(AxNum,spd.runspeed);
	dec = spd.dectime;
	end = ConverToPulse(AxNum,spd.endspeed);
	//��չ��  ����ֻ����ôд
	Hfast = ConverToPulse(AxNum,GSS.axis[AxNum].Axhomecfg.homespeedfast);
	Hslow = ConverToPulse(AxNum,GSS.axis[AxNum].Axhomecfg.homespeedslow);
	Hoffset = ConverToPulse(AxNum,GSS.axis[AxNum].Axhomecfg.homespeedoffset);
	if(start>3000)
		start = 1000;
	if(acc > 1000)
		acc = 80;
	if(dec >1000)
		dec = 80;
	if(end >3000)
		end = 1000;
	if(run>300000)
		run = 300000;
	if(run<=start)
	{
		run = start +1;
	}
	if(run<=end)
	{
		run = end +1;
	}
	
	if(AxNum >= PULS_NUM)
	{
		HZ_ExAxSetPara(AxNum-PULS_NUM,start,acc,run,dec,end,\
					   Hfast,Hslow,Hoffset,0,0);
	}
	else 
	{
		HZ_AxSetCurve(AxNum,start,acc,run,dec,end,0);
	}
}

