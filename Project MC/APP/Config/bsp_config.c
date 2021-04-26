#include "bsp_config.h"
#include "can_master.h"

#ifndef			ARM
#define 		FPGA
#endif


/**
* �弶Ӳ����ʼ��
*/
void bsp_init(void)
{
#ifdef  FPGA
    FSMC_FPGA_Init();
#endif
    USB_Mass_Init();
    gp_io_init();
//    Adc_Init();
//    Dac_Init();
    pwr_init();
    RTC_Init();
    EnCode_Init();
    Puls_Init();
    Led_Init();
    SysTim_Init();
    Can_Init(1000);
	/* ���ڳ�ʼ����������115200 */
    Uart_Init(DB485_SN,uart_sendbuf[0],uart_recvbuf[0],300,19200,30);
    Uart_Init(DB232_SN,uart_sendbuf[1],uart_recvbuf[1],300,19200,30);
#if __USE_ETH__
    Eth_SetRmt(192,168,1,100,8089);
    Eth_SetLoc(192,168,1,30,8089);
    Eth_Init(eth_rxbuf,sizeof(eth_rxbuf));
#endif
    FM25LXXB_Init();
}

/**
* Ӳ���ӿ���ѯ
*/
void bsp_exec(void)
{
    /* ������ѯ */
    UART_exeu();
#if __USE_ETH__
    /* ��̫����ѯ */
    Eth_excu();
#endif
    /* USB��ѯ */
    USB_Mass_exec();
    /* ������ */
    pwr_exec();
    /* �������ӿ���ѯ */
    EnCode_Exec();
    /* RUN�� */
    Led_Exec();
#ifdef  FPGA
	/* FPGA��ѯ */
    FSMC_FPGA_Exec();
#endif
	/* ��չ����ѯ */
    can_netLoop();
	/* ������ѯ */
    FRam_Exec();
}


