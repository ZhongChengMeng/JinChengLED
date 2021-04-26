/*
 * @Author: your name
 * @Date: 2020-06-19 14:36:56
 * @LastEditTime: 2020-06-30 22:46:11
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: \ZH_ZDJ\Code\APP\Teach.c
 */
#include "Teach.h"
#include "DataDefine.h"
#include "FSM.h"
#include "userTool.h"
#include "iodefine.h"
#include "Plate.h"
#include "Belt.h"


void Teach()
{
    if (GUR.FsmData.Data.RunStatus != STOP)
    {
		GUW.ManualData.Data.Motion = 0;
		GUW.FsmData.Data.TechWord = 0;
        return;
    }
    switch (GUW.FsmData.Data.TechWord) //1520
    {
		case 1:
			
			break;
		
		default:
			break;
    }
    GUW.FsmData.Data.TechWord = 0;
	
	switch(GUW.ManualData.Data.Motion)
	{
		case 1:
			LT.AdjustHomeTask[0].execute = TEXE;
			break;
		
		case 2:
			LT.AdjustHomeTask[1].execute = TEXE;
			break;
		
		case 3:
			LT.BeltHomeTask.execute = TEXE;
			break;
		
		case 4:
			LT.PlateHomeTask.execute = TEXE;
			break;
		
		case 7:
			LT.AdjustHalfTask[0].execute = TEXE;
			break;
		
		case 8:
			LT.AdjustFullTask[0].execute = TEXE;
			break;
		
		case 9:
			LT.AdjustHalfTask[1].execute = TEXE;
			break;
		
		case 10:
			LT.AdjustFullTask[1].execute = TEXE;
			break;
		
		case 11://凸轮正向到预备位
			if(abs(GSR.AxisEncoder[0]%PPR_PLATE) > 50)
			{
				Plate_Goto(1,0);
			}
			break;
			
		case 12://凸轮正转一圈
			AxRelSpd(AxPlate,PPR_PLATE,GUS.PlateData.Data.Spd_Slow);
			break;
			
		case 13://测试一次
			LT.ElecTestTask.Manual = 1;
			LT.ElecTestTask.execute = TEXE;
			break;
		
		
		case 15://连续测试关
			GUW.ManualData.Data.KeepETest = 0;
			break;
		
		case 16://料带前进一格
			BeltMoveStart(1,1,100);
			break;
		
		case 17://料带后退一格
			BeltMoveStart(1,-1,100);
			break;
		
		case 18://定位半步
			LT.AdjustHalfTask[0].execute = TEXE;
			LT.AdjustHalfTask[1].execute = TEXE;
			break;
		
		case 19://定位整步
			LT.AdjustFullTask[0].execute = TEXE;
			LT.AdjustFullTask[1].execute = TEXE;
			break;
		
		case 20://开始新卷
			PARAINIT(GUS.RecordData.Data.PackStep);
			LT.PackBeforTask.execute = TEXE;
			LT.MainProcessTask.execute = TSTOP;
			break;
		
		case 21://清理吸嘴
			LT.CleanTask.execute = TEXE;
			break;
		
		case 22://清除材料
			LT.ClearTask.execute = TEXE;
			break;
		
		case 23://凸轮反向到定位角度
			if(abs(GSR.AxisEncoder[0]%PPR_PLATE - GUS.PlateData.Data.Pos_AdjustStart) > 20)
			{
				Plate_Goto(-1,GUS.PlateData.Data.Pos_AdjustStart);
			}
			break;
		
		case 24://凸轮反向到进料角度
			if(abs(GSR.AxisEncoder[0]%PPR_PLATE - GUS.PlateData.Data.Pos_FeedStart) > 20)
			{
				Plate_Goto(-1,GUS.PlateData.Data.Pos_FeedStart);
			}
			break;
		
		case 25://凸轮反向到装填角度
			if(abs(GSR.AxisEncoder[0]%PPR_PLATE - GUS.PlateData.Data.Pos_PackStart) > 20)
			{
				Plate_Goto(-1,GUS.PlateData.Data.Pos_PackStart);
			}
			break;
		
		case 26://凸轮反向到旋转角度
			if(abs(GSR.AxisEncoder[0]%PPR_PLATE - GUS.PlateData.Data.Pos_ETurnStart) > 20)
			{
				Plate_Goto(-1,GUS.PlateData.Data.Pos_ETurnStart);
			}
			break;
		
		case 27://凸轮反向到测试角度
			if(abs(GSR.AxisEncoder[0]%PPR_PLATE - GUS.PlateData.Data.Pos_ETestStart) > 20)
			{
				Plate_Goto(-1,GUS.PlateData.Data.Pos_ETestStart);
			}
			break;
		
		case 28://凸轮反向到二站测试角度
			if(abs(GSR.AxisEncoder[0]%PPR_PLATE - GUS.PlateData.Data.Pos_ETest2Start) > 20)
			{
				Plate_Goto(-1,GUS.PlateData.Data.Pos_ETest2Start);
			}
			break;
		
		case 29://凸轮反向到吹料1角度
			if(abs(GSR.AxisEncoder[0]%PPR_PLATE - GUS.PlateData.Data.Pos_Blow1Start) > 20)
			{
				Plate_Goto(-1,GUS.PlateData.Data.Pos_Blow1Start);
			}
			break;
		
		case 30://凸轮反向到吹料2角度
			if(abs(GSR.AxisEncoder[0]%PPR_PLATE - GUS.PlateData.Data.Pos_Blow2Start) > 20)
			{
				Plate_Goto(-1,GUS.PlateData.Data.Pos_Blow2Start);
			}
			break;
		
		case 31://凸轮反向到吹料3角度
			if(abs(GSR.AxisEncoder[0]%PPR_PLATE - GUS.PlateData.Data.Pos_Blow3Start) > 20)
			{
				Plate_Goto(-1,GUS.PlateData.Data.Pos_Blow3Start);
			}
			break;
		
		case 32://凸轮反向到吹料4角度
			if(abs(GSR.AxisEncoder[0]%PPR_PLATE - GUS.PlateData.Data.Pos_Blow4Start) > 20)
			{
				Plate_Goto(-1,GUS.PlateData.Data.Pos_Blow4Start);
			}
			break;
		
		case 33://凸轮反向到吸嘴感应1角度
			if(abs(GSR.AxisEncoder[0]%PPR_PLATE - GUS.PlateData.Data.Pos_Detect1Start) > 20)
			{
				Plate_Goto(-1,GUS.PlateData.Data.Pos_Detect1Start);
			}
			break;
		
		case 34://凸轮反向到吸嘴感应2角度
			if(abs(GSR.AxisEncoder[0]%PPR_PLATE - GUS.PlateData.Data.Pos_Detect2Start) > 20)
			{
				Plate_Goto(-1,GUS.PlateData.Data.Pos_Detect2Start);
			}
			break;
		
		case 35://凸轮反向到底部视觉角度
			if(abs(GSR.AxisEncoder[0]%PPR_PLATE - GUS.PlateData.Data.Pos_DownCameraStart) > 20)
			{
				Plate_Goto(-1,GUS.PlateData.Data.Pos_DownCameraStart);
			}
			break;
			
		case 36://凸轮反向去到真空1开始角度
			if(abs(GSR.AxisEncoder[0]%PPR_PLATE - GUS.PlateData.Data.Pos_FeedVacuo1ON) > 20)
			{
				Plate_Goto(-1,GUS.PlateData.Data.Pos_FeedVacuo1ON);
			}
			break;
		case 37://凸轮反向去到真空1停止角度
			if(abs(GSR.AxisEncoder[0]%PPR_PLATE - GUS.PlateData.Data.Pos_FeedVacuo1OFF) > 20)
			{
				Plate_Goto(-1,GUS.PlateData.Data.Pos_FeedVacuo1OFF);
			}
			break;
		case 38://凸轮反向去到真空2开始角度
			if(abs(GSR.AxisEncoder[0]%PPR_PLATE - GUS.PlateData.Data.Pos_FeedVacuo2ON) > 20)
			{
				Plate_Goto(-1,GUS.PlateData.Data.Pos_FeedVacuo2ON);
			}
			break;
		case 39://凸轮反向去到真空2停止角度
			if(abs(GSR.AxisEncoder[0]%PPR_PLATE - GUS.PlateData.Data.Pos_FeedVacuo2OFF) > 20)
			{
				Plate_Goto(-1,GUS.PlateData.Data.Pos_FeedVacuo2OFF);
			}
			break;
		case 40://凸轮反向去到真空3开始角度
			if(abs(GSR.AxisEncoder[0]%PPR_PLATE - GUS.PlateData.Data.Pos_FeedVacuo3ON) > 20)
			{
				Plate_Goto(-1,GUS.PlateData.Data.Pos_FeedVacuo3ON);
			}
			break;
		case 41://凸轮反向去到真空3停止角度
			if(abs(GSR.AxisEncoder[0]%PPR_PLATE - GUS.PlateData.Data.Pos_FeedVacuo3OFF) > 20)
			{
				Plate_Goto(-1,GUS.PlateData.Data.Pos_FeedVacuo3OFF);
			}
			break;
		case 42://凸轮反向去到分离吹气开始角度
			if(abs(GSR.AxisEncoder[0]%PPR_PLATE - GUS.PlateData.Data.Pos_FeedBlowON) > 20)
			{
				Plate_Goto(-1,GUS.PlateData.Data.Pos_FeedBlowON);
			}
			break;
		case 43://凸轮反向去到分离吹气停止角度
			if(abs(GSR.AxisEncoder[0]%PPR_PLATE - GUS.PlateData.Data.Pos_FeedBlowOFF) > 20)
			{
				Plate_Goto(-1,GUS.PlateData.Data.Pos_FeedBlowOFF);
			}
			break;
			
		case 44://凸轮反转一圈
			AxRelSpd(AxPlate,-PPR_PLATE,GUS.PlateData.Data.Spd_Slow);
			break;
	}
	GUW.ManualData.Data.Motion = 0;
}
