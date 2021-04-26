#include "bsp_config.h"
#include "can_master.h"

#ifndef			ARM
#define 		FPGA
#endif


/**
* 板级硬件初始化
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
	/* 串口初始化：波特率115200 */
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
* 硬件接口轮询
*/
void bsp_exec(void)
{
    /* 串口轮询 */
    UART_exeu();
#if __USE_ETH__
    /* 以太网轮询 */
    Eth_excu();
#endif
    /* USB轮询 */
    USB_Mass_exec();
    /* 掉电检测 */
    pwr_exec();
    /* 编码器接口轮询 */
    EnCode_Exec();
    /* RUN灯 */
    Led_Exec();
#ifdef  FPGA
	/* FPGA轮询 */
    FSMC_FPGA_Exec();
#endif
	/* 扩展卡轮询 */
    can_netLoop();
	/* 铁电轮询 */
    FRam_Exec();
}


