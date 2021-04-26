/*
*@Header:
*@File Name:	UpCamera.c
*@Author: Yao
*@Data: 2021/03/05
*@Description: 装填上相机相关:
*					UpCamera()	装填上相机 外部轮询 Logic.c
*/

#include "Logic.h"

/**
* @author 21/03/05  yao
* @Description: 装填上相机
* @param --
* @param --  
* @return --
*/
void UpCamera()
{
	static u32 tm,tm_Circle;
	static TrigDef tr;
	int i;
	TASKRUN(LT.UpCameraTask)
	{
		case 1:
			for(i=0; i<10; i++)
			{
				if(GUS.RecordData.Data.PackStep[i])
				{
					if 	//判断当前步是否需要图像识别
					(
						(	//第一段，物料段
							GUS.RecordData.Data.PackStep[i]>= GUS.PackData.Data.Idx_Camera + GUS.ProductData.Data.Num_NoPress + GUS.ProductData.Data.Num_BeforPack&& 
							GUS.RecordData.Data.PackStep[i] < GUS.PackData.Data.Idx_Camera + GUS.ProductData.Data.Num_NoPress + GUS.ProductData.Data.Num_BeforPack + GUS.ProductData.Data.Num_Pack
						) 
						||
						(	//第二段，样本段
							GUS.RecordData.Data.PackStep[i]>= GUS.PackData.Data.Idx_Camera + GUS.ProductData.Data.Num_NoPress + GUS.ProductData.Data.Num_BeforPack + GUS.ProductData.Data.Num_Pack + GUS.ProductData.Data.Num_BehindPack &&
							GUS.RecordData.Data.PackStep[i] < GUS.PackData.Data.Idx_Camera + GUS.ProductData.Data.Num_NoPress + GUS.ProductData.Data.Num_BeforPack + GUS.ProductData.Data.Num_Pack + GUS.ProductData.Data.Num_BehindPack + GUS.ProductData.Data.Num_Sample
						)					
					)
					{
						break;
					}
				}			
			}
			
			if(i>=10 || GUS.EnableData.Data.PackCamera == 0 || GUS.EnableData.Data.AgingMode == 1)//相机功能 == 0(关闭)  老化模式 = 1
			{
				TASKOK;
				break;
			}
			TimerRst(&tm_Circle);
			TimerRst(&tm);
			my.step = 2;
			break;
			
		case 2:
			if(TimerCnt(&tm,1) >= GUS.TimeData.Data.Tim_PackCameraDelay)//4512	装填影像延时
			{
				TimerRst(&tm);
				Q_Set(Q_PackCameraSot,ON);//装填影像SOT
				my.step = 3;
			}
			break;
			
		case 3:
			if(TimerCnt(&tm,1) >= 5)
			{
				TimerRst(&tm);
				Q_Set(Q_PackCameraSot,OFF);//装填影像SOT
				my.step = 4;
			}
			break;
		
		case 4:
			if(my.execute == TSTOP || TimerCnt(&tm,1) > 200)
			{
				TASKNG;
				break;
			}
			if(TrigOne(&tr,I_Get(I_PackCameraEot) == ON,1))	//装填影像EOT
			{
//				GUR.TimeRecord.Data.UpCameraTime = TimerCnt(&tm_Circle,1);原先记录装填相机的时间
				if(I_Get(I_PackCameraOk) == ON)				//装填影像OK/NG
				{
					TASKOK;
				}
				else
				{
					TASKNG;
				}
			}
			break;
	}
}



