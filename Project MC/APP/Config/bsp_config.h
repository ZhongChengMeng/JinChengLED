#ifndef __BSP_CONFIG_H
#define __BSP_CONFIG_H
#include "bsplib.h"

#define   __USE_ETH__			(0)
#define	  __USE_IAP__			(0)

extern u8 uart_sendbuf[][300];
extern u8 uart_recvbuf[][300];
extern u8 eth_txbuf[];
extern u8 eth_rxbuf[];

/*  */
extern void bsp_init(void);
extern void bsp_exec(void);

#endif

