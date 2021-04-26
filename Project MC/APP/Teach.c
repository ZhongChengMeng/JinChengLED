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
		
		case 11://͹������Ԥ��λ
			if(abs(GSR.AxisEncoder[0]%PPR_PLATE) > 50)
			{
				Plate_Goto(1,0);
			}
			break;
			
		case 12://͹����תһȦ
			AxRelSpd(AxPlate,PPR_PLATE,GUS.PlateData.Data.Spd_Slow);
			break;
			
		case 13://����һ��
			LT.ElecTestTask.Manual = 1;
			LT.ElecTestTask.execute = TEXE;
			break;
		
		
		case 15://�������Թ�
			GUW.ManualData.Data.KeepETest = 0;
			break;
		
		case 16://�ϴ�ǰ��һ��
			BeltMoveStart(1,1,100);
			break;
		
		case 17://�ϴ�����һ��
			BeltMoveStart(1,-1,100);
			break;
		
		case 18://��λ�벽
			LT.AdjustHalfTask[0].execute = TEXE;
			LT.AdjustHalfTask[1].execute = TEXE;
			break;
		
		case 19://��λ����
			LT.AdjustFullTask[0].execute = TEXE;
			LT.AdjustFullTask[1].execute = TEXE;
			break;
		
		case 20://��ʼ�¾�
			PARAINIT(GUS.RecordData.Data.PackStep);
			LT.PackBeforTask.execute = TEXE;
			LT.MainProcessTask.execute = TSTOP;
			break;
		
		case 21://��������
			LT.CleanTask.execute = TEXE;
			break;
		
		case 22://�������
			LT.ClearTask.execute = TEXE;
			break;
		
		case 23://͹�ַ��򵽶�λ�Ƕ�
			if(abs(GSR.AxisEncoder[0]%PPR_PLATE - GUS.PlateData.Data.Pos_AdjustStart) > 20)
			{
				Plate_Goto(-1,GUS.PlateData.Data.Pos_AdjustStart);
			}
			break;
		
		case 24://͹�ַ��򵽽��ϽǶ�
			if(abs(GSR.AxisEncoder[0]%PPR_PLATE - GUS.PlateData.Data.Pos_FeedStart) > 20)
			{
				Plate_Goto(-1,GUS.PlateData.Data.Pos_FeedStart);
			}
			break;
		
		case 25://͹�ַ���װ��Ƕ�
			if(abs(GSR.AxisEncoder[0]%PPR_PLATE - GUS.PlateData.Data.Pos_PackStart) > 20)
			{
				Plate_Goto(-1,GUS.PlateData.Data.Pos_PackStart);
			}
			break;
		
		case 26://͹�ַ�����ת�Ƕ�
			if(abs(GSR.AxisEncoder[0]%PPR_PLATE - GUS.PlateData.Data.Pos_ETurnStart) > 20)
			{
				Plate_Goto(-1,GUS.PlateData.Data.Pos_ETurnStart);
			}
			break;
		
		case 27://͹�ַ��򵽲��ԽǶ�
			if(abs(GSR.AxisEncoder[0]%PPR_PLATE - GUS.PlateData.Data.Pos_ETestStart) > 20)
			{
				Plate_Goto(-1,GUS.PlateData.Data.Pos_ETestStart);
			}
			break;
		
		case 28://͹�ַ��򵽶�վ���ԽǶ�
			if(abs(GSR.AxisEncoder[0]%PPR_PLATE - GUS.PlateData.Data.Pos_ETest2Start) > 20)
			{
				Plate_Goto(-1,GUS.PlateData.Data.Pos_ETest2Start);
			}
			break;
		
		case 29://͹�ַ��򵽴���1�Ƕ�
			if(abs(GSR.AxisEncoder[0]%PPR_PLATE - GUS.PlateData.Data.Pos_Blow1Start) > 20)
			{
				Plate_Goto(-1,GUS.PlateData.Data.Pos_Blow1Start);
			}
			break;
		
		case 30://͹�ַ��򵽴���2�Ƕ�
			if(abs(GSR.AxisEncoder[0]%PPR_PLATE - GUS.PlateData.Data.Pos_Blow2Start) > 20)
			{
				Plate_Goto(-1,GUS.PlateData.Data.Pos_Blow2Start);
			}
			break;
		
		case 31://͹�ַ��򵽴���3�Ƕ�
			if(abs(GSR.AxisEncoder[0]%PPR_PLATE - GUS.PlateData.Data.Pos_Blow3Start) > 20)
			{
				Plate_Goto(-1,GUS.PlateData.Data.Pos_Blow3Start);
			}
			break;
		
		case 32://͹�ַ��򵽴���4�Ƕ�
			if(abs(GSR.AxisEncoder[0]%PPR_PLATE - GUS.PlateData.Data.Pos_Blow4Start) > 20)
			{
				Plate_Goto(-1,GUS.PlateData.Data.Pos_Blow4Start);
			}
			break;
		
		case 33://͹�ַ��������Ӧ1�Ƕ�
			if(abs(GSR.AxisEncoder[0]%PPR_PLATE - GUS.PlateData.Data.Pos_Detect1Start) > 20)
			{
				Plate_Goto(-1,GUS.PlateData.Data.Pos_Detect1Start);
			}
			break;
		
		case 34://͹�ַ��������Ӧ2�Ƕ�
			if(abs(GSR.AxisEncoder[0]%PPR_PLATE - GUS.PlateData.Data.Pos_Detect2Start) > 20)
			{
				Plate_Goto(-1,GUS.PlateData.Data.Pos_Detect2Start);
			}
			break;
		
		case 35://͹�ַ��򵽵ײ��Ӿ��Ƕ�
			if(abs(GSR.AxisEncoder[0]%PPR_PLATE - GUS.PlateData.Data.Pos_DownCameraStart) > 20)
			{
				Plate_Goto(-1,GUS.PlateData.Data.Pos_DownCameraStart);
			}
			break;
			
		case 36://͹�ַ���ȥ�����1��ʼ�Ƕ�
			if(abs(GSR.AxisEncoder[0]%PPR_PLATE - GUS.PlateData.Data.Pos_FeedVacuo1ON) > 20)
			{
				Plate_Goto(-1,GUS.PlateData.Data.Pos_FeedVacuo1ON);
			}
			break;
		case 37://͹�ַ���ȥ�����1ֹͣ�Ƕ�
			if(abs(GSR.AxisEncoder[0]%PPR_PLATE - GUS.PlateData.Data.Pos_FeedVacuo1OFF) > 20)
			{
				Plate_Goto(-1,GUS.PlateData.Data.Pos_FeedVacuo1OFF);
			}
			break;
		case 38://͹�ַ���ȥ�����2��ʼ�Ƕ�
			if(abs(GSR.AxisEncoder[0]%PPR_PLATE - GUS.PlateData.Data.Pos_FeedVacuo2ON) > 20)
			{
				Plate_Goto(-1,GUS.PlateData.Data.Pos_FeedVacuo2ON);
			}
			break;
		case 39://͹�ַ���ȥ�����2ֹͣ�Ƕ�
			if(abs(GSR.AxisEncoder[0]%PPR_PLATE - GUS.PlateData.Data.Pos_FeedVacuo2OFF) > 20)
			{
				Plate_Goto(-1,GUS.PlateData.Data.Pos_FeedVacuo2OFF);
			}
			break;
		case 40://͹�ַ���ȥ�����3��ʼ�Ƕ�
			if(abs(GSR.AxisEncoder[0]%PPR_PLATE - GUS.PlateData.Data.Pos_FeedVacuo3ON) > 20)
			{
				Plate_Goto(-1,GUS.PlateData.Data.Pos_FeedVacuo3ON);
			}
			break;
		case 41://͹�ַ���ȥ�����3ֹͣ�Ƕ�
			if(abs(GSR.AxisEncoder[0]%PPR_PLATE - GUS.PlateData.Data.Pos_FeedVacuo3OFF) > 20)
			{
				Plate_Goto(-1,GUS.PlateData.Data.Pos_FeedVacuo3OFF);
			}
			break;
		case 42://͹�ַ���ȥ�����봵����ʼ�Ƕ�
			if(abs(GSR.AxisEncoder[0]%PPR_PLATE - GUS.PlateData.Data.Pos_FeedBlowON) > 20)
			{
				Plate_Goto(-1,GUS.PlateData.Data.Pos_FeedBlowON);
			}
			break;
		case 43://͹�ַ���ȥ�����봵��ֹͣ�Ƕ�
			if(abs(GSR.AxisEncoder[0]%PPR_PLATE - GUS.PlateData.Data.Pos_FeedBlowOFF) > 20)
			{
				Plate_Goto(-1,GUS.PlateData.Data.Pos_FeedBlowOFF);
			}
			break;
			
		case 44://͹�ַ�תһȦ
			AxRelSpd(AxPlate,-PPR_PLATE,GUS.PlateData.Data.Spd_Slow);
			break;
	}
	GUW.ManualData.Data.Motion = 0;
}
