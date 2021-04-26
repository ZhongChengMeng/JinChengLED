/*
*@Header:
*@File Name:	Repack.c
*@Author: Yao
*@Data: 2021/03/05
*@Description: 补料相关:
*					Repack()	补料 外部轮询 Logic.c
*/

#include "Logic.h"
#include "Plate.h"
#include "Pack.h"
#include "Belt.h"

/**
* @author 21/03/05  yao
* @Description: 补料
* @param --
* @param --  
* @return --
*/
//补料
void Repack()
{
	static u32 tm;
	TASKRUN(LT.RepackTask)
	{
		case 1:
			/*************添加直振开启*****************/
			Q_Set(Q_FeederFlat,ON);
			if(AxSta(AxPlate) == AXSTA_READY)	//转盘停止
			{
				if(FSM_STA != RUN)//判断是否在运行状态下
				{
					TASKOK;
					break;
				}
				if(GUR.PlateLoopData.Data.Have[GUS.StationData.Data.Idx_Pack-1] == 0)//如果下一个吸嘴是空 装填吸嘴
				{
					BeltMoveStart(2,-1,5);//载带慢速后退2
					my.step = 2;
				}
				else
				{
					GUR.PlateLoopData.Data.OK[GUS.StationData.Data.Idx_Blow2-1] = 0;//让吹气2吹出一个空吸嘴 2号吹气的上个工位
					AxRelSpd(AxPlate,PPR_PLATE,GUS.PlateData.Data.Spd_Slow);		//慢速旋转一个工位
				}
				/*************添加封带上气缸关闭*****************/
				Q_Set(Q_PressUpCyd,OFF);
			}
			break;
			
		case 2:
			if(LT.BeltMoveTask.execute == TIDLE)//走带结束
			{
				Plate_Goto(1,GUS.PlateData.Data.Pos_PackStart);//转盘到装填开始位
				my.step = 3;
			}
			break;
			
		case 3:
			if(AxSta(AxPlate) == AXSTA_READY)//转盘电机准备
			{
				Q_Set(Q_PackVacuo,ON);//装填真空转吹气
				TimerRst(&tm);
				my.step = 4;
			}
			break;
			
		case 4:
			if(TimerCnt(&tm,1) > GUS.TimeData.Data.Tim_Blow)//					吹料弃料时间
			{
				GUR.PlateLoopData.Data.OK[GUS.StationData.Data.Idx_Pack] = 0;	//装填工位
				AxRelSpd(AxPlate,PPR_PLATE,GUS.PlateData.Data.Spd_Slow);		//慢速旋转一周
				my.step = 5;
			}
			break;
			
		case 5:
			if(AxSta(AxPlate) == AXSTA_READY)//转盘电机准备
			{
				Q_Set(Q_PackVacuo,OFF);//装填口切回真空
				if(GUR.PlateLoopData.Data.Have[GUS.StationData.Data.Idx_Detect2] == 0 && I_Get(I_DetectTakeOut) == ON)//如果吸嘴have==1，且检测到材料，已经取出来了
				{
					my.step = 6;
					GUS.AlarmData.Data.Num_RepackNum ++;//统计补料数量
				}
				else
				{
					BeltMoveStart(2,1,5);
					my.step = 0x51;
				}
			}
			break;
			
		case 0x51:
			if(LT.BeltMoveTask.execute == TIDLE)//走带结束
			{
				Plate_Goto(1,0);//转盘到预备位
				my.step = 0x52;
			}
			break;
			
		case 0x52:
			if(AxSta(AxPlate) == AXSTA_READY)//转盘电机准备
			{
				AlarmSetBit(2,0,26);//补料取NG材料失败
				TASKNG;
			}
			break;
			
		case 6:
			if(AxSta(AxPlate) == AXSTA_READY)//转盘电机准备
			{
				if((GUR.PlateLoopData.Data.Have[GUS.StationData.Data.Idx_Pack] == 1 &&
					GUR.PlateLoopData.Data.OK[GUS.StationData.Data.Idx_Pack] == 1) ||
					FSM_STA != RUN)
				{
					BeltMoveStart(2,1,5);//把这个好料装进去，停止的时候是为了恢复补料初始位置
					my.step = 7;
				}
				else
				{
					AxRelSpd(AxPlate,PPR_PLATE,GUS.PlateData.Data.Spd_Slow);//转盘电机旋转一圈
				}
			}
			break;
			
		case 7:
			if(LT.BeltMoveTask.execute == TIDLE)//走带空闲
			{
				Plate_Goto(1,0);//转盘到预备位
				LT.UpCameraTask.execute = TEXE;//执行上视觉
				my.step = 8;
			}
			break;
			
		case 8:
			if(LT.UpCameraTask.execute == TIDLE && AxSta(AxPlate) == AXSTA_READY)//等待上视觉工位结束 并且 转盘停止
			{
				if(LT.UpCameraTask.done == 1|| FSM_STA != RUN)//如果视觉ok
				{
					/*************添加封带上气缸打开*****************/
					Q_Set(Q_PressUpCyd,ON);//封带上气缸
					TASKOK;
				}
				else
				{
					my.step = 1;
				}
			}
			break;
	}
}
