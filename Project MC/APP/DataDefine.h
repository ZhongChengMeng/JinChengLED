#ifndef _DATADEFINE_H_
#define _DATADEFINE_H_

#include "lock.h"
#include "Trigger.h"

#define DATACHAECK 			0x01237654               				//校验码

#define SYSREADLEN 			1000                     				//系统读区寄存器长度
#define USERREADLEN 		1000                    				//用户读区寄存器长度
#define READLEN 			(SYSREADLEN + USERREADLEN)  			//系统读区寄存器总长度

#define SYSWRITELEN 		1000                      				//系统写区长度
#define USERWRITELEN 		1000                     				//用户写区长度
#define WRITELEN 			(SYSWRITELEN + USERWRITELEN) 			//总写区长度

#define SYSSAVELEN 			4000                         			//系统寄存器长度
#define USERSAVELEN 		4000                       				//用户存储寄存器长度
#define SAVELEN 			(SYSSAVELEN + USERSAVELEN + 4) 			//用户存储区总长

#define GLOBALDATALEN 		(READLEN + WRITELEN + SAVELEN) 			//modbus总地址长度

#define MAXAXISNUM 			50           							//最大轴数
#define INPUTSTARTADDR 		310      								//modbus输入检测地址
#define INPUTENDADDR 		389        								//
#define OUTPUTSTARTADDR 	1020    								//modbus输出更新地址
#define OUTPUTENDADDR 		1099      								//


/************************************************************************/
/* 定义用户自定义的数据类型									            */
/************************************************************************/
//测试结果
typedef enum
{
	ElecTurn0,
	ElecTurn90,
	ElecTurn180,
	ElecTurnN90,
	ElecNg,
	ElecBusy,
}ElecSta;

//注册机数据
typedef struct
{
	u32	Lockflag;		//	有没有
	u32 DateNum;		//	日期
}LoginDataDef;


typedef struct
{
	TrigDef Tr;
	s32	IsTrig;
}PosTrigDef;

//
//struct
//{
//	s32 Station_Feed;				//4320	转盘入料
//	s32 Station_ElecTest;			//4322	电性测试
//	s32	Station_BlowElec;			//4324	电性弃料
//	s32 Station_ElecTurn;			//4326	旋转方向
//	s32 Station_DownCamera;			//4338	底部相机
//	s32 Station_Pack;				//4340	装填入料
//	s32	Station_BlowRepack;			//4342	补料前吹
//	s32 Station_ChkIsHave;			//4344	检测转盘吸嘴是否有料
//	s32	Station_ChkRepackTake;		//4346	检测补料取不良
//	s32	Station_BlowEndNg;			//4348	末端吹NG
//	s32	Station_BlowEndOk;			//4350	末端吹OK
//}PlateIdxDef;

/******************  Bit definition for PointPara  *******************/





/************************************************************************/
/* sysmodbus data define                                		        */
/************************************************************************/
typedef struct
{
    u8 hour;		//范围0-23
    u8 min;			//0-59
    u8 sec;			//0-59
    u8 ampm;		//这个值没用，禁止使用
} Time;

//轴速度配置
typedef struct AxSpd_t
{
    float  startspeed;  //unit/s
    float  acctime;		//ms
    float  runspeed;	//unit/s
    float  dectime;		//ms
    float  endspeed;	//unit/s
} AxspdDef;

//轴回零配置
typedef struct AxHome_t
{
    u32	orgnum;			 	//原点
    u16 orglev;			 	//原点信号电平
    u16 homemode;		 	//回零模式
    float homespeedfast;    //unit/s 回零快速
    float homespeedslow;  	//unit/s 回零慢速
    float homespeedoffset; 	//unit/s 零点偏移
} AxHomeDef;

//轴限位配置
typedef struct AxLimit_t
{
    u16 limitMode;		//限位模式：0 没限位 1 软件限位 2 硬件限位 3 软硬都限
    u16	alarmmode;		//轴报警口配置  0 1 2报警电平信号
    u16 poslimitsig;    //正限位信号
    u16 poslimitlev;    //正限位信号电平
    u16 neglimitsig;    //负限位信号
    u16 neglimitlev;    //负限位信号电平
    float softminlimit;   //软负限位 unit
    float softmaxlimit;   //软正限位 unit
} AxislimitDef;

//轴单位转换
typedef struct AxisConversion_t
{
    u32 PPR;	//每转脉冲数
    float MPR;	//每转毫米__导程
} AxisConversion;

//轴参数类
typedef struct AxisPara_t
{
    AxspdDef AxSpd;             //10个寄存器
    AxHomeDef Axhomecfg;        //10个寄存器
    AxislimitDef Axlimitcfg;    //10个寄存器
    AxisConversion Axconver;    //4个寄存器
} AxisData;

//modbus 地址交互表
typedef union {
    u8 Data_8[GLOBALDATALEN];
    u16 Data_16[GLOBALDATALEN / 2];
    struct
    {
        struct //READ ADDR 0-999
        {
            union //SYS READ ADDR 0-499 系统只读寄存器
            {
                u16 Data_16[SYSREADLEN / 2];
                struct
                {
                    u32 HardWare_Ver[10];		  			//0-19	硬件版本
                    u32 SoftWare_Ver[10];		  			//20-39	软件版本
                    u32 res[5];                  			//40-49
                    u16 AXSTA[MAXAXISNUM];		  			//50-99	轴状态
                    float AxisUnitPosition[MAXAXISNUM];  	//100-199 用户单位的当前位置
                    s32 AxisPosition[MAXAXISNUM]; 			//200-299	轴当前位置
                    s32 AxisEncoder[5];			  			//300-309	轴当前状态
                    u32 InputStatus[40];		  			//310-389	输入口状态
                    u32 ErrorCode[20];			  			//390-	错误码
                    u32 ErrorLevel;				  			//430-	错误等级
                    Time time;								//432	rtc 时间
                    Date date;								//434	rtc 日期
                    u32 CID[2];								//436 438 本机生成的客户随机码
					/* 显示到期时间 */
					u16 year;		//440显示到期时间
					u16 month;      //441
					u16 day;        //442
					u16 week;		//443剩余时间
                } Data;
            } Sys;

            union //USER READ ADDR 500-999 用户只读寄存器
            {
                u16 Data_16[USERREADLEN / 2];
                struct
                {
					union
					{
						u32	Data_32[10];//500 518
						struct
						{
							u32 RunStatus; 				//500 运行状态显示
							u32 HaveToReset;    		//502 复位标志
							u32 HaveToRepower;  		//504 重启标志
						}Data;
					}FsmData;
					
					union
					{
						u32	Data_32[50];//520 618
						struct
						{
							u32	PlateDownTime;			//520	凸轮下停留时间  测试1开始角度到测试1结束角度
							u32	AdjustTime[2];			//522 24	定位时间	矫正器运行一周所用时间
							u32	ElecTest1Time;			//526	测试1时间		测试1所用时间
							u32 ElecTest2Time;			//528	测试2时间		测试2所用时间
							u32	ElecTurn90Time;			//530	旋转90时间		旋转工位90°时间
							u32	ElecTurn180Time;		//532	旋转180时间		旋转工位180°时间
							u32	ElecTurn270Time;		//534	旋转270时间		旋转工位-90°时间
							u32	DownCameraTime;			//536	底部视觉检测时间 检测周期
							u32	PackMotorTime;			//538	装填电机时间	 装填电机一格
							u32	UpCameraTime;			//540	装填相机检测时间 检测周期
							u32	PackPressTime;			//542	装填封带时间	 封带周期
						}Data;
					}TimeRecord;
					
					union
					{
						u32	Data_32[100];//620 818
						struct
						{
							s32	Have[20];				//620	吸嘴有料 0:没有料	1：有料
							s32	Elec[20];				//660	电性结果 -1：NG 0：未测试 1：0度 2：90度 3：180度 4：-90度	
							s32	OK[20];					//700	是否好料 0：NG 1：OK
						}Data;
					}PlateLoopData;
					
					union
					{
						u32	Data_32[40];//820 900
						struct
						{
							char FileName[8][20];	//文件名字 
						}Data;
					}FileName;
					
					
                } Data;
            } User;
        } Read;

        struct //WRITE ADDR 1000-1999 读写寄存器区，掉电数据丢失为0
        {
            union //SYS WRITE ADDR 1000-1499 系统读写寄存器，掉电不保存
            {
                u16 Data_16[SYSWRITELEN / 2];
                struct
                {
                    u32 res[10];		  	//1000-1019 保留
                    u32 OutputStatus[40]; 	//1020-1099	输出口状态
                    u32 FlashOperate;	 	//1100-	Flash操作
                    u32 FilesOperate;	 	//1102-	文件操作
                    u32 FramOperate;	  	//1104-	铁电操作
                    u32 JogMode;		  	//1106-	点动模式 0级别 1行程 2速度模式(走到限位)
                    u32 JogPositionLevel; 	//1108-	点动行程级别0 1 2 3 4
                    u32 JogPosition;	  	//1110-	点动设定行程 
                    u32 JogForward[2];		//1112-1115	正向点动
                    u32 JogBackward[2];   	//1116-1119	反向点动
                    u32 JogGohome[2];	 	//1120-1123	点动回零
                    u32 JogStop[2];		  	//1124-1127	点动立即停止
                    u32 jogSpd;			  	//1128  轴点动百分比速度hz
                    u32 ClearAlarm;		  	//1130-	清除错误
                    struct {
                        u32 refreshId;	//	1132 随机数刷新
                        s32 decode[4];	// 1134 1136 1138 1140  4个解锁码 *6个数 一共24个数
                    } LockPara;
                } Data;
            } Sys;

            union //USER WRITE ADDR 1500-1999 用户读写寄存器
            {
                u16 Data_16[USERWRITELEN / 2];
                struct
                {
                    union
                    {
						u32	Data_32[10];//1500 1518
						struct
						{
							u32 RunCommand;     		//1500
							u32 DeviceMode;		  		//1502
							u32 TechWord;   			//1504
							u32 Login;					//1506  注册按钮
						}Data;
                    } FsmData;
					
					union
					{
						u32	Data_32[20];//1520 1558
						struct
						{
							s32	Motion;					//1520	通用动作触发
							s32	PackMove;				//1522	连续走带开关
							s32	Feed;					//1524	连续送料开关
							s32	PassPackNg;				//1526	强制通过装填影像NG
							u32	JogSpeed;				//1528	点动速度 0：快速	1：慢速
							u32	ETurnHome;				//1530	旋转使能 1：回零
							u32	KeepETest;				//1532	连续测试
							u32	KeepBeltMove;			//1534	连续走带
							u32	KeepFeed;				//1536	连续送料
						}Data;
					}ManualData;
					
 					union
					{
						u32	Data_32[20];//1560 1598
						struct
						{
							s32	PackCameraOk;			//1560	装填视觉结果
							s32	DownCameraOk;			//1562	底部视觉结果
							s32	ETestResult;			//1564	电性测试结果
							s32	Num_ETestNg;			//1566	连续测试NG报警
							s32	Num_ETestSame;			//1568	连续测试结果相同报警
							s32	Num_RepackNg;			//1570	连续补料失败报警
							s32	Num_DownCameraNg;		//1572	连续下相机NG报警
							s32	Num_NoBeltSupply;		//1574	连续不送带数量
							s32 Num_BeltCollect;		//1576  收带马达停止编带报警数量
						}Data;
					}ProcessData;
					
					union
					{
						u32	Data_32[20];//1600 1638
						struct
						{
							s32 RunSpeed;				//1600	运行速度
							float PackSpeed;			//1602	装填速度
							float ETestNgRatio;			//1604	电性不良比例
							float DownCameraNgRatio;	//1606	底部不良比例
							s32	PlatePosCmd;			//1608	转盘位置指令单圈值
							s32	PlatePosEnc;			//1610	转盘位置反馈单圈值
							s32 File_Chiose;			//1612  文件名更新
							s32 File_Write;				//1614  文件写入本机
							s32 File_Read;				//1616  本机写入文件
						}Data;
					}DisplayData;
					
					union
					{
						u32	Data_32[20];//1640 1678
						struct
						{
							u16 settime;	//1640
							u16 y;			//1641
							u16 m;			//1642
							u16 d;			//1643
							u16 hour;		//1644
							u16 min;		//1645
							u16 sec;		//1646
							u16 week;		//1647
							u32 DateRequest;//1648	1649 时间请求
						}Data;
					}DisplayTime;
               } Data;
            } User;
        } Write;

        struct //SAVE ADDR 存储区地址寄存器
        {
            union //SYS SAVE ADDR 2000-3999 //系统存储寄存器地址
            {
                u16 Data_16[SYSSAVELEN / 2];
                struct
                {
                    AxisData axis[MAXAXISNUM];       //轴的运行参数 34*50  2000-3699
                    struct
                    {  
                        u32 checkflag;  //初始化检测，当新板块没写过数据或是数据不是规定值时，需要初始化modbus的存储寄存器初值，防止出现新卡上电全ff或数据清空后，导致的撞机等意外情况。初始化过数据后写0xea
                        u32 res[9];     //预留，用做锁机时间等
                    } SaveCheck; 		//3700-3719

                } Data;
            } Sys;

            union //USER SAVE ADDR 4000- 5999//用户存储寄存器地址
            {
                u16 Data_16[USERSAVELEN / 2];
                struct
                {
					//电机参数，厂家权限保护
					union
					{
						u32 Data_32[100];
						struct
						{
							s32	HomeSpeed[10];		//4000	回零速度
							s32	RunSpeed[10];		//4020	运行速度
							s32	AccDecTime[10];		//4040	加减速时间
							s32	OriginDir[10];		//4060	原点电平
							s32	AlarmDir[10];		//4080	报警电平
							s32	MotorDir[10];		//4100	电机方向
							s32	JogSpeed[10];		//4120	手动速度
							s32	EnableDir[10];		//4140	使能电平
						}Data;
					}MotorData;
					//配置参数，厂家权限保护
					union
					{
						u32 Data_32[50];
						struct
						{
							s32	FuncSel_AdjustMode;			//4200	定位模式	0：脉冲步进	1：IO步进
							s32	FuncSel_PackHomeMode;		//4202	装填电机	0：脉冲步进	1：IO步进	2：脉冲伺服
							s32	FuncSel_HopperMode;			//4204	料斗模式	0：电磁铁 1：振动盘-低电平 2：振动盘-高电平 3：电机
							s32	FuncSel_PackCameraMode;		//4206	装填相机	0：基恩士	1：麦视
							s32	FuncSel_DownCameraMode;		//4208	底部相机	0：基恩士	1：麦视
						}Data;
					}ConfigData;
					//设置参数，工程师权限保护
                    union
					{
						u32 Data_32[50];
						struct
						{
							s32	AgingMode;					//4300	老化模式	0：正常生产 1：老化空跑
							s32	FinishStop;					//4302	单卷停机	0：单卷完成不停机， 1：单卷完成停机
							s32	LightIn;					//4304	里光源
							s32	LightOut;					//4306	外光源
							s32	Hopper;						//4308	料斗
							s32	Beep;						//4310	蜂鸣器
							s32	RePack;						//4312	补料
							s32	ElecTest;					//4314	电性测试	0：关	1：开
							s32	ElecTest2;					//4316	二站测试	0：关	1：开
							s32	DownCamera;					//4318	底部视觉	0：关	1：开
							s32	Pack;						//4320	装填		0：关	1：开
							s32	PackCamera;					//4322	装填视觉	0：关	1：开
						}Data;
					}EnableData;
					union
					{
						u32	Data_32[50];
						struct
						{
							s32	Num_NoPress;				//4400	不封数量
							s32	Num_BeforPack;				//4402	前空格数量
							s32	Num_Pack;					//4404	目标装填数量
							s32	Num_BehindPack;				//4406	后空格数量
							s32	Num_Sample;					//4408	样品数量
							s32	Num_SampleBehind;			//4410	样品后空格数量
							s32	Num_TargetReel;				//4412	目标完卷数量
							s32 Num_ReelPack;				//4414  一卷需要多少编带长度(PCS)
						}Data;
					}ProductData;
					union
					{
						u32	Data_32[50];
						struct
						{
							s32	Tim_PressDown;				//4500	封带下压时间
							s32	Tim_PressUp;				//4502	封带抬起时间
							s32	Tim_Hopper;					//4504	料斗震动周期
							s32	Tim_Supply;					//4506	料带电机一次转动时间
							s32	Tim_Clean;					//4508	清洗吹气时间
							s32	Tim_Blow;					//4510	吹料弃料时间
							s32	Tim_PackCameraDelay;		//4512	装填影像延时
							s32	Tim_DownCameraDelay;		//4514	底部影像延时
							s32 Tim_HopperKeep;				//4516	圆振缺料滤波	新
							s32 Tim_HopperTime;				//4518	料斗振动次数	新
						}Data;
					}TimeData;
					union
					{
						u32	Data_32[50];
						struct
						{
							s32	Num_ETestNg;				//4600	连续测试NG报警
							s32	Num_ETestSame;				//4602	连续测试结果相同报警
							s32	Num_RepackNg;				//4604	连续补料失败报警
							s32	Num_DownCameraNg;			//4606	连续下相机NG报警
							s32	Num_DiscardFull;			//4608	弃料筒满料报警
							s32	Num_NewBelt;				//4610	一卷新载带数量
							s32	Num_NewFilm;				//4612	一卷新胶膜数量
							s32	Num_NoBeltSupply;			//4614	连续不送带数量
							s32 Num_RepackNum;				//4616	补料计数	新
						}Data;
					}AlarmData;
					union
					{
						u32	Data_32[50];
						struct
						{
							s32	Pos_Ready;					//4700	预备位
							s32	Num_SwitchToFast;			//4702	进入快速数量，入料口连续完成进料，切换到快速
							s32	Num_SwitchToSlow;			//4704	进入慢速数量，入料口连续未完成进料，切换到慢速
							
							s32	Pos_ETestStart;				//4706	测试开始角度
							s32	Pos_ETestEnd;				//4708	测试结束角度
							s32	Pos_ETest2Start;			//4710	二站测试开始角度
							s32	Pos_ETest2End;				//4712	二站测试结束角度
							s32	Pos_DownCameraStart;		//4714	下视觉开始角度
							s32	Pos_DownCameraEnd;			//4716	下视觉结束角度
							s32	Pos_Blow1Start;				//4718	吹料1开始角度
							s32	Pos_Blow1End;				//4720	吹料1结束角度
							s32	Pos_Blow2Start;				//4722	吹料2开始角度
							s32	Pos_Blow2End;				//4724	吹料2结束角度
							s32	Pos_Blow3Start;				//4726	吹料3开始角度
							s32	Pos_Blow3End;				//4728	吹料3结束角度
							s32	Pos_Blow4Start;				//4730	吹料4开始角度
							s32	Pos_Blow4End;				//4732	吹料4结束角度
							s32	Pos_Detect1Start;			//4734	吸嘴感应1开始角度
							s32	Pos_Detect1End;				//4736	吸嘴感应1结束角度
							s32	Pos_Detect2Start;			//4738	吸嘴感应2开始角度
							s32	Pos_Detect2End;				//4740	吸嘴感应2开始角度
							s32	Pos_PackStart;				//4742	装填开始角度
							s32	Pos_PackEnd;				//4744	装填结束角度
							s32	Pos_AdjustStart;			//4746	定位开始角度
							s32	Pos_AdjustEnd;				//4748	定位结束角度
							s32	Pos_FeedStart;				//4750	进料开始角度，不用了
							s32	Pos_FeedEnd;				//4752	进料结束角度，不用了
							s32	Pos_ETurnStart;				//4754	旋转换向开始角度
							s32	Pos_ETurnEnd;				//4756	旋转换向结束角度
							s32	Spd_Fast;					//4758	凸轮快速速度
							s32	Spd_Slow;					//4760	凸轮慢速速度
							s32 Pos_FeedVacuo1ON;			//4762	进料真空1开
							s32 Pos_FeedVacuo1OFF;			//4764	进料真空1关
							s32 Pos_FeedVacuo2ON;			//4766	进料真空2开
							s32 Pos_FeedVacuo2OFF;			//4768	进料真空2关
							s32 Pos_FeedVacuo3ON;			//4770	进料真空3开
							s32 Pos_FeedVacuo3OFF;			//4772	进料真空3关
							s32 Pos_FeedBlowON;				//4774	进料分离吹气开
							s32 Pos_FeedBlowOFF;			//4776	进料分离吹气关
							s32	Pos_InpStart;				//4778	到位检测开始
							s32	Pos_InpEnd;					//4780	到位检测结束
						}Data;
					}PlateData;

					union
					{
						u32	Data_32[50];
						struct
						{
							s32	Pos_Ready[2];				//4800	定位夹子预备位
						}Data;
					}AdjustData;
					union
					{
						u32	Data_32[50];
						struct
						{
							s32	Dir_Num;					//4900	电性方向数量
							s32	Dir_Target;					//4902	电性目标方向
							s32	Dir_Target2;				//4904	二站测试方向
						}Data;
					}ETestData;
					union
					{
						u32	Data_32[50];
						struct
						{
							s32	Pos_Ready;					//5000	装填预备位
							s32	Pos_Feed;					//5002	装填轴一格脉冲
							s32	Idx_Camera;					//5004	视觉位置
							s32	Idx_Press;					//5006	封刀位置
							s32	Idx_Cut;					//5008	切带位置
							s32	Num_Press;					//5010	封刀宽度
							s32	Num_RepackBlow;				//5012	补料吹空数量
						}Data;
					}PackData;
					union
					{
						u32 Data_32[50];
						struct
						{
							s32	Num_FinishReel;				//5100	已编卷数
							s32	Num_Packed;					//5102	已装填数
							s32	Num_NgDiscard1;				//5104	已装1号不良料筒1
							s32	Num_OkDiscard1;				//5106	已装1号良品料筒2
							s32	Num_NgDiscard2;				//5108	已装2号不良料筒3
							s32	Num_OkDiscard2;				//5110	已装2号良品料筒4
							s32	Num_PackBlet;				//5112	已用一卷载带数量
							s32	Num_PackFilm;				//5114	已用一卷胶膜数量
							s32	Num_HistoryReel;			//5116	历史已编卷数
							s32	Num_HistoryPack;			//5118	历史已装填数
							s32	PackStep[10];				//5120	编带步数记录，留10个位置，用来处理前后两卷料的衔接
						}Data;
					}RecordData;
					
					union
					{
						u32 Data_32[50];
						struct
						{
							s32	Idx_ETest;					//5200	测试工位号
							s32	Idx_Etest2;					//5202	二站测试工位号
							s32	Idx_ETurn;					//5204	换向工位号
							s32	Idx_DownCamera;				//5206	下视觉工位号
							s32	Idx_Blow1;					//5208	吹料1工位号
							s32	Idx_Blow2;					//5210	吹料2工位号
							s32	Idx_Blow3;					//5212	吹料3工位号
							s32	Idx_Blow4;					//5214	吹料4工位号
							s32	Idx_Detect1;				//5216	吸嘴材料感应1工位号
							s32	Idx_Detect2;				//5218	吸嘴材料感应2工位号
							s32	Idx_Pack;					//5220	装填工位号
						}Data;
					}StationData;
					
					union
					{
						u32 Data_32[50];//5300
						struct
						{
							s32 TransWatch1;				//5300	窗口转换	新
							s32	IO_Trans_Hopper;			//5302	料斗信号逻辑反转	新
							char FileName_save[20];			//5304  文件名字10 5312
						}Data;
					}TransWatch;
				} Data;
            } User;
        } Save;
        u32 Check; //校验码
    } SpaceType;
} GlobalDataDef;
extern GlobalDataDef GlobalData;

//快捷宏
#define GSR GlobalData.SpaceType.Read.Sys.Data
#define GSW GlobalData.SpaceType.Write.Sys.Data
#define GSS GlobalData.SpaceType.Save.Sys.Data
#define GUR GlobalData.SpaceType.Read.User.Data
#define GUW GlobalData.SpaceType.Write.User.Data
#define GUS GlobalData.SpaceType.Save.User.Data

//快捷宏定义
#define	FSM_STA GUR.FsmData.Data.RunStatus
#define FSM_CMD GUW.FsmData.Data.RunCommand

#define	STATIONNUM	12		//转盘工位数
#define	PPR_PLATE	1000	//转盘一圈脉冲数
#define	PPR_ADJUST	1000	//定位（矫正）一圈脉冲数
#define	PPR_PACK	1000	//装填一格脉冲数


#endif
