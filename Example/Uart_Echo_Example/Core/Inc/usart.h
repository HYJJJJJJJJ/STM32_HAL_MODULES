/*
 * usart.h
 *
 *  Created on: Jun 29, 2020
 *      Author: HYJ
 */

#ifndef __usart_H
#define __usart_H
#ifdef __cplusplus
 extern "C" {
#endif

#include "main.h"
#include "gpio.h"

extern UART_HandleTypeDef huart1;

#define MAX_RX_LENGTH 128		//最大接收长度
#define MAX_TX_LENGTH 128		//最大发送长度

#ifdef STM32F103xE
enum {
	EN_USART1=1,EN_USART2,EN_USART3,EN_UART4,EN_UART5
}UART_PROT;
#else
enum {
	EN_USART1=1,EN_USART2,EN_USART3
}UART_PROT;
#endif

typedef struct UART_COMMON{
	UART_HandleTypeDef uart_handle;
	u8 rx_flag;
	u16 rx_len;
	u8 rx_buf[MAX_RX_LENGTH];
	u8 tx_buf[MAX_TX_LENGTH];
	void (*UClearRec)(struct UART_COMMON *this);
	void (*UPrintf)(struct UART_COMMON *this,char *fmt, ...);
}UART_COMMON;

UART_COMMON *new_Uart(struct UART_COMMON *this, u8 port,u32 baud);		//串口初始化，port参数填入u8类型的UART_PORT枚举变量

#ifdef __cplusplus
}
#endif
#endif /*__ usart_H */
