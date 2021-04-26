/*
 * @Author: your name
 * @Date: 2020-05-26 15:09:21
 * @LastEditTime: 2020-06-30 21:27:56
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: \YD_KZJ\Code\APP\Tool\AxisMove.h
 */
#ifndef __AXISMOVE_H
#define __AXISMOVE_H
#include "logic.h"


enum {
    GOHOMETYPE1,	     //ֱ�ӻ�ԭ��(ƽ��ģʽ)
    GOHOMETYPE2,		//�ȵ�����λ�ٻ�ԭ��
    GOHOMETYPE3,		//�Ȼ�����λ�ٻ�ԭ��
    GOHOMETYPE4,        //z����ԭ������
    GOHOMETYPE5,        //z����ԭ�㷴��
};

typedef enum
{
    AX_ERR_AX_ALM = (1 << 0),		//�ᱨ��
    AX_ERR_AX_TMOUT = (1 << 1),		//��ʱ����ԭ�㣩
    AX_ERR_UP_LIM_H = (1 << 8),		//Ӳ����λ
    AX_ERR_UP_LIM_S = (1 << 9),		//������λ
    AX_ERR_DW_LIM_H = (1 << 12),		//Ӳ����λ
    AX_ERR_DW_LIM_S = (1 << 13),		//������λ
} AxErrBitTp;

typedef enum
{
    AXSTA_READY = 0,	//����״̬
    AXSTA_STOP,			//ֹͣ״̬
    AXSTA_STOPDEC,		//ֹͣ״̬(����ͣ)
    AXSTA_DISCRETE,		//��ͨ�˶�״̬
    AXSTA_CONTINUOUS,	//�����˶�
    AXSTA_HOMING,		//���ڻ�ԭ��
    AXSTA_DISABLE,		//δ����״̬
    AXSTA_ERRSTOP,		//����ͣ
    AXSTA_SYNC,			//��ͬ��״̬
} AxStateTp;

typedef enum	//��Ŷ���
{
	AxAdjust1,
	AxAdjust2,
	AxBelt,
	AxPlate,
} AxisNum;

typedef enum	//�ᶯģʽѡ��
{
    ABSMODE,	//0
    RELMODE,	//1
    SPDMODE,	//2
    GOHOMEMODE,	//3
    STOPMODE,	//4
} MoveMode;

/*********  ���ƶ��ӿ� *********/
s32 HZ_MoveUnit(u32 num,u32 speed,float pos,u32 mode);
void SetAxisSpdRadio(u8 i,u8 spd);
/******  ��������ƶ��ӿ� ******/
s32 UserGetStatus(u8 AxNum);
void UserGoHome(u8 AxNum);


//�����ƶ��ӿ� Mr.Yiu
extern void AxAbs(u8 num, s32 pos)      ;
extern void AxRel(u8 num, s32 pos)      ;
extern void AxAbsSpd(u8 num, s32 pos, u8 spd)      ;
extern void AxRelSpd(u8 num, s32 pos, u8 spd)      ;
extern void AxHome(u8 num)              ;
extern AxStateTp AxSta(u8 num);         ;

#endif
