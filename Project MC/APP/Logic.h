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

typedef struct //系统任务队列
{
	LogicParaDef AxisMoveTask[PULS_NUM];
	
	LogicParaDef	BeforeRunTask;			//运行前的动作
	LogicParaDef	AfterStopTask;			//停止后的动作
	LogicParaDef	MainProcessTask;		//主任务
	LogicParaDef	IndicatorTask;			//状态显示任务
	
	/****************************************
    App.Plate 转盘上的任务
    ****************************************/
	LogicParaDef	AdjustHalfTask[2];		//矫正半步
	LogicParaDef	AdjustFullTask[2];		//矫正整步
	LogicParaDef	AdjustHomeTask[2];		//矫正回零
	LogicParaDef	Blow1Task;				//吹料电性NG
	LogicParaDef	Blow2Task;				//吹料给补料空吸嘴
	LogicParaDef	Blow3Task;				//吹料末端NG
	LogicParaDef	Blow4Task;				//吹料末端OK
	LogicParaDef	Detect1Task;			//检查吸嘴有料给装填
	LogicParaDef	Detect2Task;			//检查补料时是否取走
	LogicParaDef	DownCameraTask;			//下相机识别
	LogicParaDef	ElecTestTask;			//电性测试
	LogicParaDef	ElecTurnTask;			//电性旋转
	LogicParaDef	FeedPlateTask;			//进料给转盘
	LogicParaDef	FeederRoundTask;		//圆振
	LogicParaDef	FeederFlatTask;			//直振
	LogicParaDef	FeederHopperTask;		//料斗
	LogicParaDef	PlateRunTask;			//转盘运行
	LogicParaDef	PlateTrigTask;			//转盘位置触发
	LogicParaDef	PlateHomeTask;			//转盘回零
	LogicParaDef	ClearTask;				//清料
	LogicParaDef	CleanTask;				//清洗
	LogicParaDef	ElecTest2Task;			//测试2站
	
	/****************************************
    App.Pack 编带上的任务
    ****************************************/
	LogicParaDef	PackTask;				//装填
	LogicParaDef	PackBeforTask;			//前编
	LogicParaDef	PackBehindTask;			//后编
	LogicParaDef	PackSampleTask;			//编样品
	LogicParaDef	PackSampleBehindTask;	//样品后编
	LogicParaDef	RepackTask;				//补料
	LogicParaDef	BeltMoveTask;			//走带
	LogicParaDef	BeltKeepMoveTask;		//连续走带
	LogicParaDef	BeltSupplyTask;			//送带
	LogicParaDef	BeltHomeTask;			//走带回零
	LogicParaDef	PressTask;				//封带
	LogicParaDef	PressUpTask;			//封刀抬起
	LogicParaDef	CutTask;				//切带
	LogicParaDef	UpCameraTask;			//上相机识别
	
} Task;
extern Task LogicTask;

typedef enum
{
	Slow,			//慢速模式
	Fast,			//快速模式
	StopToReady,	//停止去到预备位
	StopRightNow	//立刻停止
}PlateMode;

typedef struct //系统程序自用变量
{
	struct
	{
		s32	Num;	//走带数量	
		s32	Dir;	//走带方向	1:zheng -1:负
		s32	Spd;	//走带速度	百分比速度
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
		PlateMode	Mode;	//0:Slow 1:Fast	2:停到预备位 3：立即停止
		s32	IntoFastNum;	//进入快速的数量
		s32	IntoSlowNum;	//进入慢速的数量
	}PlateRun;				//转盘运行
	
	struct
	{
		s32	Dir;//-1：反向	1：正向
		s32	Pos;//一圈内的指定位置
	}PlateGotoRelPos;
	
	struct
	{
		s32	CameraChkIdx;//相机检测的那颗物料在哪里
		s32	RemainNum;//入料口的余料数量
		s32	TakeInp;//取到要补的料了
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

//编带总长度
#define	BeltTotalNum	(GUS.ProductData.Data.Num_NoPress + \
							GUS.ProductData.Data.Num_BeforPack + \
							GUS.ProductData.Data.Num_Pack + \
							GUS.ProductData.Data.Num_BehindPack + \
							GUS.ProductData.Data.Num_Sample + \
							GUS.ProductData.Data.Num_SampleBehind)
							
	
extern void AxisDataConfig(void);

#endif
