/**
  ******************************************************************************
  * File Name          : USART.h
  * Description        : This file provides code for the configuration
  *                      of the USART instances.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __usart_H
#define __usart_H
#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

extern UART_HandleTypeDef huart1;

/* USER CODE BEGIN Private defines */
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
/* USER CODE END Private defines */

void MX_USART1_UART_Init(void);

/* USER CODE BEGIN Prototypes */
UART_COMMON *Uart_init(struct UART_COMMON *this, u8 port);		//串口初始化，port参数填入u8类型的UART_PORT枚举变量
/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif
#endif /*__ usart_H */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
