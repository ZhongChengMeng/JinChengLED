/*
*@Header:
*@File Name:	ElecTest.c
*@Author: Yao
*@Data: 2021/03/05
*@Description: 方向旋转相关:
*					TurnMotor(u32 Dir)	方向选择
*					ElecTurn()	电性换向 兼外部轮询 Logic.c
*/

#include "Logic.h"
#include "Plate.h"

/**
* @author 21/03/05  yao
* @Description: 换向位置选择
* @param -- 方向
* @return --
*/
static void TurnMotor(u32 Dir)
{
	Data.ETurn.CurDir += Dir;
	switch(Data.ETurn.CurDir%4)
	{
		case ElecTurn0:
			Q_Set(Q_TurnCtr1,OFF);
			Q_Set(Q_TurnCtr2,OFF);
			break;
		
		case ElecTurn90:
			Q_Set(Q_TurnCtr1,ON);
			Q_Set(Q_TurnCtr2,OFF);
			break;
		
		case ElecTurn180:
			Q_Set(Q_TurnCtr1,OFF);
			Q_Set(Q_TurnCtr2,ON);
			break;
		
		case ElecTurnN90:
			Q_Set(Q_TurnCtr1,ON);
			Q_Set(Q_TurnCtr2,ON);
			break;
		
		case ElecNg:
			break;
	}
}

/**
* @author 21/03/05  yao
* @Description:  电性换向
* @param -- 
* @return --
*/

void ElecTurn()
{
	u32	TurnDir;
	static TrigDef tr_Start,tr_End,tr_Enable;
	
	//旋转使能
	if(TrigOne(&tr_Enable,SysTim_GetUser() > 200000,0))//重新上电自动回原点
	{
		GUW.ManualData.Data.ETurnHome = 1;
	}
	//旋转电机通过触摸屏使能，值变化触发PLC控制
	if(GUW.ManualData.Data.ETurnHome)
	{
		GUW.ManualData.Data.ETurnHome++;
	}
	if(PlateTrig(&tr_Start,GUS.PlateData.Data.Pos_ETurnStart))
	{
		LT.ElecTurnTask.execute = TEXE;
	}
	if(PlateTrig(&tr_End,GUS.PlateData.Data.Pos_ETurnEnd))
	{
		LT.ElecTurnTask.execute = TSTOP;
	}

	TASKRUN(LT.ElecTurnTask)
	{
		case 1:
			if(
				GUR.PlateLoopData.Data.Have[GUS.StationData.Data.Idx_ETurn] == 1 && 
				GUR.PlateLoopData.Data.OK[GUS.StationData.Data.Idx_ETurn] == 1)
			{
				switch(GUS.ETestData.Data.Dir_Num)
				{
					case 0://单晶
						if(GUS.ETestData.Data.Dir_Target == 0)
						{
							TurnDir = GUR.PlateLoopData.Data.Elec[GUS.StationData.Data.Idx_ETurn];
						}else if(GUR.PlateLoopData.Data.Elec[GUS.StationData.Data.Idx_ETurn] == ElecTurn0)
						{
							TurnDir = ElecTurn180;
						}
						else
						{
							TurnDir = ElecTurn0;
						}
						break;
					
					case 1://4脚3晶
						TurnDir = (GUR.PlateLoopData.Data.Elec[GUS.StationData.Data.Idx_ETurn] + GUS.ETestData.Data.Dir_Target)%4;
						//电性方向测试结果 + 电性目标方向 的和 取余 4
						break;
				}
				TurnMotor(TurnDir);//电性方向旋转
				my.step = 2;
			}
			else
			{
				TASKOK;
			}
			break;
			
		case 2:
			if(1)//等待换向结束
			{
				TASKOK;
			}
			break;
	}
}

