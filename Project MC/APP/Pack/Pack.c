/*
*@Header:
*@File Name:	Pack.c
*@Author: Yao
*@Data: 2021/03/05
*@Description: 编带:
*					PackBefore()		前编
*					PackBehind()		后编
*					PackSample()		编样品
*					PackSampleBehind()	样品后编
*					GetPackedNum()		计算已经装填的数量
*					Pack()				装填	外部轮询 Logic.c
*/
#include "Logic.h"
#include "Plate.h"
#include "Pack.h"
#include "Belt.h"

/**
* @author 21/03/05  yao
* @Description: 前编
* @param -- 
* @param --  
* @return --
*/
static void PackBefore()
{
	static u32 tm;
	TASKRUN(LT.PackBeforTask)
	{
		case 1:
			if(GUS.AlarmData.Data.Num_NewBelt - GUS.RecordData.Data.Num_PackBlet < BeltTotalNum && GUS.AlarmData.Data.Num_NewBelt != 0)
				//新的 - 已用的 < 一卷数量  并且  一卷数量不为0
			{
				AlarmSetBit(2,0,20);//剩余料带已不足
			}
			if(GUS.AlarmData.Data.Num_NewFilm - GUS.RecordData.Data.Num_PackFilm < BeltTotalNum && GUS.AlarmData.Data.Num_NewFilm != 0)
				//新的 - 已用的 < 一卷数量  并且  一卷数量不为0
			{
				AlarmSetBit(2,0,21);//剩余胶膜已不足
			}
			my.count = 0;
			my.step = 2;
			break;
		
		case 2:
			BeltMoveStart(1,1,100);//装填走带
			my.step = 3;
			break;
		
		case 3:
			if(LT.BeltMoveTask.execute == TIDLE)//走带
			{
				LT.PressTask.execute = TEXE;//封带
				my.step = 4;
			}
			break;
		
		case 4:
			if(LT.PressTask.execute == TIDLE)//封带
			{
				TimerRst(&tm);
				my.step = 5;
			}
			break;
			
		case 5:
			if(TimerCnt(&tm,1) > 50)
			{
				if(my.count == 0)
				{
					for(int i=0; i<10; i++)
					{
						if(GUS.RecordData.Data.PackStep[i] == 0)//卷带为0
						{
							GUS.RecordData.Data.PackStep[i] = 1;
							break;
						}
					}
				}
				my.count++;
				if(my.count < GUS.ProductData.Data.Num_BeforPack + GUS.ProductData.Data.Num_NoPress)
					// 						 前空格数量 +  不封数量
				{
					my.step = 2;
				}
				else
				{
					TASKOK;
				}
			}
			break;
	}
}

/**
* @author 21/03/05  yao
* @Description: 后编
* @param -- 
* @param --  
* @return --
*/
static void PackBehind()
{
	static u32 tm;
	TASKRUN(LT.PackBehindTask)//后编
	{
		case 1:
			my.count = 0;
			my.step = 2;
			break;
		
		case 2:
			if(my.execute == TSTOP)//本任务被杀死
			{
				PARAINIT(my);
				break;
			}
			if(FSM_STA == RUN)//设备状态为运行
			{
				
				BeltMoveStart(1,1,100);//装填走带 格数 方向 速度
				my.step = 3;
			}
			break;
			
		case 3:
			if(LT.BeltMoveTask.execute == TIDLE && LT.UpCameraTask.execute == TIDLE) //走带功能 == 空闲 上相机功能 == 空闲
			{
				/************修改后编的视觉触发*******************/
				if(LT.UpCameraTask.done == 1)	//执行后编的前一步是装填，而装填时已经触发视觉，判断最后一科料装填时触发的视觉结果
				{
					LT.PressTask.execute = TEXE;	//封带启动
					LT.UpCameraTask.execute = TEXE;	//上相机启动
					my.step = 4;
				}
				else
				{
					LT.PressTask.execute = TEXE;	//封带启动
					my.step = 0x31;
				}
			}
			break;
			
		case 0x31:
			if(LT.PressTask.execute == TIDLE)		//封带结束
			{
				BeltMoveStart(1,-1,100);			//装填走带
				my.step = 6;
			}
			break;
		
		case 4:
			if(LT.PressTask.execute == TIDLE)		////封带结束
			{
				TimerRst(&tm);
				my.step = 5;
			}
			break;
			
		case 5:
			if(TimerCnt(&tm,1) > 100)
			{
				my.count++;
				if(my.count < GUS.ProductData.Data.Num_BehindPack)	//后空格数量
				{
					my.step = 2;
				}
				else
				{
					TASKOK;
				}
			}
			break;
			
		case 6:
			if(LT.BeltMoveTask.execute == TIDLE)	//走带功能 == 空闲
			{
				if(GUS.EnableData.Data.RePack == 1)//开启补料功能了
				{
					LT.PlateTrigTask.execute = TEXE;//转盘位置触发
					LT.RepackTask.execute = TEXE;	//补料功能开启
					my.step = 7;
				}
				else
				{
					AlarmSetBit(2,0,11);//装填视觉NG，未开启自动补料，请人工处理
					my.step = 8;
				}
			}
			break;
			
		case 7:
			if(LT.RepackTask.execute == TIDLE)	//补料结束
			{
				LT.PlateTrigTask.execute = TIDLE;////转盘位置触发
				if(LT.RepackTask.done == 1)	//如果补料失败
				{
					my.step = 0x71;
				}
				else
				{
					AlarmSetBit(2,0,8);		//补料失败，请检查并人工补料
					my.step = 8;
				}
				BeltMoveStart(1,1,100);		//补料位置还原
			}
			break;
			
		case 0x71:
			if(LT.BeltMoveTask.execute == TIDLE)	//走带功能 == 空闲
			{
				TimerRst(&tm);
				LT.PressTask.execute = TEXE;	//封带启动
				LT.UpCameraTask.execute = TEXE;	//上相机启动
				my.step = 5;
			}
			break;
			
		case 8:
			if(my.execute == TSTOP)//本任务被杀死
			{
				PARAINIT(my);
				break;
			}
			if(FSM_STA == RUN)//设备状态在运行
			{
				my.step = 3;
			}
			break;
	}
}

/**
* @author 21/03/05  yao
* @Description: 编样品
* @param -- 
* @param --  
* @return --
*/
static void PackSample()
{
	TASKRUN(LT.PackSampleTask)//编样品任务
	{
		case 1:		//初始化计数
			my.count = 0;
			my.step = 2;
			break;
		
		case 2:		
			if(my.execute == TSTOP)//本任务被叫停
			{
				LT.PlateTrigTask.execute = TSTOP;//转盘位置触发
				PARAINIT(my);
				break;
			}
			if(FSM_STA == RUN)//设备运行中
			{
				if(GUS.ProductData.Data.Num_Sample > 0)	//样品数量 > 0
				{
					LT.PlateTrigTask.execute = TEXE;	//启动转盘位置触发
					AxRelSpd(AxPlate,PPR_PLATE,GUS.PlateData.Data.Spd_Slow);	//转盘走一圈 工位走一格
					my.step = 3;
				}
				else
				{
					if(AxSta(AxPlate) == AXSTA_READY)
					{
						LT.PlateTrigTask.execute = TIDLE;//如果样品等于零则停止启动转盘位置触发
						TASKOK;
					}
				}
			}
			break;
			
		case 3:
			if(AxSta(AxPlate) == AXSTA_READY && LT.PackTask.execute != TEXE)//等待转盘运行结束 并且 装填任务不被别人召唤
			{
				if(LT.PackTask.done == 1)	//装填成功
				{
					my.count++;	//计数
					if(my.count < GUS.ProductData.Data.Num_Sample)//循环装填直达数量
					{
						my.step = 2;
					}
					else
					{
						LT.PlateTrigTask.execute = TIDLE;
						TASKOK;
					}
				}
				else	//装填不成功
				{
					my.step = 2;
				}
			}
			break;
	}
}

/**
* @author 21/03/05  yao
* @Description: 样品后编
* @param -- 
* @param --  
* @return --
*/
static void PackSampleBehind()
{
	static u32 tm;
	TASKRUN(LT.PackSampleBehindTask)//样品后编
	{
		case 1:
			my.count = 0;
			my.step = 2;
			break;
		
		case 2:
			if(my.execute == TSTOP)
			{
				PARAINIT(my);
				break;
			}
			if(FSM_STA == RUN)//机器设备运行状态
			{
				if(GUS.ProductData.Data.Num_SampleBehind > 0)//样品后空格数量
				{
					BeltMoveStart(1,1,100);//全速前进一格
					my.step = 3;
				}
				else
				{
					TASKOK;
				}
			}
			break;
			
		case 3:
			if(LT.BeltMoveTask.execute == TIDLE && LT.UpCameraTask.execute == TIDLE)//走带任务为空闲 并且 上相机任务空闲
			{
				/************修改后编的视觉触发*******************/
				if(LT.UpCameraTask.done == 1)//查看视觉结果
				{
					LT.PressTask.execute = TEXE;//启动封带
					LT.UpCameraTask.execute = TEXE;//启动上相机
					my.step = 4;
				}
				else
				{
					LT.PressTask.execute = TEXE;//启动封带
					my.step = 0x31;
				}
			}
			break;
			
		case 0x31://执行补料前先等待封带动作完成，避免编带回退时膜褶皱
			if(LT.PressTask.execute == TIDLE)//封带结束
			{
				BeltMoveStart(1,-1,100);//全速后退1格
				my.step = 6;
			}
			break;
		
		case 4:
			if(LT.PressTask.execute == TIDLE)//封带结束
			{
				TimerRst(&tm);
				my.step = 5;
			}
			break;
			
		case 5:
			if(TimerCnt(&tm,1) > 100)//计时 100ms 
			{
				my.count++;
				if(my.count < GUS.ProductData.Data.Num_SampleBehind)//计数 < 样品后空格数量 循环移动直到数量达到
				{
					my.step = 2;
				}
				else
				{
					TASKOK;
				}
			}
			break;
			
		case 6:
			if(LT.BeltMoveTask.execute == TIDLE)//走带任务空闲
			{
				if(GUS.EnableData.Data.RePack == 1)//有无开启功能
				{
					LT.PlateTrigTask.execute = TEXE;	//转盘位置触发任务召唤
					LT.RepackTask.execute = TEXE;		//补料任务召唤
					my.step = 7;
				}
				else
				{
					AlarmSetBit(2,0,11);//装填视觉NG，未开启自动补料，请人工处理
					my.step = 8;
				}
			}
			break;
			
		case 7:
			if(LT.RepackTask.execute == TIDLE)		//补料任务空闲
			{
				LT.PlateTrigTask.execute = TIDLE;	//转盘任务空闲
				if(LT.RepackTask.done == 1)//查看补料任务结果为完美
				{
					my.step = 0x71;
				}
				else
				{
					AlarmSetBit(2,0,8);//补料失败，请检查并人工补料
					my.step = 8;
				}
				BeltMoveStart(1,1,100);//补料开始位置还原
			}
			break;
			
		case 0x71:
			if(LT.BeltMoveTask.execute == TIDLE)//装填电机空闲
			{
				TimerRst(&tm);
				LT.PressTask.execute = TEXE;	//启动封带
				LT.UpCameraTask.execute = TEXE;	//启动上相机
				my.step = 5;
			}
			break;
			
		case 8:
			if(my.execute == TSTOP)	//本任务被杀死
			{
				PARAINIT(my);
				break;
			}
			if(FSM_STA == RUN)		//设备状态为运行
			{
				my.step = 3;
			}
			break;
	}
}

/**
* @author 21/03/05  yao
* @Description: 计算已经装填的数量
* @param -- 
* @param --  
* @return --
*/
s32 GetPackedNum()
{
	s32 StepTemp = BeltTotalNum + GUS.PackData.Data.Idx_Cut;//装填总长 + 切带位置
	for(int i=0; i< 10; i++)//寻找最短的正在装填的料带
	{
		if(GUS.RecordData.Data.PackStep[i] > 0 && GUS.RecordData.Data.PackStep[i] < StepTemp)
		{
			StepTemp = GUS.RecordData.Data.PackStep[i];
		}
	}
	StepTemp = StepTemp - GUS.ProductData.Data.Num_BeforPack - GUS.ProductData.Data.Num_NoPress;
				//正在装填的料带 - 前空格 - 不封数量
	if(StepTemp < 0)	//考虑如果全部为0
	{
		StepTemp = 0;
	}
	if(StepTemp > GUS.ProductData.Data.Num_Pack)	//正在装填的料带 > 目标装填数量
	{
		StepTemp = GUS.ProductData.Data.Num_Pack;	//正在装填的料带 = 目标装填数量
	}
	GUS.RecordData.Data.Num_Packed = StepTemp;	//已装填数 = 正在装填的料带
	return GUS.RecordData.Data.Num_Packed;		//返回已装填数
}

/**
* @author 21/03/05  yao
* @Description: 装填
* @param -- 
* @param --  
* @return --
*/
void Pack()
{
	static u32 tm,tm_Circle;
	static TrigDef tr_Start,tr_End;
	PackBefore();	//前编任务
	PackBehind();	//后编任务
	PackSample();	//样品任务
	PackSampleBehind();	//样品后编任务
	if(PlateTrig(&tr_Start,GUS.PlateData.Data.Pos_PackStart))	//装填开始位置触发
	{
		LT.PackTask.execute = TEXE;	//装填启动
	}
	if(PlateTrig(&tr_End,GUS.PlateData.Data.Pos_PackEnd))		//装填结束位置触发
	{
		LT.PackTask.execute = TSTOP;
	}
	TASKRUN(LT.PackTask)
	{
		case 1:
			if(
				GUS.EnableData.Data.Pack == 1 &&
				GUR.PlateLoopData.Data.Have[GUS.StationData.Data.Idx_Pack] == 1 && 
				GUR.PlateLoopData.Data.OK[GUS.StationData.Data.Idx_Pack] == 1 &&
				LT.RepackTask.execute == TIDLE)
			{
				Q_Set(Q_PackVacuo,OFF);	//装填位真空关
				BeltMoveStart(1, 1, 100);
				my.step = 2;
			}
			else
			{
				TASKNG;
			}
			break;
			
		case 2:
			if(LT.BeltMoveTask.execute == TIDLE)
			{
				if(LT.UpCameraTask.done == 1 || LT.PackSampleTask.execute == TEXE)//视觉正常或正在编样品，编样品的时候他自己判断是否成功
				{
					LT.UpCameraTask.execute = TEXE;	//上相机任务启动
					LT.PressTask.execute = TEXE;	//封带任务启动
					GUW.DisplayData.Data.PackSpeed = 3600/TimerCnt(&tm_Circle,1);
					TimerRst(&tm_Circle);
					GUR.PlateLoopData.Data.Have[GUS.StationData.Data.Idx_Pack] = 0;
					TimerRst(&tm);
					my.step = 3;
				}
				else//视觉NG
				{
					/*********皮带倒退一格执行补料动作前要先封带，避免出现膜褶皱情况********/
					HZ_AxStop(AxPlate);//立即停转盘
					LT.PressTask.execute = TEXE;	//封带任务启动
					my.step = 0x21;
				}
			}
			break;
			
		case 0x21://视觉NG等待封带完成后进行补料，防止膜褶皱
			if(LT.PressTask.execute == TIDLE)
			{
				BeltMoveStart(1,-1,100);//料带倒退一格
				if(GUS.EnableData.Data.RePack == 1 && I_Get(I_DetectForPack) == ON)//如果开启补料 且 装填前感应没有料
				{
					LT.Blow2Task.Manual = 1;	//吹料2 模式为手动
					LT.Blow2Task.execute = TEXE;//强制吹掉一颗料，制造空吸嘴
				}
				if(LT.UpCameraTask.execute == TEXE)
				{
					AlarmSetBit(2,0,7);//装填视觉未完成，请调整视觉参数，或降低整机运行速度
				}
				my.step = 4;
			}
			break;
			
		case 3:
			if(LT.PressTask.execute == TIDLE)//封带任务空闲
			{
				TASKOK;
			}
			break;
			
		case 4:
			if(AxSta(AxPlate) == AXSTA_READY && LT.BeltMoveTask.execute == TIDLE)//转盘停止，料带倒退完成后
			{
				TimerRst(&tm);
				Q_Set(Q_PackVacuo,ON);//入料口真空转吹气
				my.step = 5;
			}
			break;
			
		case 5:
			if(TimerCnt(&tm,1) > GUS.TimeData.Data.Tim_Blow)//延时
			{
				Data.PlateRun.Mode = StopToReady;//转盘停到预备位，这时候会取走最后放的那颗料
				my.step = 6;
			}
			break;
			
		case 6:
			if(LT.PlateRunTask.execute == TIDLE)
			{
				Q_Set(Q_PackVacuo,OFF);//入料口切换回真空
				if(GUS.EnableData.Data.RePack == 0)//如果补料使能未打开
				{
					AlarmSetBit(2,0,11);//装填视觉NG，未开启自动补料，请人工处理
					TASKNG;
				}
				else
				{
					LT.RepackTask.execute = TEXE;//启动补料
					my.step = 7;
				}
			}
			break;
			
		case 7:
			if(LT.RepackTask.execute == TIDLE)//等待补料任务结束
			{
				if(LT.RepackTask.done == 1)//补料任务结束成功
				{
					if(FSM_STA == RUN && LT.PackSampleTask.execute == TIDLE && LT.PackBehindTask.execute == TIDLE && LT.PackSampleBehindTask.execute == TIDLE)
					{
						LT.PlateRunTask.execute = TEXE;//启动转盘
					}
					TASKOK;
				}
				else
				{
					AlarmSetBit(2,0,8);//补料失败，请检查并人工补料
					TASKNG;
				}
			}
			break;
	}
}

