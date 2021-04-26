/*
 * @Descripttion: 
 * @version: 
 * @Author: yfs
 * @Date: 2020-02-24 10:31:18
 * @LastEditors: Please set LastEditors
 * @LastEditTime: 2020-06-01 14:55:24
 */
#ifndef _LOGIC_H_
#define _LOGIC_H_

#include "DataDefine.h"
#include "bsplib.h"
#include "interlayer_config.h"
#include "FSM.h"
#include "string.h"
#include "logicdata.h"
#include "AxisMove.h"
#include "Timer.h"
#include "Trigger.h"
#include "GpioManager.h"
#include "Film.h"

#define PARAINIT(a)  		memset(&a,0,sizeof(a));

typedef struct //ϵͳ�������
{
	LogicParaDef AxisMoveTask[PULS_NUM];
	
	LogicParaDef	BeforeRunTask;			//����ǰ�Ķ���
	LogicParaDef	AfterStopTask;			//ֹͣ��Ķ���
	LogicParaDef	MainProcessTask;		//������
	LogicParaDef	IndicatorTask;			//״̬��ʾ����
	
	/****************************************
    App.Plate ת���ϵ�����
    ****************************************/
	LogicParaDef	AdjustHalfTask[2];		//�����벽
	LogicParaDef	AdjustFullTask[2];		//��������
	LogicParaDef	AdjustHomeTask[2];		//��������
	LogicParaDef	Blow1Task;				//���ϵ���NG
	LogicParaDef	Blow2Task;				//���ϸ����Ͽ�����
	LogicParaDef	Blow3Task;				//����ĩ��NG
	LogicParaDef	Blow4Task;				//����ĩ��OK
	LogicParaDef	Detect1Task;			//����������ϸ�װ��
	LogicParaDef	Detect2Task;			//��鲹��ʱ�Ƿ�ȡ��
	LogicParaDef	DownCameraTask;			//�����ʶ��
	LogicParaDef	ElecTestTask;			//���Բ���
	LogicParaDef	ElecTurnTask;			//������ת
	LogicParaDef	FeedPlateTask;			//���ϸ�ת��
	LogicParaDef	FeederRoundTask;		//Բ��
	LogicParaDef	FeederFlatTask;			//ֱ��
	LogicParaDef	FeederHopperTask;		//�϶�
	LogicParaDef	PlateRunTask;			//ת������
	LogicParaDef	PlateTrigTask;			//ת��λ�ô���
	LogicParaDef	PlateHomeTask;			//ת�̻���
	LogicParaDef	ClearTask;				//����
	LogicParaDef	CleanTask;				//��ϴ
	LogicParaDef	ElecTest2Task;			//����2վ
	
	/****************************************
    App.Pack ����ϵ�����
    ****************************************/
	LogicParaDef	PackTask;				//װ��
	LogicParaDef	PackBeforTask;			//ǰ��
	LogicParaDef	PackBehindTask;			//���
	LogicParaDef	PackSampleTask;			//����Ʒ
	LogicParaDef	PackSampleBehindTask;	//��Ʒ���
	LogicParaDef	RepackTask;				//����
	LogicParaDef	BeltMoveTask;			//�ߴ�
	LogicParaDef	BeltKeepMoveTask;		//�����ߴ�
	LogicParaDef	BeltSupplyTask;			//�ʹ�
	LogicParaDef	BeltHomeTask;			//�ߴ�����
	LogicParaDef	PressTask;				//���
	LogicParaDef	PressUpTask;			//�⵶̧��
	LogicParaDef	CutTask;				//�д�
	LogicParaDef	UpCameraTask;			//�����ʶ��
	
} Task;
extern Task LogicTask;

typedef enum
{
	Slow,			//����ģʽ
	Fast,			//����ģʽ
	StopToReady,	//ֹͣȥ��Ԥ��λ
	StopRightNow	//����ֹͣ
}PlateMode;

typedef struct //ϵͳ�������ñ���
{
	struct
	{
		s32	Num;	//�ߴ�����	
		s32	Dir;	//�ߴ�����	1:zheng -1:��
		s32	Spd;	//�ߴ��ٶ�	�ٷֱ��ٶ�
	}BeltMoveTemp;
	struct
	{
		PosTrigDef DownStart;
		PosTrigDef DownEnd;
		PosTrigDef UpStart;
		PosTrigDef UpEnd;
		PosTrigDef BlowEndNg;
		PosTrigDef FeedStart;
		PosTrigDef FeedEnd;
	}PosTrig;
	
	struct
	{
		PlateMode	Mode;	//0:Slow 1:Fast	2:ͣ��Ԥ��λ 3������ֹͣ
		s32	IntoFastNum;	//������ٵ�����
		s32	IntoSlowNum;	//�������ٵ�����
	}PlateRun;				//ת������
	
	struct
	{
		s32	Dir;//-1������	1������
		s32	Pos;//һȦ�ڵ�ָ��λ��
	}PlateGotoRelPos;
	
	struct
	{
		s32	CameraChkIdx;//��������ǿ�����������
		s32	RemainNum;//���Ͽڵ���������
		s32	TakeInp;//ȡ��Ҫ��������
	}RePack;
	
	struct
	{
		u32	CurDir;
	}ETurn;
	
} SysData;
extern SysData Data;

extern void Logic(void);

#define	LT	LogicTask

#define	my (*p)
#define	TASKRUN(LTXX) \
	LogicParaDef *p = &LTXX;\
	if(my.execute == TEXE && my.step == 0)\
	{\
		TimerRst(&my.tm);\
		my.step = 1;\
		my.done = 0;\
	}\
	switch(my.step)

#define	TASKOK\
	my.Exetime = TimerCnt(&my.tm,1);\
	my.execute = TIDLE;\
	my.step = 0;\
	my.done = 1;\
	my.Manual = 0

#define	TASKNG\
	my.Exetime = TimerCnt(&my.tm,1);\
	my.execute = TIDLE;\
	my.step = 0;\
	my.done = 0;\
	my.Manual = 0

//����ܳ���
#define	BeltTotalNum	(GUS.ProductData.Data.Num_NoPress + \
							GUS.ProductData.Data.Num_BeforPack + \
							GUS.ProductData.Data.Num_Pack + \
							GUS.ProductData.Data.Num_BehindPack + \
							GUS.ProductData.Data.Num_Sample + \
							GUS.ProductData.Data.Num_SampleBehind)
							
	
extern void AxisDataConfig(void);

#endif
