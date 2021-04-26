#include "InterLayer.h"
#include "Algorithm.h"
#include "can_master.h"
#include "usertool.h"
#include "iodefine.h"

void Axis_pos(void);

//ÿ����ѯ�Ľӿں���
void InterLayer()
{
    //��ȡrtcʱ��
    RTC_Get_Time(&GSR.time.hour, &GSR.time.min, &GSR.time.sec, &GSR.time.ampm);
    RTC_Get_Date(&GSR.date.year,&GSR.date.month,&GSR.date.day,&GSR.date.week);
    //�ײ��ᶯ����
    HZ_AxMotion();
    //modbusͨѶ����
    HZ_ModBus(uart_232, 1);
    HZ_ModBus(uart_485,1);
    //�����ʱ��
    HZ_TimerThread();
    //���ö���岹
    //HZ_MultiAxMotion();
    //��ַ���еı�������
    HZ_Alarm();
    //��ȡ�ᵱǰ״̬
    Axis_pos();
}
/**
* @author  yang
* @Description: ��ȡ�ᵱǰ״̬
* @param -- �û��Լ������ڲ���Ҫ��ȡ������
* @param --
* @return --
*/
void Axis_pos()
{
    int i;
    for(i=0; i<PULS_NUM; i++)//6����
    {
        GSR.AxisPosition[i]= HZ_AxGetCurPos(i);
        GSR.AxisUnitPosition[i] = ConvertToUserUnit(i, GSR.AxisPosition[i]);
    }
    for(i=0; i<PULS_NUM_Ex; i++)//��չ��
    {
        GSR.AxisPosition[i+PULS_NUM] = HZ_ExAxGetCurPos(i);
        GSR.AxisUnitPosition[i] = ConvertToUserUnit(i, GSR.AxisPosition[i]);
    }
	for(i=0; i<ECD_NUM; i++)//3
	{
		GSR.AxisEncoder[i] = EnCode_Get32(i)/2;
	}
	GUW.DisplayData.Data.PlatePosCmd = (u32)GSR.AxisPosition[AxPlate]%PPR_PLATE;
	GUW.DisplayData.Data.PlatePosEnc = (u32)GSR.AxisEncoder[0]%PPR_PLATE;
}

/**
* @author  yang
* @Description: �Դ��㶯���ú�������ִ�е㶯ʱ����
* @param --
* @param --
* @return --
*/
void JogGo(u8 axisnum,s32 pos,u32 spd)
{
	float movepos = pos+GSR.AxisUnitPosition[axisnum];
	
	if(movepos>GSS.axis[axisnum].Axlimitcfg.softmaxlimit)
	{
		movepos = GSS.axis[axisnum].Axlimitcfg.softmaxlimit;
	}else if(movepos<0)
	{
		movepos = 0;
	}
	SetAxisSpdRadio(axisnum, spd);	//�����ٶ�
    if(axisnum<PULS_NUM)	//ֻ���������ֵ㶯����
    {
        if(pos > 0)	//����㶯
        {
            if(AXSTA_ERRSTOP == HZ_AxGetStatus(axisnum))
            {
                //ֻ�����޴���
                if(0 == (0x0fff & HZ_AxGetAxisErr(axisnum)))
                {
                    HZ_AxReset(axisnum);
                    HZ_AxMoveRel(axisnum,pos);
                }
            } else
            {
                //û�б���,�����˶�
                HZ_AxMoveRel(axisnum,pos);
            }
        }
        else	//����㶯
        {
            if(AXSTA_ERRSTOP == HZ_AxGetStatus(axisnum))
            {
                //ֻ�����޴���
                if(0 == (0xf0ff & HZ_AxGetAxisErr(axisnum)))
                {
                    HZ_AxReset(axisnum);
                    HZ_AxMoveRel(axisnum,pos);
                }
            } else {
                //û�б���,�����˶�
                HZ_AxMoveRel(axisnum,pos);
            }
        }
    }else 
	{
		if(pos > 0)	//����㶯
        {
            if(AXSTA_ERRSTOP == HZ_ExAxGetStatus(axisnum-PULS_NUM))
            {
                //ֻ�����޴���
                if(0 == (0x0fff & HZ_ExAxGetErr(axisnum-PULS_NUM)))
                {
                    HZ_ExAxMoveRel(axisnum-PULS_NUM,pos);
                }
            } else {
                //û�б���,�����˶�
                HZ_ExAxMoveRel(axisnum-PULS_NUM,pos);
            }
        }
        else	//����㶯
        {
            if(AXSTA_ERRSTOP == HZ_ExAxGetStatus(axisnum-PULS_NUM))
            {
                //ֻ�����޴���
                if(0 == (0xf0ff & HZ_ExAxGetErr(axisnum-PULS_NUM)))
                {
                    HZ_ExAxMoveRel(axisnum-PULS_NUM,pos);
                }
            } else {
                //û�б���,�����˶�
                HZ_ExAxMoveRel(axisnum-PULS_NUM,pos);
            }
        }
	}
}
//ϵͳ�㶯ֹͣ���ú���
void jogstop(u32 axisnum)
{
    if(axisnum<PULS_NUM)
        HZ_AxStop(axisnum);
    else
        HZ_ExAxStop(axisnum-PULS_NUM);
}
//ϵͳ������ú���
void joghome(u32 axisnum)
{
	if(axisnum<PULS_NUM)
	{
		HZ_AxReset(axisnum);
		HZ_AxHome(axisnum);
	}else 
	{
		HZ_ExAxHome(axisnum-PULS_NUM);
	}
}
/**
* @author  yang
* @Description: ��չ���������״̬��Modbus���£���Modbus��ѯ��ָ����ַ�����
* @param --
* @param --
* @return --
*/
void ex_inputupdata()
{
    u8 i;
    //��չ��1
    GSR.InputStatus[4] = 0;
    GSR.InputStatus[5] = 0;
    GSR.InputStatus[6] = 0;
    GSR.InputStatus[7] = 0;
    for(i=0; i<32; i++){
        GSR.InputStatus[4] |=(u32) HZ_ExInPutGet(0,i)<<i;
    }

}

void ex_outputstatusupdata()
{
    u8 i;
    //��չ�����1
    GSW.OutputStatus[4] = 0;
    for(i = 0; i < 32; i ++){
        GSW.OutputStatus[4]	|= (u32)HZ_ExOutPutGet(0,i) <<	i;
	}

}

void ex_outputupdata()
{
    u8 i;
    //��չ�����1
    for(i = 0; i < 32; i ++){
        HZ_ExOutPutSet(0,i, (GSW.OutputStatus[4]>>(i)&0x01));
    }
}

