/*
*@Header:
*@File Name:	Detect.c
*@Author: Yao
*@Data: 2021/03/05
*@Description: ת�̹�λ�ϸ�Ӧ���:
*					Detect1()	װ��ǰ��Ӧ
*					Detect2()	װ����Ӧ ��Ӧ2
*					Detect()	�ⲿ��ѯ Logic.c
*/

#include "Logic.h"
#include "Plate.h"

/**
* @author 21/03/05  yao
* @Description: װ��ǰ��Ӧ ��Ӧ1
* @param -- 
* @return --
*/
//װ��ǰ��Ӧ ��Ӧ1
static void Detect1()
{
	static u32 tm;
	static TrigDef tr_Start,tr_End;
	if(PlateTrig(&tr_Start,GUS.PlateData.Data.Pos_Detect1Start))
	{
		LT.Detect1Task.execute = TEXE;
	}
	if(PlateTrig(&tr_End,GUS.PlateData.Data.Pos_Detect1End))
	{
		LT.Detect1Task.execute = TSTOP;
	}
	TASKRUN(LT.Detect1Task)
	{
		case 1:
			TimerRst(&tm);
			if(GUS.EnableData.Data.AgingMode == 0)//��������ģʽ�����
			{
				GUR.PlateLoopData.Data.Have[GUS.StationData.Data.Idx_Detect1] = 0;//��ʼ��Ϊ0
			}
			my.step = 2;
			break;
			
		case 2:
			if(my.execute == TSTOP)
			{
				TASKOK;
				break;
			}
			if(I_Get(I_DetectForPack) == ON)//��Ӧ����
			{
				GUR.PlateLoopData.Data.Have[GUS.StationData.Data.Idx_Detect1] = 1;//˵������
				TASKOK;
			}
			break;
	}
}

/**
* @author 21/03/05  yao
* @Description: װ����Ӧ ��Ӧ2
* @param -- 
* @return --
*/
//װ����Ӧ ��Ӧ2
static void Detect2()
{
	static u32 tm;
	static TrigDef tr_Start,tr_End;
	if(PlateTrig(&tr_Start,GUS.PlateData.Data.Pos_Detect2Start))
	{
		LT.Detect2Task.execute = TEXE;
	}
	if(PlateTrig(&tr_End,GUS.PlateData.Data.Pos_Detect2End))
	{
		LT.Detect2Task.execute = TSTOP;
	}
	TASKRUN(LT.Detect2Task)
	{
		case 1:
			TimerRst(&tm);
			GUR.PlateLoopData.Data.Have[GUS.StationData.Data.Idx_Detect2] = 0;
			my.step = 2;
			break;
		
		case 2:
			if(my.execute == TSTOP)
			{
				TASKOK;
				break;
			}
			if(I_Get(I_DetectTakeOut) == ON)//��Ӧ����
			{
				GUR.PlateLoopData.Data.Have[GUS.StationData.Data.Idx_Detect2] = 1;
				TASKOK;
			}
			break;
	}
}

void Detect()
{
	Detect1();	//װ��ǰ��Ӧ
	Detect2();	//װ����Ӧ
}

