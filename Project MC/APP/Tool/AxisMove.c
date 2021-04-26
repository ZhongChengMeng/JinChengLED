#include "axismove.h"
#include "userTool.h"
#include "bsplib.h"
#include "interlayer_config.h"
#include "datadefine.h"
#include "AxisConfig.h"
#include "can_master.h"

/**********************************************************************/
/***************		���ƶ��ӿ�						***************/
/**********************************************************************/
/* <1> ���������û���λ */
/**
* @author 19/02/19  yang
* @Description: ���ƶ��ӿ�
* @param -- ��� �ٶ� λ�� ģʽ
* @param --  �ٶ� �ٷֱ�
* @return --
*/
s32 HZ_MoveUnit(u32 num,u32 speed,float pos,u32 mode)
{
    if(num>PULS_NUM+PULS_NUM_Ex)
        return -1;
    SetAxisSpdRadio(num, speed);	//�����ٶ�
    switch(mode)
    {
    case ABSMODE:
        HZ_AxMoveAbs(num,ConverToPulse(num, pos));
        break;
    case RELMODE:
        HZ_AxMoveRel(num, ConverToPulse(num, pos));
        break;
    case SPDMODE:
        HZ_AxMoveVelocity(num, ConverToPulse(num, pos));
        break;
    case GOHOMEMODE:
        HZ_AxHome(num);
        break;
    default:
        break;
    }
    return 0;
}
/*
 * Created: 11/16
 * Description:�����������û��ٶ�
 * param:
 * Modified By:yang
 */
void SetAxisSpdRadio(u8 i,u8 spd)
{
    if(spd>=100)
    {
        spd = 100;
    }
    if(spd == 0)
    {
        spd = 1;
    }
	AxspdDef Axspd;
	Axspd = GSS.axis[i].AxSpd;
	float radio = (float)spd / 100.0f;
	Axspd.runspeed = Axspd.runspeed * radio;
	AxisSetSpd(i,Axspd);
}


/**********************************************************************/
/***************		���ⶨ������ƶ��ӿ�			***************/
/**********************************************************************/
/**
* @author 18/12/14  ����
* @Description: ��ȡ��״̬
* @param 	AxNum:������������Ϊ��չ��
* @param --
* @return --
*/
s32 UserGetStatus(u8 AxNum)
{
	if (AxNum < PULS_NUM)
	{
		return HZ_AxGetStatus(AxNum);
	}
	else
	{
		return HZ_ExAxGetStatus(AxNum - PULS_NUM);
	}
}

/**
* @author 19/02/18  wangh
* @Description: ��ԭ�㺯��
* @param AxNum:������������Ϊ��չ��
* @param --
* @return --
*/
void UserGoHome(u8 AxNum)
{
    if (AxNum < PULS_NUM)
    {
        HZ_AxReset(AxNum);
        HZ_AxHome(AxNum);
    }
    else
    {
		HZ_ExAxHome(AxNum - PULS_NUM);
    }
}

/**
* @author Mr.Yiu	2021.1.20
* @Description: ȫ����ABS����
* @param 
* @param --
* @return --
*/
void AxAbs(u8 num, s32 pos)
{
    if(num>PULS_NUM+PULS_NUM_Ex)
        return;
    SetAxisSpdRadio(num, 100);	//�����ٶ�
	HZ_AxMoveAbs(num,pos);
}

/**
* @author Mr.Yiu	2021.1.20
* @Description: ȫ����Rel����
* @param 
* @param --
* @return --
*/
void AxRel(u8 num, s32 pos)
{
    if(num>PULS_NUM+PULS_NUM_Ex)
        return;
    SetAxisSpdRadio(num, 100);	//�����ٶ�
	HZ_AxMoveRel(num,pos);
}

/**
* @author Mr.Yiu	2021.1.20
* @Description: ȫ����ABS����
* @param 
* @param --
* @return --
*/
void AxAbsSpd(u8 num, s32 pos, u8 spd)
{
    if(num>PULS_NUM+PULS_NUM_Ex)
        return;
    SetAxisSpdRadio(num, spd);	//�����ٶ�
	HZ_AxMoveAbs(num,pos);
}

/**
* @author Mr.Yiu	2021.1.20
* @Description: ȫ����Rel����
* @param 
* @param --
* @return --
*/
void AxRelSpd(u8 num, s32 pos, u8 spd)
{
    if(num>PULS_NUM+PULS_NUM_Ex)
        return;
    SetAxisSpdRadio(num, spd);	//�����ٶ�
	HZ_AxMoveRel(num,pos);
}

/**
* @author Mr.Yiu	2021.1.20
* @Description: �����
* @param 
* @param --
* @return --
*/
void AxHome(u8 num)
{
    if(num>PULS_NUM+PULS_NUM_Ex)
        return;
	HZ_AxHome(num);
}

/**
* @author Mr.Yiu	2021.1.20
* @Description: ��״̬��ѯ
* @param 
* @param --
* @return --
*/
AxStateTp AxSta(u8 num)
{
    if(num>PULS_NUM+PULS_NUM_Ex)
        return AXSTA_READY;
	return (AxStateTp)HZ_AxGetStatus(num);
}




