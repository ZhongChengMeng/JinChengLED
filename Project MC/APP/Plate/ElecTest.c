/*
*@Header:
*@File Name:	ElecTest.c
*@Author: Yao
*@Data: 2021/03/05
*@Description: 电性测试相关:
*					ElecTesk2()	二站测试
*					ElecTesk()	一站测试 兼外部轮询 Logic.c
*/


#include "Logic.h"
#include "Plate.h"

#define	MackETestResult(A,B,C) (((u32)A<<0)|((u32)B<<1)|((u32)C<<2))//位操作

/**
* @author 21/03/05  yao
* @Description: 二站测试
* @param -- 
* @return --
*/
void ElecTesk2()
{
	static u32 tm;
	static TrigDef tr_Start,tr_End;
	if(PlateTrig(&tr_Start,GUS.PlateData.Data.Pos_ETest2Start))
	{
		LT.ElecTest2Task.execute = TEXE;
	}
	if(PlateTrig(&tr_End,GUS.PlateData.Data.Pos_ETest2End))
	{
		LT.ElecTest2Task.execute = TSTOP;
	}
	Q_Set(Q_ElecTest2Sot,ON);//二站测试SOT
//	Q_Set(Q_ElecTest2Dir,(GpioSta)GUS.ETestData.Data.Dir_Target2);
	TASKRUN(LT.ElecTest2Task)
	{
		case 1:
			if(GUS.EnableData.Data.AgingMode == 1 || GUS.EnableData.Data.ElecTest2 == 0)//老化模式或没开测试功能，给一个随机结果
			{
				TASKOK;
			}
			else if(GUR.PlateLoopData.Data.Have[GUS.StationData.Data.Idx_Etest2] == 1 || my.Manual)//正常模式，吸嘴有料或手动测试
			{
				TimerRst(&tm);
				my.step = 2;
			}
			else//吸嘴没有料，直接结束
			{
				TASKOK;
			}
			break;
			
		case 2:
			if(I_Get(I_ElecTest2Ret) == ON)//二站测试结果
			{
				TASKOK;
			}
			else if(my.execute == TSTOP)
			{
				GUR.PlateLoopData.Data.OK[GUS.StationData.Data.Idx_Etest2] = -1;//到时还没信号直接定义为NG
				TASKOK;
			}
			break;
	}
}

/**
* @author 21/03/05  yao
* @Description: 一站测试
* @param -- 
* @return --
*/
void ElecTesk()
{
	static u32 tm,tm_Cilcle;	//定时器
	static TrigDef tr_Eot,tr_Start,tr_End;	//触发器
	static u32 LastResult;	//上次结果，用来判断连续结果相同报警
	u32 ThisResult = MackETestResult(I_Get(BCD_I_A),I_Get(BCD_I_B),I_Get(BCD_I_C));	//实时获取结果值
	
	ElecTesk2();	//二站测试任务调用
	
	if(GUW.ManualData.Data.KeepETest)	//连续测试
	{
		LT.ElecTestTask.Manual = 1;
		LT.ElecTestTask.execute = TEXE;	//电性测试启动
	}
	
	if(PlateTrig(&tr_Start,GUS.PlateData.Data.Pos_ETestStart))//位置启动
	{
		LT.ElecTestTask.execute = TEXE;
	}
	if(PlateTrig(&tr_End,GUS.PlateData.Data.Pos_ETestEnd))//位置停止
	{
		LT.ElecTestTask.execute = TSTOP;
	}
	TASKRUN(LT.ElecTestTask)
	{
		case 1:
			if(GUS.EnableData.Data.AgingMode == 1 || GUS.EnableData.Data.ElecTest == 0)//老化模式或没开测试功能，给一个随机结果
			{
				if(GUS.ETestData.Data.Dir_Num == 0)//2方向
				{
					GUR.PlateLoopData.Data.Elec[GUS.StationData.Data.Idx_ETest] = rand()%2? ElecTurn0:ElecTurn180;
				}
				else//4方向
				{
					GUR.PlateLoopData.Data.Elec[GUS.StationData.Data.Idx_ETest] = rand()%4;
				}
				TASKOK;
			}
			else if(GUR.PlateLoopData.Data.Have[GUS.StationData.Data.Idx_ETest] == 1 || my.Manual)//正常模式，吸嘴有料或手动测试
			{
				Q_Set(BCD_Q_ETestSOT,ON);	//打开测试
				TimerRst(&tm_Cilcle);
				TimerRst(&tm);
				my.step = 2;
			}
			else//吸嘴没有料，直接结束
			{
				TASKOK;
			}
			break;
		
		case 2:
			if(TimerCnt(&tm,1) < 5)break;
			Q_Set(BCD_Q_ETestSOT,OFF);
			TrigRst(&tr_Eot);
			TimerRst(&tm);
			my.step = 3;
			break;
			
		case 3:
			if(TimerCnt(&tm,1) > 200)
			{
				AlarmSetBit(2,0,17);//电性测试无响应
				TASKNG;
				break;
			}
			if(TrigOne(&tr_Eot,ThisResult != MackETestResult(OFF,OFF,OFF),0))//111
			{
				//GUR.TimeRecord.Data.ElecTest1Time = TimerCnt(&tm_Cilcle,1);原先使用的
				switch(GUS.ETestData.Data.Dir_Num)//有几个电性方向
				{
					case 0://2向
						switch(ThisResult)
						{
							case MackETestResult(ON,OFF,OFF):
								GUW.ProcessData.Data.ETestResult = ElecTurn0;
								break;
							
							case MackETestResult(OFF,ON,OFF):
								GUW.ProcessData.Data.ETestResult = ElecTurn180;
								break;

							case MackETestResult(OFF,OFF,ON):
								GUW.ProcessData.Data.ETestResult = ElecNg;
								break;
							
							default:
								GUW.ProcessData.Data.ETestResult = ElecNg;
								break;
						}
						break;
							
					case 1://4向
						switch(ThisResult)
						{
							case MackETestResult(OFF,OFF,ON):
								GUW.ProcessData.Data.ETestResult = ElecTurn0;
								break;
							
							case MackETestResult(ON,OFF,OFF):
								GUW.ProcessData.Data.ETestResult = ElecTurn90;
								break;

							case MackETestResult(ON,OFF,ON):
								GUW.ProcessData.Data.ETestResult = ElecTurn180;
								break;

							case MackETestResult(OFF,ON,OFF):
								GUW.ProcessData.Data.ETestResult = ElecTurnN90;
								break;

							case MackETestResult(ON,ON,ON):
								GUW.ProcessData.Data.ETestResult = ElecNg;
								break;
							
							default:
								GUW.ProcessData.Data.ETestResult = ElecNg;
								break;
						}
						break;
				}
				GUR.PlateLoopData.Data.Elec[GUS.StationData.Data.Idx_ETest] = GUW.ProcessData.Data.ETestResult;//电性结果  = 电性测试结果
				if(GUW.ProcessData.Data.ETestResult == ElecNg)//结果为NG 
				{
					GUR.PlateLoopData.Data.OK[GUS.StationData.Data.Idx_ETest] = 0;
					GUW.ProcessData.Data.Num_ETestNg++;
					if(GUW.ProcessData.Data.Num_ETestNg > GUS.AlarmData.Data.Num_ETestNg && GUS.AlarmData.Data.Num_ETestNg != 0)
					{
						GUW.ProcessData.Data.Num_ETestNg = 0;
						AlarmSetBit(2,0,18);//电性测试连续不良
					}
				}
				else
				{
					GUW.ProcessData.Data.Num_ETestNg = 0;
					if(LastResult == GUW.ProcessData.Data.ETestResult)//上次的结果 == 新的结果
					{
						GUW.ProcessData.Data.Num_ETestSame++;
						if(GUW.ProcessData.Data.Num_ETestSame > GUS.AlarmData.Data.Num_ETestSame && GUS.AlarmData.Data.Num_ETestSame != 0)
						{
							GUW.ProcessData.Data.Num_ETestSame = 0;
							AlarmSetBit(2,0,19);//电性测试结果连续相同
						}
					}
					else
					{
						GUW.ProcessData.Data.Num_ETestSame = 0;
					}
					LastResult = GUW.ProcessData.Data.ETestResult;//更新
				}
				TASKOK;
			}
			if(my.execute == TSTOP)
			{
//				AlarmSetBit(2,0,1);//电性测试未完成，请检查控制器与测试卡的连接，调整测试时间或降低运行速度
				GUR.PlateLoopData.Data.OK[GUS.StationData.Data.Idx_ETest] = 0;
				TASKOK;
			}
			break;
	}
}


