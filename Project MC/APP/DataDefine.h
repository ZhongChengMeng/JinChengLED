#ifndef _DATADEFINE_H_
#define _DATADEFINE_H_

#include "lock.h"
#include "Trigger.h"

#define DATACHAECK 			0x01237654               				//У����

#define SYSREADLEN 			1000                     				//ϵͳ�����Ĵ�������
#define USERREADLEN 		1000                    				//�û������Ĵ�������
#define READLEN 			(SYSREADLEN + USERREADLEN)  			//ϵͳ�����Ĵ����ܳ���

#define SYSWRITELEN 		1000                      				//ϵͳд������
#define USERWRITELEN 		1000                     				//�û�д������
#define WRITELEN 			(SYSWRITELEN + USERWRITELEN) 			//��д������

#define SYSSAVELEN 			4000                         			//ϵͳ�Ĵ�������
#define USERSAVELEN 		4000                       				//�û��洢�Ĵ�������
#define SAVELEN 			(SYSSAVELEN + USERSAVELEN + 4) 			//�û��洢���ܳ�

#define GLOBALDATALEN 		(READLEN + WRITELEN + SAVELEN) 			//modbus�ܵ�ַ����

#define MAXAXISNUM 			50           							//�������
#define INPUTSTARTADDR 		310      								//modbus�������ַ
#define INPUTENDADDR 		389        								//
#define OUTPUTSTARTADDR 	1020    								//modbus������µ�ַ
#define OUTPUTENDADDR 		1099      								//


/************************************************************************/
/* �����û��Զ������������									            */
/************************************************************************/
//���Խ��
typedef enum
{
	ElecTurn0,
	ElecTurn90,
	ElecTurn180,
	ElecTurnN90,
	ElecNg,
	ElecBusy,
}ElecSta;

//ע�������
typedef struct
{
	u32	Lockflag;		//	��û��
	u32 DateNum;		//	����
}LoginDataDef;


typedef struct
{
	TrigDef Tr;
	s32	IsTrig;
}PosTrigDef;

//
//struct
//{
//	s32 Station_Feed;				//4320	ת������
//	s32 Station_ElecTest;			//4322	���Բ���
//	s32	Station_BlowElec;			//4324	��������
//	s32 Station_ElecTurn;			//4326	��ת����
//	s32 Station_DownCamera;			//4338	�ײ����
//	s32 Station_Pack;				//4340	װ������
//	s32	Station_BlowRepack;			//4342	����ǰ��
//	s32 Station_ChkIsHave;			//4344	���ת�������Ƿ�����
//	s32	Station_ChkRepackTake;		//4346	��ⲹ��ȡ����
//	s32	Station_BlowEndNg;			//4348	ĩ�˴�NG
//	s32	Station_BlowEndOk;			//4350	ĩ�˴�OK
//}PlateIdxDef;

/******************  Bit definition for PointPara  *******************/





/************************************************************************/
/* sysmodbus data define                                		        */
/************************************************************************/
typedef struct
{
    u8 hour;		//��Χ0-23
    u8 min;			//0-59
    u8 sec;			//0-59
    u8 ampm;		//���ֵû�ã���ֹʹ��
} Time;

//���ٶ�����
typedef struct AxSpd_t
{
    float  startspeed;  //unit/s
    float  acctime;		//ms
    float  runspeed;	//unit/s
    float  dectime;		//ms
    float  endspeed;	//unit/s
} AxspdDef;

//���������
typedef struct AxHome_t
{
    u32	orgnum;			 	//ԭ��
    u16 orglev;			 	//ԭ���źŵ�ƽ
    u16 homemode;		 	//����ģʽ
    float homespeedfast;    //unit/s �������
    float homespeedslow;  	//unit/s ��������
    float homespeedoffset; 	//unit/s ���ƫ��
} AxHomeDef;

//����λ����
typedef struct AxLimit_t
{
    u16 limitMode;		//��λģʽ��0 û��λ 1 �����λ 2 Ӳ����λ 3 ��Ӳ����
    u16	alarmmode;		//�ᱨ��������  0 1 2������ƽ�ź�
    u16 poslimitsig;    //����λ�ź�
    u16 poslimitlev;    //����λ�źŵ�ƽ
    u16 neglimitsig;    //����λ�ź�
    u16 neglimitlev;    //����λ�źŵ�ƽ
    float softminlimit;   //����λ unit
    float softmaxlimit;   //������λ unit
} AxislimitDef;

//�ᵥλת��
typedef struct AxisConversion_t
{
    u32 PPR;	//ÿת������
    float MPR;	//ÿת����__����
} AxisConversion;

//�������
typedef struct AxisPara_t
{
    AxspdDef AxSpd;             //10���Ĵ���
    AxHomeDef Axhomecfg;        //10���Ĵ���
    AxislimitDef Axlimitcfg;    //10���Ĵ���
    AxisConversion Axconver;    //4���Ĵ���
} AxisData;

//modbus ��ַ������
typedef union {
    u8 Data_8[GLOBALDATALEN];
    u16 Data_16[GLOBALDATALEN / 2];
    struct
    {
        struct //READ ADDR 0-999
        {
            union //SYS READ ADDR 0-499 ϵͳֻ���Ĵ���
            {
                u16 Data_16[SYSREADLEN / 2];
                struct
                {
                    u32 HardWare_Ver[10];		  			//0-19	Ӳ���汾
                    u32 SoftWare_Ver[10];		  			//20-39	����汾
                    u32 res[5];                  			//40-49
                    u16 AXSTA[MAXAXISNUM];		  			//50-99	��״̬
                    float AxisUnitPosition[MAXAXISNUM];  	//100-199 �û���λ�ĵ�ǰλ��
                    s32 AxisPosition[MAXAXISNUM]; 			//200-299	�ᵱǰλ��
                    s32 AxisEncoder[5];			  			//300-309	�ᵱǰ״̬
                    u32 InputStatus[40];		  			//310-389	�����״̬
                    u32 ErrorCode[20];			  			//390-	������
                    u32 ErrorLevel;				  			//430-	����ȼ�
                    Time time;								//432	rtc ʱ��
                    Date date;								//434	rtc ����
                    u32 CID[2];								//436 438 �������ɵĿͻ������
					/* ��ʾ����ʱ�� */
					u16 year;		//440��ʾ����ʱ��
					u16 month;      //441
					u16 day;        //442
					u16 week;		//443ʣ��ʱ��
                } Data;
            } Sys;

            union //USER READ ADDR 500-999 �û�ֻ���Ĵ���
            {
                u16 Data_16[USERREADLEN / 2];
                struct
                {
					union
					{
						u32	Data_32[10];//500 518
						struct
						{
							u32 RunStatus; 				//500 ����״̬��ʾ
							u32 HaveToReset;    		//502 ��λ��־
							u32 HaveToRepower;  		//504 ������־
						}Data;
					}FsmData;
					
					union
					{
						u32	Data_32[50];//520 618
						struct
						{
							u32	PlateDownTime;			//520	͹����ͣ��ʱ��  ����1��ʼ�Ƕȵ�����1�����Ƕ�
							u32	AdjustTime[2];			//522 24	��λʱ��	����������һ������ʱ��
							u32	ElecTest1Time;			//526	����1ʱ��		����1����ʱ��
							u32 ElecTest2Time;			//528	����2ʱ��		����2����ʱ��
							u32	ElecTurn90Time;			//530	��ת90ʱ��		��ת��λ90��ʱ��
							u32	ElecTurn180Time;		//532	��ת180ʱ��		��ת��λ180��ʱ��
							u32	ElecTurn270Time;		//534	��ת270ʱ��		��ת��λ-90��ʱ��
							u32	DownCameraTime;			//536	�ײ��Ӿ����ʱ�� �������
							u32	PackMotorTime;			//538	װ����ʱ��	 װ����һ��
							u32	UpCameraTime;			//540	װ��������ʱ�� �������
							u32	PackPressTime;			//542	װ����ʱ��	 �������
						}Data;
					}TimeRecord;
					
					union
					{
						u32	Data_32[100];//620 818
						struct
						{
							s32	Have[20];				//620	�������� 0:û����	1������
							s32	Elec[20];				//660	���Խ�� -1��NG 0��δ���� 1��0�� 2��90�� 3��180�� 4��-90��	
							s32	OK[20];					//700	�Ƿ���� 0��NG 1��OK
						}Data;
					}PlateLoopData;
					
					union
					{
						u32	Data_32[40];//820 900
						struct
						{
							char FileName[8][20];	//�ļ����� 
						}Data;
					}FileName;
					
					
                } Data;
            } User;
        } Read;

        struct //WRITE ADDR 1000-1999 ��д�Ĵ��������������ݶ�ʧΪ0
        {
            union //SYS WRITE ADDR 1000-1499 ϵͳ��д�Ĵ��������粻����
            {
                u16 Data_16[SYSWRITELEN / 2];
                struct
                {
                    u32 res[10];		  	//1000-1019 ����
                    u32 OutputStatus[40]; 	//1020-1099	�����״̬
                    u32 FlashOperate;	 	//1100-	Flash����
                    u32 FilesOperate;	 	//1102-	�ļ�����
                    u32 FramOperate;	  	//1104-	�������
                    u32 JogMode;		  	//1106-	�㶯ģʽ 0���� 1�г� 2�ٶ�ģʽ(�ߵ���λ)
                    u32 JogPositionLevel; 	//1108-	�㶯�г̼���0 1 2 3 4
                    u32 JogPosition;	  	//1110-	�㶯�趨�г� 
                    u32 JogForward[2];		//1112-1115	����㶯
                    u32 JogBackward[2];   	//1116-1119	����㶯
                    u32 JogGohome[2];	 	//1120-1123	�㶯����
                    u32 JogStop[2];		  	//1124-1127	�㶯����ֹͣ
                    u32 jogSpd;			  	//1128  ��㶯�ٷֱ��ٶ�hz
                    u32 ClearAlarm;		  	//1130-	�������
                    struct {
                        u32 refreshId;	//	1132 �����ˢ��
                        s32 decode[4];	// 1134 1136 1138 1140  4�������� *6���� һ��24����
                    } LockPara;
                } Data;
            } Sys;

            union //USER WRITE ADDR 1500-1999 �û���д�Ĵ���
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
							u32 Login;					//1506  ע�ᰴť
						}Data;
                    } FsmData;
					
					union
					{
						u32	Data_32[20];//1520 1558
						struct
						{
							s32	Motion;					//1520	ͨ�ö�������
							s32	PackMove;				//1522	�����ߴ�����
							s32	Feed;					//1524	�������Ͽ���
							s32	PassPackNg;				//1526	ǿ��ͨ��װ��Ӱ��NG
							u32	JogSpeed;				//1528	�㶯�ٶ� 0������	1������
							u32	ETurnHome;				//1530	��תʹ�� 1������
							u32	KeepETest;				//1532	��������
							u32	KeepBeltMove;			//1534	�����ߴ�
							u32	KeepFeed;				//1536	��������
						}Data;
					}ManualData;
					
 					union
					{
						u32	Data_32[20];//1560 1598
						struct
						{
							s32	PackCameraOk;			//1560	װ���Ӿ����
							s32	DownCameraOk;			//1562	�ײ��Ӿ����
							s32	ETestResult;			//1564	���Բ��Խ��
							s32	Num_ETestNg;			//1566	��������NG����
							s32	Num_ETestSame;			//1568	�������Խ����ͬ����
							s32	Num_RepackNg;			//1570	��������ʧ�ܱ���
							s32	Num_DownCameraNg;		//1572	���������NG����
							s32	Num_NoBeltSupply;		//1574	�������ʹ�����
							s32 Num_BeltCollect;		//1576  �մ����ֹͣ�����������
						}Data;
					}ProcessData;
					
					union
					{
						u32	Data_32[20];//1600 1638
						struct
						{
							s32 RunSpeed;				//1600	�����ٶ�
							float PackSpeed;			//1602	װ���ٶ�
							float ETestNgRatio;			//1604	���Բ�������
							float DownCameraNgRatio;	//1606	�ײ���������
							s32	PlatePosCmd;			//1608	ת��λ��ָ�Ȧֵ
							s32	PlatePosEnc;			//1610	ת��λ�÷�����Ȧֵ
							s32 File_Chiose;			//1612  �ļ�������
							s32 File_Write;				//1614  �ļ�д�뱾��
							s32 File_Read;				//1616  ����д���ļ�
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
							u32 DateRequest;//1648	1649 ʱ������
						}Data;
					}DisplayTime;
               } Data;
            } User;
        } Write;

        struct //SAVE ADDR �洢����ַ�Ĵ���
        {
            union //SYS SAVE ADDR 2000-3999 //ϵͳ�洢�Ĵ�����ַ
            {
                u16 Data_16[SYSSAVELEN / 2];
                struct
                {
                    AxisData axis[MAXAXISNUM];       //������в��� 34*50  2000-3699
                    struct
                    {  
                        u32 checkflag;  //��ʼ����⣬���°��ûд�����ݻ������ݲ��ǹ涨ֵʱ����Ҫ��ʼ��modbus�Ĵ洢�Ĵ�����ֵ����ֹ�����¿��ϵ�ȫff��������պ󣬵��µ�ײ���������������ʼ�������ݺ�д0xea
                        u32 res[9];     //Ԥ������������ʱ���
                    } SaveCheck; 		//3700-3719

                } Data;
            } Sys;

            union //USER SAVE ADDR 4000- 5999//�û��洢�Ĵ�����ַ
            {
                u16 Data_16[USERSAVELEN / 2];
                struct
                {
					//�������������Ȩ�ޱ���
					union
					{
						u32 Data_32[100];
						struct
						{
							s32	HomeSpeed[10];		//4000	�����ٶ�
							s32	RunSpeed[10];		//4020	�����ٶ�
							s32	AccDecTime[10];		//4040	�Ӽ���ʱ��
							s32	OriginDir[10];		//4060	ԭ���ƽ
							s32	AlarmDir[10];		//4080	������ƽ
							s32	MotorDir[10];		//4100	�������
							s32	JogSpeed[10];		//4120	�ֶ��ٶ�
							s32	EnableDir[10];		//4140	ʹ�ܵ�ƽ
						}Data;
					}MotorData;
					//���ò���������Ȩ�ޱ���
					union
					{
						u32 Data_32[50];
						struct
						{
							s32	FuncSel_AdjustMode;			//4200	��λģʽ	0�����岽��	1��IO����
							s32	FuncSel_PackHomeMode;		//4202	װ����	0�����岽��	1��IO����	2�������ŷ�
							s32	FuncSel_HopperMode;			//4204	�϶�ģʽ	0������� 1������-�͵�ƽ 2������-�ߵ�ƽ 3�����
							s32	FuncSel_PackCameraMode;		//4206	װ�����	0������ʿ	1������
							s32	FuncSel_DownCameraMode;		//4208	�ײ����	0������ʿ	1������
						}Data;
					}ConfigData;
					//���ò���������ʦȨ�ޱ���
                    union
					{
						u32 Data_32[50];
						struct
						{
							s32	AgingMode;					//4300	�ϻ�ģʽ	0���������� 1���ϻ�����
							s32	FinishStop;					//4302	����ͣ��	0��������ɲ�ͣ���� 1���������ͣ��
							s32	LightIn;					//4304	���Դ
							s32	LightOut;					//4306	���Դ
							s32	Hopper;						//4308	�϶�
							s32	Beep;						//4310	������
							s32	RePack;						//4312	����
							s32	ElecTest;					//4314	���Բ���	0����	1����
							s32	ElecTest2;					//4316	��վ����	0����	1����
							s32	DownCamera;					//4318	�ײ��Ӿ�	0����	1����
							s32	Pack;						//4320	װ��		0����	1����
							s32	PackCamera;					//4322	װ���Ӿ�	0����	1����
						}Data;
					}EnableData;
					union
					{
						u32	Data_32[50];
						struct
						{
							s32	Num_NoPress;				//4400	��������
							s32	Num_BeforPack;				//4402	ǰ�ո�����
							s32	Num_Pack;					//4404	Ŀ��װ������
							s32	Num_BehindPack;				//4406	��ո�����
							s32	Num_Sample;					//4408	��Ʒ����
							s32	Num_SampleBehind;			//4410	��Ʒ��ո�����
							s32	Num_TargetReel;				//4412	Ŀ���������
							s32 Num_ReelPack;				//4414  һ����Ҫ���ٱ������(PCS)
						}Data;
					}ProductData;
					union
					{
						u32	Data_32[50];
						struct
						{
							s32	Tim_PressDown;				//4500	�����ѹʱ��
							s32	Tim_PressUp;				//4502	���̧��ʱ��
							s32	Tim_Hopper;					//4504	�϶�������
							s32	Tim_Supply;					//4506	�ϴ����һ��ת��ʱ��
							s32	Tim_Clean;					//4508	��ϴ����ʱ��
							s32	Tim_Blow;					//4510	��������ʱ��
							s32	Tim_PackCameraDelay;		//4512	װ��Ӱ����ʱ
							s32	Tim_DownCameraDelay;		//4514	�ײ�Ӱ����ʱ
							s32 Tim_HopperKeep;				//4516	Բ��ȱ���˲�	��
							s32 Tim_HopperTime;				//4518	�϶��񶯴���	��
						}Data;
					}TimeData;
					union
					{
						u32	Data_32[50];
						struct
						{
							s32	Num_ETestNg;				//4600	��������NG����
							s32	Num_ETestSame;				//4602	�������Խ����ͬ����
							s32	Num_RepackNg;				//4604	��������ʧ�ܱ���
							s32	Num_DownCameraNg;			//4606	���������NG����
							s32	Num_DiscardFull;			//4608	����Ͳ���ϱ���
							s32	Num_NewBelt;				//4610	һ�����ش�����
							s32	Num_NewFilm;				//4612	һ���½�Ĥ����
							s32	Num_NoBeltSupply;			//4614	�������ʹ�����
							s32 Num_RepackNum;				//4616	���ϼ���	��
						}Data;
					}AlarmData;
					union
					{
						u32	Data_32[50];
						struct
						{
							s32	Pos_Ready;					//4700	Ԥ��λ
							s32	Num_SwitchToFast;			//4702	����������������Ͽ�������ɽ��ϣ��л�������
							s32	Num_SwitchToSlow;			//4704	�����������������Ͽ�����δ��ɽ��ϣ��л�������
							
							s32	Pos_ETestStart;				//4706	���Կ�ʼ�Ƕ�
							s32	Pos_ETestEnd;				//4708	���Խ����Ƕ�
							s32	Pos_ETest2Start;			//4710	��վ���Կ�ʼ�Ƕ�
							s32	Pos_ETest2End;				//4712	��վ���Խ����Ƕ�
							s32	Pos_DownCameraStart;		//4714	���Ӿ���ʼ�Ƕ�
							s32	Pos_DownCameraEnd;			//4716	���Ӿ������Ƕ�
							s32	Pos_Blow1Start;				//4718	����1��ʼ�Ƕ�
							s32	Pos_Blow1End;				//4720	����1�����Ƕ�
							s32	Pos_Blow2Start;				//4722	����2��ʼ�Ƕ�
							s32	Pos_Blow2End;				//4724	����2�����Ƕ�
							s32	Pos_Blow3Start;				//4726	����3��ʼ�Ƕ�
							s32	Pos_Blow3End;				//4728	����3�����Ƕ�
							s32	Pos_Blow4Start;				//4730	����4��ʼ�Ƕ�
							s32	Pos_Blow4End;				//4732	����4�����Ƕ�
							s32	Pos_Detect1Start;			//4734	�����Ӧ1��ʼ�Ƕ�
							s32	Pos_Detect1End;				//4736	�����Ӧ1�����Ƕ�
							s32	Pos_Detect2Start;			//4738	�����Ӧ2��ʼ�Ƕ�
							s32	Pos_Detect2End;				//4740	�����Ӧ2��ʼ�Ƕ�
							s32	Pos_PackStart;				//4742	װ�ʼ�Ƕ�
							s32	Pos_PackEnd;				//4744	װ������Ƕ�
							s32	Pos_AdjustStart;			//4746	��λ��ʼ�Ƕ�
							s32	Pos_AdjustEnd;				//4748	��λ�����Ƕ�
							s32	Pos_FeedStart;				//4750	���Ͽ�ʼ�Ƕȣ�������
							s32	Pos_FeedEnd;				//4752	���Ͻ����Ƕȣ�������
							s32	Pos_ETurnStart;				//4754	��ת����ʼ�Ƕ�
							s32	Pos_ETurnEnd;				//4756	��ת��������Ƕ�
							s32	Spd_Fast;					//4758	͹�ֿ����ٶ�
							s32	Spd_Slow;					//4760	͹�������ٶ�
							s32 Pos_FeedVacuo1ON;			//4762	�������1��
							s32 Pos_FeedVacuo1OFF;			//4764	�������1��
							s32 Pos_FeedVacuo2ON;			//4766	�������2��
							s32 Pos_FeedVacuo2OFF;			//4768	�������2��
							s32 Pos_FeedVacuo3ON;			//4770	�������3��
							s32 Pos_FeedVacuo3OFF;			//4772	�������3��
							s32 Pos_FeedBlowON;				//4774	���Ϸ��봵����
							s32 Pos_FeedBlowOFF;			//4776	���Ϸ��봵����
							s32	Pos_InpStart;				//4778	��λ��⿪ʼ
							s32	Pos_InpEnd;					//4780	��λ������
						}Data;
					}PlateData;

					union
					{
						u32	Data_32[50];
						struct
						{
							s32	Pos_Ready[2];				//4800	��λ����Ԥ��λ
						}Data;
					}AdjustData;
					union
					{
						u32	Data_32[50];
						struct
						{
							s32	Dir_Num;					//4900	���Է�������
							s32	Dir_Target;					//4902	����Ŀ�귽��
							s32	Dir_Target2;				//4904	��վ���Է���
						}Data;
					}ETestData;
					union
					{
						u32	Data_32[50];
						struct
						{
							s32	Pos_Ready;					//5000	װ��Ԥ��λ
							s32	Pos_Feed;					//5002	װ����һ������
							s32	Idx_Camera;					//5004	�Ӿ�λ��
							s32	Idx_Press;					//5006	�⵶λ��
							s32	Idx_Cut;					//5008	�д�λ��
							s32	Num_Press;					//5010	�⵶���
							s32	Num_RepackBlow;				//5012	���ϴ�������
						}Data;
					}PackData;
					union
					{
						u32 Data_32[50];
						struct
						{
							s32	Num_FinishReel;				//5100	�ѱ����
							s32	Num_Packed;					//5102	��װ����
							s32	Num_NgDiscard1;				//5104	��װ1�Ų�����Ͳ1
							s32	Num_OkDiscard1;				//5106	��װ1����Ʒ��Ͳ2
							s32	Num_NgDiscard2;				//5108	��װ2�Ų�����Ͳ3
							s32	Num_OkDiscard2;				//5110	��װ2����Ʒ��Ͳ4
							s32	Num_PackBlet;				//5112	����һ���ش�����
							s32	Num_PackFilm;				//5114	����һ��Ĥ����
							s32	Num_HistoryReel;			//5116	��ʷ�ѱ����
							s32	Num_HistoryPack;			//5118	��ʷ��װ����
							s32	PackStep[10];				//5120	���������¼����10��λ�ã���������ǰ�������ϵ��ν�
						}Data;
					}RecordData;
					
					union
					{
						u32 Data_32[50];
						struct
						{
							s32	Idx_ETest;					//5200	���Թ�λ��
							s32	Idx_Etest2;					//5202	��վ���Թ�λ��
							s32	Idx_ETurn;					//5204	����λ��
							s32	Idx_DownCamera;				//5206	���Ӿ���λ��
							s32	Idx_Blow1;					//5208	����1��λ��
							s32	Idx_Blow2;					//5210	����2��λ��
							s32	Idx_Blow3;					//5212	����3��λ��
							s32	Idx_Blow4;					//5214	����4��λ��
							s32	Idx_Detect1;				//5216	������ϸ�Ӧ1��λ��
							s32	Idx_Detect2;				//5218	������ϸ�Ӧ2��λ��
							s32	Idx_Pack;					//5220	װ�λ��
						}Data;
					}StationData;
					
					union
					{
						u32 Data_32[50];//5300
						struct
						{
							s32 TransWatch1;				//5300	����ת��	��
							s32	IO_Trans_Hopper;			//5302	�϶��ź��߼���ת	��
							char FileName_save[20];			//5304  �ļ�����10 5312
						}Data;
					}TransWatch;
				} Data;
            } User;
        } Save;
        u32 Check; //У����
    } SpaceType;
} GlobalDataDef;
extern GlobalDataDef GlobalData;

//��ݺ�
#define GSR GlobalData.SpaceType.Read.Sys.Data
#define GSW GlobalData.SpaceType.Write.Sys.Data
#define GSS GlobalData.SpaceType.Save.Sys.Data
#define GUR GlobalData.SpaceType.Read.User.Data
#define GUW GlobalData.SpaceType.Write.User.Data
#define GUS GlobalData.SpaceType.Save.User.Data

//��ݺ궨��
#define	FSM_STA GUR.FsmData.Data.RunStatus
#define FSM_CMD GUW.FsmData.Data.RunCommand

#define	STATIONNUM	12		//ת�̹�λ��
#define	PPR_PLATE	1000	//ת��һȦ������
#define	PPR_ADJUST	1000	//��λ��������һȦ������
#define	PPR_PACK	1000	//װ��һ��������


#endif
