 /*
 * File: Logic.c
 * File Created: Tuesday, 9th October 2018 2:43:29 pm
 * version: v00.00.01 8轴底层
 * Description:用户逻辑
 * Modified By:yang
 * -----
 * Copyright 2018 - 2018, <<hzzh>>
 */

#include "Logic.h"
#include "Plate.h"
#include "Adjust.h"
#include "Blow.h"
#include "Detect.h"
#include "DownCamera.h"
#include "ElecTest.h"
#include "ElecTurn.h"
#include "Feed.h"
#include "Clean.h"
#include "Belt.h"
#include "Cut.h"
#include "Film.h"
#include "Pack.h"
#include "Press.h"
#include "Repack.h"
#include "UpCamera.h"

#define PI 3.1415926

/*全局变量做统一处理*/
Task LogicTask; //统一的任务类
SysData Data;  //统一的数据类

//轴配置
void AxisDataConfig()
{
	for(int i=0; i<PULS_NUM; i++)
	{
		GSS.axis[i].AxSpd.acctime = GUS.MotorData.Data.AccDecTime[i];//加速时间
		GSS.axis[i].AxSpd.dectime = GUS.MotorData.Data.AccDecTime[i];//减速时间
		GSS.axis[i].Axhomecfg.orglev = GUS.MotorData.Data.OriginDir[i];//原点信号电平
		GSS.axis[i].Axhomecfg.homespeedfast = GUS.MotorData.Data.HomeSpeed[i];//回原点快速
		if(GSW.JogForward[0] == 0 && GSW.JogBackward[0] == 0)//没有点动时，设置为运行速度
		{
			GSS.axis[i].AxSpd.startspeed = 1000;
			GSS.axis[i].AxSpd.endspeed = 1000;
			GSS.axis[i].AxSpd.runspeed = GUS.MotorData.Data.RunSpeed[i];
		}
		else//有点动时，设置为点动速度
		{
			GSS.axis[i].AxSpd.startspeed = 1;
			GSS.axis[i].AxSpd.endspeed = 1;
			GSS.axis[i].AxSpd.runspeed = GUS.MotorData.Data.JogSpeed[i];
		}
		AxDirCfg(i,GUS.MotorData.Data.MotorDir[i]);
	}
	GUS.MotorData.Data.RunSpeed[AxPlate] = 100000*PPR_PLATE/3600;//转盘运行速度固定100kph，其他地方用的时候设置速度比例，比例就是Kp/h
	AxisConfig(GSS.axis);
}

//设备按钮
void Button()
{
	static TrigDef Tr_Run,Tr_Stop,Tr_ScramBtn;
	static u32 Tm_Scram;
	if(TrigOne(&Tr_Run,I_Get(I_StartBtn) == ON,50))//开始按钮
	{
		FSM_CMD = RUN;
	}
	if(TrigOne(&Tr_Stop,I_Get(I_StopBtn) == ON,50))//停止按钮
	{
		GSW.ClearAlarm = 1;
		FSM_CMD = STOP;
	}
	if(TimerCnt(&Tm_Scram,I_Get(I_ScramBtn) == OFF) > 50)//急停按下
	{
		FSM_CMD = SCRAM;
	}
	if(TrigOne(&Tr_ScramBtn,I_Get(I_ScramBtn) == ON,50))//急停弹起
	{
		InitFsm(&SysFsm);
	}
	
}

//零散报警
void Alarm()
{
	static u32 tm_ChkBelt,tm_BeltCollect;
	if(TimerCnt(&tm_ChkBelt,I_Get(I_ChkBelt) == ON) > 3000)
	{
		AlarmSetBit(2,0,9);//载带感应持续有信号，载带用完，或载带卡带
	}
	
	if(FSM_STA == RUN)
	{
		static u32 tm_ChkFilm;
		if(TimerCnt(&tm_ChkFilm,I_Get(I_ChkFilm) == ON) > 200)
		{
			AlarmSetBit(2,0,24);//胶膜用完信号感应
		}
		
		static u32 tm_ChkPack;
		if(TimerCnt(&tm_ChkPack,I_Get(I_ChkPack) == OFF) > 60)
		{
			AlarmSetBit(2,0,25);//装填卡料
		}
		//收带电机未开启
		if(GUS.RecordData.Data.Num_Packed > GUW.ProcessData.Data.Num_BeltCollect && \
			GUW.ProcessData.Data.Num_BeltCollect != 0)
		{
			if(TimerCnt(&tm_BeltCollect,Q_Get(Q_BeltCollect) == OFF) > 0)
			{
				AlarmSetBit(2,0,27);//收带电机未开启
			}
		}
		static u32 tm_TempErr;
		if(TimerCnt(&tm_TempErr,I_Get(I_TempErr) == ON) > 0)
		{
			AlarmSetBit(2,0,28);//温控异常
		}
	}
}

//状态指示灯和报警器
void Indicator()
{
	static u32 tm;
	static TrigDef tr_Run,tr_Stop;
	if(TrigOne(&tr_Run,FSM_STA == RUN,0))//运行状态时
	{
		Q_Set(Q_Green,ON);//绿灯亮
		Q_Set(Q_Yellow,OFF);//黄灯灭
	}
	if(TrigOne(&tr_Stop,FSM_STA != RUN,0))//非运行状态时
	{
		Q_Set(Q_Green,OFF);//绿灯灭
		Q_Set(Q_Yellow,ON);//黄灯亮
	}
	if(GSR.ErrorLevel && GUR.FsmData.Data.HaveToReset == 0)//有报警时
	{
		LT.IndicatorTask.execute = TEXE;//执行报警闪烁流程
	}
	TASKRUN(LT.IndicatorTask)
	{
		case 1:
			Q_Set(Q_Red,ON);//红灯亮
			if(GUS.EnableData.Data.Beep == 1)//如果开启报警器
			{
				Q_Set(Q_Beep,ON);//报警器响
			}
			TimerRst(&tm);
			my.step = 2;
			break;
			
		case 2:
			if(TimerCnt(&tm,1) > 300)//300ms后
			{
				Q_Set(Q_Red,OFF);//灭红灯
				Q_Set(Q_Beep,OFF);//停报警器
				TimerRst(&tm);
				my.step = 3;
			}
			break;
			
		case 3:
			if(TimerCnt(&tm,1) > 300)
			{
				TASKOK;
			}
			break;
	}
}

//运行前准备动作
void BeforeRun()
{
	static u32 tm;
	TASKRUN(LT.BeforeRunTask)
	{
		case 1:
			Q_Set(Q_PressUpCyd,ON);//上气缸下压
			Q_Set(Q_FeederFlat,ON);//直振开启
			Q_Set(Q_FeederRound,ON);//圆振开启
			TimerRst(&tm);
			my.step = 2;
			break;
		
		case 2:
			if(TimerCnt(&tm,1) > 500)//500ms后
			{
				LT.UpCameraTask.execute = TEXE;//上相机执行
				my.step = 3;
			}
			break;
			
		case 3:
			if(LT.UpCameraTask.execute == TIDLE)//等待上相机结束
			{
				if(LT.UpCameraTask.done == 1)//如果物料ok
				{
					TASKOK;
				}
				else
				{
					if(GUS.EnableData.Data.RePack == 1)//如果使能补料
					{
						LT.PlateTrigTask.execute = TEXE;//开启转盘触发
						LT.RepackTask.execute = TEXE;//补料
						my.step = 4;
					}
					else
					{
						AlarmSetBit(2,0,11);//装填视觉NG，未开启自动补料，请人工处理
						TASKNG;
					}
				}
			}
			break;
			
		case 4:
			if(LT.RepackTask.execute == 0)//等待补料结束
			{
				if(LT.RepackTask.done == 1)//补料成功
				{
					TASKOK;
				}
				else//补料失败
				{
					TASKNG;
				}
			}
			break;
	}
}

void FileNameRead()
{
	
	
}
//停止后动作
void AfterStop()
{
	static TrigDef tr;
	if(TrigOne(&tr,FSM_STA != RUN,1000))//进入停止后1s后
	{
		LT.AfterStopTask.execute = TEXE;
	}
	TASKRUN(LT.AfterStopTask)
	{
		case 1:
			Q_Set(Q_PressUpCyd,OFF);//封带上气缸上升
			Q_Set(Q_FeederFlat,OFF);//关闭振动盘
			Q_Set(Q_FeederRound,OFF);//关闭圆振
			my.execute = TASKOK;
			break;
	}
}

//设备主流程
void MainProcess()
{
	u32 i;
	if(FSM_STA == RUN)
	{
		LT.MainProcessTask.execute = TEXE;
	}
	TASKRUN(LT.MainProcessTask)
	{
		case 1:
			for(i=0; i<10; i++)
			{
				//如果有料带走到
				if(GUS.RecordData.Data.PackStep[i] > 0 && GUS.RecordData.Data.PackStep[i] < BeltTotalNum)
				{
					break;
				}
			}
			if(i >= 10)//没有正在进行的料带
			{
				LT.PackBeforTask.execute = TEXE;//走前编，前编时会进新卷数据
			}
			my.step = 2;
			break;
		
		case 2:
			if(LT.PackBeforTask.execute == TIDLE)
			{
				LT.BeforeRunTask.execute = TEXE;//运行前动作
				my.step = 3;
			}
			break;
		
		case 3:
			if(LT.BeforeRunTask.execute == TIDLE && FSM_STA == RUN)
			{
				if(LT.BeforeRunTask.done == 1)//运行前准备完成
				{
					LT.PlateTrigTask.execute = TEXE;//开启转盘触发
					LT.PlateRunTask.execute = TEXE;//转盘控制开始
					my.step = 4;
				}
				else//运行前准备异常
				{
					TASKNG;
				}
			}
			break;
		
		case 4:
			//判断停止条件，停机、装填数量
			if((FSM_STA != RUN || GUS.RecordData.Data.Num_Packed >= (GUS.ProductData.Data.Num_Pack-1)) && LT.PackTask.execute != TEXE)
			{			
				Data.PlateRun.Mode = StopToReady;//转盘停到预备位
				my.step = 5;
			}
			break;
			
		case 5:
			if(LT.PlateRunTask.execute == TIDLE)//转盘控制结束
			{
				LT.PlateTrigTask.execute = TIDLE;//关闭转盘触发
				if(GUS.RecordData.Data.Num_Packed >= GUS.ProductData.Data.Num_Pack)//判断满料
				{
					my.step = 6;
				}
				else
				{
					TASKOK;
				}
			}
			break;
			
		case 6:
			if(FSM_STA == RUN)
			{
				LT.PackBehindTask.execute = TEXE;//后编
				my.step = 7;
			}
			break;
			
		case 7:
			if(my.execute == TSTOP)//后编时接到停止命令
			{
				LT.PackBehindTask.execute = TSTOP;//结束后编动作
				my.step = 11;//跳到结束步骤
				break;
			}
			if(LT.PackBehindTask.execute == TIDLE)//后编正常结束
			{
				LT.PackSampleTask.execute = TEXE;//编样品
				my.step = 8;
			}
			break;
			
		case 8:
			if(my.execute == TSTOP)//编样品时收到停止命令
			{
				LT.PackSampleTask.execute = TSTOP;//结束样品动作
				my.step = 11;//跳到结束步骤
				break;
			}
			if(LT.PackSampleTask.execute == TIDLE)//样品正常结束
			{
				LT.PackSampleBehindTask.execute = TEXE;//样品后编
				my.step = 9;
			}
			break;
			
		case 9:
			if(my.execute == TSTOP)//样品后编时收到停止命令
			{
				LT.PackSampleBehindTask.execute = TSTOP;//结束样品后编动作
				my.step = 11;
				break;
			}
			if(LT.PackSampleBehindTask.execute == TIDLE)//样品后编正常结束
			{
				LT.CleanTask.execute = TEXE;//清理吸嘴
				my.step = 10;
			}
			break;
			
		case 10:
			if(LT.CleanTask.execute == TIDLE)//清理吸嘴任务完成
			{
				TASKOK;//一条编带结束
			}
			break;
			
		case 11:
			if( LT.PackBehindTask.execute == TIDLE &&
				LT.PackSampleTask.execute == TIDLE &&
				LT.PackSampleBehindTask.execute == TIDLE)
			{
				TASKNG;
			}
			break;
	}
}

//读取测试时间
void TaskTime()
{
	GUR.TimeRecord.Data.AdjustTime[0] = LT.AdjustFullTask[0].Exetime;//矫正1时间
	GUR.TimeRecord.Data.AdjustTime[1] = LT.AdjustFullTask[1].Exetime;//矫正2时间
	GUR.TimeRecord.Data.DownCameraTime = LT.DownCameraTask.Exetime;//下相机视觉检测时间
	GUR.TimeRecord.Data.ElecTest1Time = LT.ElecTestTask.Exetime;//测试1时间		测试1所用时间
	GUR.TimeRecord.Data.ElecTest2Time = LT.ElecTest2Task.Exetime;//测试1时间		测试1所用时间
	GUR.TimeRecord.Data.ElecTurn90Time = LT.ElecTurnTask.Exetime;//旋转90时间		旋转工位90°时间
	GUR.TimeRecord.Data.PackMotorTime = LT.BeltMoveTask.Exetime;//装填电机时间	 装填电机一格
	GUR.TimeRecord.Data.PackPressTime = LT.PressTask.Exetime;//装填封带时间	 封带周期
	GUR.TimeRecord.Data.UpCameraTime = LT.UpCameraTask.Exetime;//装填相机检测时间 检测周期
	static TrigDef tr_PlateDownStart,tr_PlateDownEnd;
	static u32 tm_PlateDown;
	if(PlateTrig(&tr_PlateDownStart,GUS.PlateData.Data.Pos_DownCameraStart))
	{
		TimerRst(&tm_PlateDown);
	}
	if(PlateTrig(&tr_PlateDownEnd,GUS.PlateData.Data.Pos_DownCameraEnd))
	{
		GUR.TimeRecord.Data.PlateDownTime = TimerCnt(&tm_PlateDown,1);
	}//记录转一圈的时间
	
	GUS.ProductData.Data.Num_ReelPack = GUS.ProductData.Data.Num_BeforPack + GUS.ProductData.Data.Num_BehindPack + GUS.ProductData.Data.Num_NoPress + \
										GUS.ProductData.Data.Num_Pack + GUS.ProductData.Data.Num_Sample + GUS.ProductData.Data.Num_SampleBehind;
}
//2020.7.20
//设备逻辑循环
void Logic()
{
	
	AxisDataConfig();		//轴参数配置
	Button();				//按钮处理
	Alarm();				//错误报警
	Indicator();			//灯 与  蜂鸣器
	TaskTime();				//时间记录
	
	BeforeRun();			//运行前动作
	AfterStop();			//停止后动作
	MainProcess();			//主任务	
	
	/**** 转盘工位相关 ****/
	Plate();				//转盘
	Adjust();				//矫正
	Blow();					//吹气
	Detect();				//转盘工位感应器
	DownCamera();			//下相机检测
	ElecTesk();				//电性测试
	ElecTurn();				//电性旋转
	Feed();					//料斗 圆振 直振
	Clean();				//转盘清洁
	
	/**** 编带相关 ****/		
	Belt();					//料带
	Cut();					//切带
	Pack();					//装填
	Press();				//封带
	Repack();				//补料
	UpCamera();				//上相机检测
	
	/****文件相关****/
	File_operator();		//读取U盘文件
}

