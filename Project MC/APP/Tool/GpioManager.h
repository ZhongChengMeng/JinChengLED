#ifndef	_GPIOMANAGER_H_
#define	_GPIOMANAGER_H_

	//输入输出编号
	//CardId:板卡号，0主卡，>0扩展卡
	//idx	:板卡输入输出丝印号，从1开始
	#define	GPIO(idx) (idx-1)
	#define	EGPIO(CardId,idx) (CardId*1000+idx-1)

	typedef enum
	{
		ON=0,
		OFF=1,
		ONR=1,
		OFFR=0,
	}GpioSta;
	
	typedef enum
	{
		I_Origin1		=	GPIO(1),	//定位1原点
		I_Origin2		=	GPIO(2),	//定位2原点
		I_Origin3		=	GPIO(3),	//装填原点
		I_Origin4		=	GPIO(4),	//凸轮原点
		I_Origin5		=	GPIO(5),	//
		I_Origin6		=	GPIO(6),	//
		I_StartBtn		=	GPIO(7),	//启动
		I_StopBtn		=	GPIO(8),	//暂停
		I_ScramBtn		=	GPIO(9),	//急停
		I_ResetBtn		=	GPIO(10),	//复位
		I_FlatLack		=	GPIO(11),	//直振缺料
		I_RoundLack		=	GPIO(12),	//圆振缺料
		I_FlatInp		=	GPIO(13),	//直振到位
		I_PackCameraEot	=	GPIO(14),	//装填影像EOT
		I_PackCameraOk	=	GPIO(15),	//装填影像OK/NG
		I_DownCameraEot	=	GPIO(16),	//底部影像EOT
		I_DownCameraOk	=	GPIO(17),	//底部影像OK/NG
		I_CollectBtn	=	GPIO(18),	//收带按钮
		I_TempErr		=	GPIO(19),	//温控器异常
		I_PressureErr	=	GPIO(20),	//正压检测
		I_VacuoErr		=	GPIO(21),	//负压检测
		I_ElecTest2Ret	=	GPIO(22),	//二站测试结果
		I_DetectForPack	=	GPIO(23),	//装填前侦测
		I_ChkPack		=   GPIO(24),	//装填感应
		I_DetectTakeOut	=   GPIO(25),	//补料取料检测
		I_ChkBelt		=   GPIO(26),	//料带感应
		I_ChkFilm		=	GPIO(27),	//胶膜感应

		Q_TurnCtr1		=	GPIO(1),	//旋转运行控制1
		Q_TurnCtr2		=	GPIO(2),	//旋转运行控制2
		Q_BeltSupply	=	GPIO(3),	//放带马达
		Q_BeltCollect	=	GPIO(4),	//收带马达
		Q_FeedVacuo1	=	GPIO(5),	//进料真空1
		Q_FeedVacuo2	=	GPIO(6),	//进料真空2
		Q_FeedVacuo3	=	GPIO(7),	//进料真空3
		Q_FeedBlow		=	GPIO(8),	//分离吹气
		Q_PackVacuo		=	GPIO(9),	//装填位真空
		Q_PlateClean	=	GPIO(10),	//吸嘴清理
		Q_DiscardBlow1	=	GPIO(11),	//吹气1
		Q_DiscardBlow2	=	GPIO(12),	//吹气2
		Q_DiscardBlow3	=	GPIO(13),	//吹气3
		Q_DiscardBlow4	=	GPIO(14),	//吹气4
		Q_PressUpCyd	=   GPIO(15),   //封带上气缸
		Q_PressDownCyd	=	GPIO(16),   //封带下气缸
		Q_CutCyd		=   GPIO(17),   //切带气缸
		Q_FeederHopper	=	GPIO(18),   //料斗控制
		Q_PackCameraSot	=	GPIO(19),	//装填影像SOT
		Q_DownCameraSot	=	GPIO(20),	//底部影像SOT
		Q_ElecTest2Sot	=	GPIO(21),	//二站测试SOT
		Q_ElecTest2Dir	=	GPIO(22),	//二站测试方向
		Q_LightIn		=	GPIO(23),	//里光源
		Q_LightOut		=	GPIO(24),	//外光源
		Q_VacuoPump		=	GPIO(25),	//真空泵		
		Q_FeederFlat	=   GPIO(26),   //直振
		Q_Green			=	GPIO(27),   //绿灯
		Q_Yellow		=   GPIO(28),   //黄灯
		Q_Red			=	GPIO(29),   //红灯
		Q_Beep			=	GPIO(30),	//蜂鸣器
		Q_FeederRound	=	GPIO(31),   //圆振
		
	//BCD Output
	BCD_Q_ETestSOT		=	GPIO(38),	//BCD_OUT 0  //电性测试触发SOT
	//BCD Input
	BCD_I_ETestRet_0	=   GPIO(31),   //BCD_IN_0  2向0度
	BCD_I_ETestRet_180	=	GPIO(32),   //BCD_IN_1  2向180度
	BCD_I_ETestRet_NG	=   GPIO(33),   //BCD_IN_2  2向测试不良
	
	BCD_I_A				=  	GPIO(31),   //BCD_IN_0  4向bit0
	BCD_I_B				=	GPIO(32),   //BCD_IN_1  4向bit1
	BCD_I_C				=  	GPIO(33),   //BCD_IN_2  4向bit2
		
		
		
		
	}Gpio;

	extern GpioSta I_Get(Gpio idx);
	extern void Q_Set(Gpio idx, GpioSta val);
	extern GpioSta Q_Get(Gpio idx);
	

#endif
