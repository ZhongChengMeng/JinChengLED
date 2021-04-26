#ifndef __BSPLIB_H
#define __BSPLIB_H

#include "stdint.h"


typedef int64_t  s64;
typedef int32_t  s32;
typedef int16_t s16;
typedef int8_t  s8;

typedef uint64_t  u64;
typedef uint32_t  u32;
typedef uint16_t u16;
typedef uint8_t  u8;

typedef const uint32_t uc32;  /*!< Read Only */
typedef const uint16_t uc16;  /*!< Read Only */
typedef const uint8_t uc8;   /*!< Read Only */

/**
* ϵͳ��ʼ��	
* �ϵ��ȵ��ô˺���
*/
extern void sys_init(void);
extern void sys_init_IAP(void);		

/**
* ����
*/
#define UART_NUM			2
#define DB485_SN			0
#define DB232_SN			1
extern void UART_exeu(void); 
//UART
//fun	:��ʼ������
//num 	:���ں�
//tx_buf:���ͻ�����ָ��
//rx_buf:���ջ�����ָ��
//bps	:������
//SilenceTime	:��Ĭʱ�䣨��λ100us�������ڽ��ն�֡��
//return 		:0����
extern s32 Uart_Init(u32 num,void *tx_buf,void * rx_buf,u32 rx_Maxlen,u32 bps,u32 silenceTime);
//fun	:���ڷ�������
//buf	:���ͻ�����
//len	:���ͳ���
//return :0����1:����æ -1����������
extern s32 Uart_Send(u32 num,void *buf,u32 len);
//fun		:���մ�������
//buf		:���ջ�����
//len		:���ճ���
//return	:0û�յ������������ճ���
extern u32 Uart_Recv(u32 num,void *buf);

/**
* ��̫��
*/
//fun 		:��̫���ӿڳ�ʼ��
//rx_buf	:���ջ�����
//rx_Maxlen	:���ջ�������󳤶�
//return 	:0:�ɹ� ����:ʧ��
extern s32 Eth_Init(void * rx_buf,u32 rx_Maxlen);
//��̫������
//return 0:δ���գ��������������ݳ���
extern u32 Eth_Recv(void);
//��̫������
//tx_buf:���ͻ����ַ
//tx_len:�������ݳ���
extern u32 Eth_Send(u8 * tx_buf,u32 tx_len);

//��Ҫ������ѯ�ĺ���
extern void Eth_excu(void);
/*
* Ĭ�ϰ忨ip:192.168.1.30  �˿ں�8080
* Ĭ��Զ��ip:192.168.1.100 �˿ں�8080
*/
//��������Զ��IP��˿ں�
extern void Eth_SetRmt(u8 ip_HH,u8 ip_HL,u8 ip_LH,u8 ip_LL,u16 port);
//���������Լ�IP��˿�
extern void Eth_SetLoc(u8 ip_HH,u8 ip_HL,u8 ip_LH,u8 ip_LL,u16 port);


/**
* USB�洢
*/
//USB(FAT32�ļ�ϵ�y)
void USB_Mass_Init(void);
void USB_Mass_exec(void);



/**
* ϵͳ��ʱ��
*/
#define TIMER_STA_PAUSE		0
#define TIMER_STA_CONTINUE	1

extern void SysTim_Init(void);		//ϵͳʱ�ӳ�ʼ������λ0.1ms

//�û�ʱ����ͣ 0:��ͣ��1������
extern void SysTimer_SetSta(u8 Sta);
//�û�ʱ��״̬��ȡ 0:��ͣ��1������
extern u8 SysTim_GetSta(void);
//��ȡ�û�ʱ�䣨������ͣ����λ0.1ms��
extern u32 SysTim_GetUser(void);
//��ȡϵͳʱ�䣨������ͣ��λ0.1ms��
extern u32 SysTim_GetSys(void);




/**
* IO��
*/

extern void gp_io_init(void);
//���������״̬
//num : ��ţ���0��ʼ��
//val : ״̬ 0��ON 1:OFF
//return ��0 �ɹ� ��������������
extern s32 OutPut_SetSta(u8 num,u8 val);

//��ȡ�����״̬
//num : ��ţ���0��ʼ��
//return ��0��ON 1:OFF��������������
extern s32 OutPut_GetSta(u8 num);
//��ȡ�����״̬
//num : ��ţ���0��ʼ��
//return ��0��ON 1:OFF��������������
extern s32 InPut_GetSta(u8 num);

//��ȡ����״̬
//num:��ţ���0��ʼ��
//return :0 :on 1:off, ������ʾ��������
//����
extern s32 Button_GetSta(u8 num);


//���÷��������״̬
//num : ��ţ���0��ʼ��
//val : ״̬ 0��1
//return ��0 �ɹ� ��������������
extern s32 Dir_SetSta(u8 num,u8 val);

//��ȡ���������״̬
//num : ��ţ���0��ʼ��
//return ��0��1:OFF��������������
extern s32 Dir_GetSta(u8 num);


//����ʹ�������״̬
//num : ��ţ���0��ʼ��
//val : ״̬ 0��1
//return ��0 �ɹ� ��������������
extern s32 EN_SetSta(u8 num,u8 val);

//��ȡʹ�������״̬
//num : ��ţ���0��ʼ��
//return ��0��1��������������
extern s32 EN_GetSta(u8 num);
//��ȡ���������״̬
//num : ��ţ���0��ʼ��
//return ��0��1��������������
extern s32 ALM_GetSta(u8 num);

//��ȡ��λ��״̬
//num : ��ţ���0��ʼ��
//return ��0��1��������������
extern s32 LIMIT_GetSta(u8 num);


/**
* can��
*/
//CAN��ʼ����֧��1000��500��250kbps ������ֵ��һ��500kbps
extern  void Can_Init(u32 Kbps);										 							 				    
//u8 CAN1_Mode_Init(u8 tsjw,u8 tbs2,u8 tbs1,u16 brp,u8 mode);//CAN��ʼ��
extern  void CAN_exeu(void);
//ID��С��0x800 �Զ�������չ֡��
//return ��0���ɹ� -1����һ֡û����
extern  s32 Can_SendData(u32 id,u8 len,u8* buf);
//return 0:���У�1��busy
extern  u8 Can_GetSendSta(void);
//return -1:û���յ����� �������������ݳ���
extern  s8 Can_GetRecvData(u32 * id,u8* buf);

/**
* run��
*/
extern void Led_Init(void);
extern void Led_Exec(void);

/**
* flash�洢
*/
//д����������
//buf :Ҫ���������buf
//size:��С����λByte ��� 120*1024 ֻ��Ϊ4��������
//return 0������ -1���������� ����������ʧ��
extern s32 Flash_WriteData(void *buf,u32 size);
//������������
//buf :����bufָ��
//size:��С����λByte ��� 120*1024 ֻ��Ϊ4��������
//return 0������ -1����������
extern s32 Flash_ReadData(void *buf,u32 size);
//д����������
//buf :Ҫ���������buf
//size:��С����λByte ��� 120*1024 ֻ��Ϊ4��������
//return 0������ -1���������� ����������ʧ��
extern s32 Flash_WriteBkData(void *buf,u32 size);
//������������
//buf :����bufָ��
//size:��С����λByte ��� 120*1024 ֻ��Ϊ4��������
//return 0������ -1����������
extern s32 Flash_ReadBkData(void *buf,u32 size);


/**
*RTC
*/
//RTC��ʼ��
extern u8 RTC_Init(void);						//RTC��ʼ��
//RTCʱ������
extern u8 RTC_Set_Time(u8 hour,u8 min,u8 sec,u8 ampm);
//RTC��������
extern u8 RTC_Set_Date(u8 year,u8 month,u8 date,u8 week);
//��ȡRTCʱ��
extern void RTC_Get_Time(u8 *hour,u8 *min,u8 *sec,u8 *ampm);
//��ȡRTC����
extern void RTC_Get_Date(u8 *year,u8 *month,u8 *date,u8 *week);

//���ñ���������
//��С4k
//offset :������ƫ���������֣�4Byte��Ϊ��λ
//src :Ҫ�����Դ����ָ�루��
//size:���ݴ�С,��λByte��֤��4 * offset + size < 0x1000)
//return 0:z���� -1����������
extern s32 RTCSetBkSramData(u32 offset,void *src,u32 size);
//��ȡ����������
//��С4k
//offset :������ƫ���������֣�4Byte��Ϊ��λ
//tar :Ҫ�����Ŀ������ָ�루��
//size:���ݴ�С,��λByte��֤��4 * offset + size < 0x1000)
//return 0:z���� -1����������
extern s32 RTCGetBkSramData(u32 offset,void *tar,u32 size);

/*
*pwr	��Դ����
*/
extern void pwr_init(void);
extern void pwr_exec(void);
extern s32 pwr_Task(void);	//�ⲿʵ�֣���Ҫ���ⲿʵ�ֵĺ�����

/**
*��������*
*/
extern void Puls_Init(void);
extern void Puls_Exec(void);
extern s32 Puls_Stop(u8 num);
extern s32 Puls_Start(u8 num,u32 steps);
extern s32 Puls_SetWidth(u8 num,float us);
extern s32 Puls_GetSta(u8 num);
extern u32 Puls_GetRan(u8 num);
extern u32 Puls_GetRun(u8 num);
extern s32 Puls_SetSpd(u8 num,u32 spd);
//����(�������˶��иı��г�)
extern u32 Puls_SetRun(u8 num,u32 steps);


//ECODE ������
//Ӳ����ʼ��
extern void EnCode_Init(void);
//��Ҫ��ѯ��ִ�к���
extern void EnCode_Exec(void);
//��ȡ��Ӧ������32λ����
extern s32 EnCode_Get32(u8 num);
s32 EnCode_GetGpioVal(u8 num,u8 pin);
void EnCode_SetVal(u8 num,s32 val);

//FILE �ļ�����
//20180704 zwg
//д���ļ������û���򴴽�
//name		: �ļ������ڵ��ַ�����Ϊ0ʱ��num�����ã�
//buf 		: ��Ҫд����������ڵĵ�ַ��ָ�룩
//num 		: ��Ҫд����ļ����кţ�������nameΪ0ʱ�����ã�
//len 		: ��Ҫд��ĳ���
//offset	: ��Ҫд����ļ�ƫ�Ƶ�ַ�����ļ���offset����ʼд��
//return	: �������� 
extern u8 File_Write(char * name,void * buf,u32 num, u32 len,u32 offset);

//20180704 zwg
//���ļ�
//name		: �ļ������ڵ��ַ�����Ϊ0ʱ��num�����ã�
//buf 		: ��Ҫ�����������ڵĵ�ַ��ָ�룩
//num 		: ��Ҫ�����ļ����кţ�������nameΪ0ʱ�����ã�
//len 		: ��Ҫ���ĳ���
//offset	: ��Ҫ�����ļ�ƫ�Ƶ�ַ�����ļ���offset����ʼд��
//return	: ��������
extern u8 File_Read(char * name,void * buf,u32 num, u32 len,u32 offset);

//20180704 zwg
//���ļ���С
//name		: �ļ������ڵ��ַ�����Ϊ0ʱ��num�����ã�
//num 		: ��Ҫ�����ļ����кţ�������nameΪ0ʱ�����ã�
//return	: �Ǹ�:�ļ���С
//			: ��  :����
extern s32 File_GetSize(char * name,u32 num);

//20180704 zwg
//ɾ�ļ�
//name		: �ļ������ڵ��ַ�����Ϊ0ʱ��num�����ã�
//num 		: ��Ҫɾ�����ļ����кţ�������nameΪ0ʱ�����ã�
//return	: ��������
extern u8 File_Delete(char * name,u32 num);

//20180704 zwg
//ɨ���ļ����ļ��б�
//name �ļ�������(����Ҫ��ǰ��ġ�/��)
//����ֵ��0: ����
//		������ �б��ַ
extern char* File_List(char * name);

//ѡ��洢����
//Obj	0�����̣�1��nandflash
extern void File_SelObj(u8 Obj);

//��ʼ������
void FM25LXXB_Init(void);
//����״̬��ѯ����
void FRam_Exec(void);
//��ȡ����Ĳ���״̬
u8 FRAM_GetSta(void);
//writeaddr:��ʼд��ĵ�ַ
//writebuff:���ݴ洢��
//length:д����ֽ���
u8 FRam_Write(u32 writeaddr, void* writebuff, u32 size);
//readaddr:��ʼ��ȡ�ĵ�ַ
//readbuff:���ݴ洢��
//length:��ȡ���ֽ���
u8 FRam_Read(u32 readaddr, void* Readbuff, u32 size);
//ѡ�����磺0��1
void FM25LXXB_Sel(u8 obj);

//Plus
#define PULS_NUM				6			//���壨�ᣩ����
#define	PULS_NUM_Ex				0			//��չ������

//ECode
#define ECD_NUM					3			//����������
#define ECD_FLUSH_INTERVAL		1000		//������32λ���¼����0.1ms(������Ҫ�ܸ�Ƶ�ʣ�ֻ��Ҫ��16λ���ʱ��֮�ڸ���һ�ξͺ�)

#define	GPO_NUM	38
#define	GPI_NUM 40
#define	GPO_NUM_Ex 0
#define	GPI_NUM_Ex 0


#endif

