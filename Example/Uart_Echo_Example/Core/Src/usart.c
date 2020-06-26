/**
  ******************************************************************************
  * File Name          : USART.c
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

/* Includes ------------------------------------------------------------------*/
#include "usart.h"

/* USER CODE BEGIN 0 */
/*
 * usart.c
 *
 *  Created on: Jun 27, 2020
 *      Author: HYJ
 */
u8 aRxBuffer[1]; //HAL库使用的串口接收缓冲
struct UART_COMMON *uart1_common;
/* USER CODE END 0 */

UART_HandleTypeDef huart1;

/* USART1 init function */

void MX_USART1_UART_Init(void)
{

  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }

}

void HAL_UART_MspInit(UART_HandleTypeDef* uartHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(uartHandle->Instance==USART1)
  {
  /* USER CODE BEGIN USART1_MspInit 0 */

  /* USER CODE END USART1_MspInit 0 */
    /* USART1 clock enable */
    __HAL_RCC_USART1_CLK_ENABLE();
  
    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**USART1 GPIO Configuration    
    PA9     ------> USART1_TX
    PA10     ------> USART1_RX 
    */
    GPIO_InitStruct.Pin = GPIO_PIN_9;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_10;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* USART1 interrupt Init */
    HAL_NVIC_SetPriority(USART1_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(USART1_IRQn);
  /* USER CODE BEGIN USART1_MspInit 1 */
    HAL_UART_Receive_IT(&huart1, (u8*) aRxBuffer, 1);
  /* USER CODE END USART1_MspInit 1 */
  }
}

void HAL_UART_MspDeInit(UART_HandleTypeDef* uartHandle)
{

  if(uartHandle->Instance==USART1)
  {
  /* USER CODE BEGIN USART1_MspDeInit 0 */

  /* USER CODE END USART1_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_USART1_CLK_DISABLE();
  
    /**USART1 GPIO Configuration    
    PA9     ------> USART1_TX
    PA10     ------> USART1_RX 
    */
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_9|GPIO_PIN_10);

    /* USART1 interrupt Deinit */
    HAL_NVIC_DisableIRQ(USART1_IRQn);
  /* USER CODE BEGIN USART1_MspDeInit 1 */

  /* USER CODE END USART1_MspDeInit 1 */
  }
} 

/* USER CODE BEGIN 1 */
void USART1_IRQHandler(void) {
	u32 timeout = 0;
	u32 maxDelay = 0x1FFFF;
#if SYSTEM_SUPPORT_OS	 	//使用OS
		OSIntEnter();
	#endif

	HAL_UART_IRQHandler(&huart1);	//调用HAL库中断处理公用函数

	timeout = 0;
	while (HAL_UART_GetState(&huart1) != HAL_UART_STATE_READY)	//等待就绪
	{
		timeout++;	////超时处理
		if (timeout > maxDelay)
			break;
	}

	timeout = 0;
	while (HAL_UART_Receive_IT(&huart1, (u8*) aRxBuffer, 1) != HAL_OK)//一次处理完成之后，重新开启中断并设置RxXferCount为1
	{
		timeout++; //超时处理
		if (timeout > maxDelay)
			break;
	}
}


void uart_clear_rec(struct UART_COMMON *this)
{
	this->rx_flag=0;
	this->rx_len=0;
}

void uart_printf(struct UART_COMMON *this,char *fmt, ...) {			//通过任意串口发送字符串
	u16 i;
	va_list ap;
	va_start(ap, fmt);
	vsprintf((char*) this->tx_buf, fmt, ap);
	va_end(ap);
	i = strlen((const char*) this->tx_buf);		//此次发送数据的长度
	HAL_UART_Transmit(&this->uart_handle, (uint8_t*) this->tx_buf, i, 0xffff);
}

void Uart_init(struct UART_COMMON *this, u8 port)						//串口的初始化与相关函数注册
{
	if(port==EN_USART1){
		this=(struct UART_COMMON*)calloc(1,sizeof(struct UART_COMMON));
		MX_USART1_UART_Init();
		this->uart_handle=huart1;
		this->UClearRec=uart_clear_rec;
		this->UPrintf=uart_printf;
	}
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
	if (huart->Instance == USART1) //如果是串口1
	{
		if (((uart1_common->rx_flag) & 0x80) == 0) //接收未完成
				{
			if ((uart1_common->rx_flag) & 0x40) //接收到了0x0d
					{
				if (aRxBuffer[0] != 0x0a)
				{
					uart_clear_rec(uart1_common); //接收错误,重新开始
				}
				else
					uart1_common->rx_flag |= 0x80;	//接收完成了
			} else //还没收到0X0D
			{
				if (aRxBuffer[0] == 0x0d)
					uart1_common->rx_flag |= 0x40;
				else {
					uart1_common->rx_buf[uart1_common->rx_len++] = aRxBuffer[0];
					if (uart1_common->rx_len > (MAX_RX_LENGTH - 1))
					{
						uart_clear_rec(uart1_common);	//接收错误,重新开始
					}
				}
			}
		}
	}
}

/* USER CODE END 1 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
