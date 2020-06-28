/*
 * usart.c
 *
 *  Created on: Jun 27, 2020
 *      Author: HYJ
 */

#include "usart.h"

UART_HandleTypeDef huart1;
u8 aRxBuffer[1]; //HAL库使用的串口接收缓冲
struct UART_COMMON *uart1_common;

void HAL_UART_MspInit(UART_HandleTypeDef *uartHandle) {
	GPIO_COMMON *u1_txd = NULL, *u1_rxd = NULL;

	if (uartHandle->Instance == USART1) {
		/* USART1 clock enable */
		__HAL_RCC_USART1_CLK_ENABLE();

		//    /**USART1 GPIO Configuration
		//    PA9     ------> USART1_TX
		//    PA10     ------> USART1_RX
		//    */
		u1_txd = new_Gpio(u1_txd, GPIOA, GPIO_PIN_9, GPIO_MODE_AF_PP,
		GPIO_NOPULL, GPIO_SPEED_FREQ_HIGH);
		u1_rxd = new_Gpio(u1_rxd, GPIOA, GPIO_PIN_10, GPIO_MODE_INPUT,
		GPIO_NOPULL, GPIO_SPEED_FREQ_HIGH);

		HAL_NVIC_SetPriority(USART1_IRQn, 0, 0);
		HAL_NVIC_EnableIRQ(USART1_IRQn);
	}
}

void HAL_UART_MspDeInit(UART_HandleTypeDef *uartHandle) {

	if (uartHandle->Instance == USART1) {
		__HAL_RCC_USART1_CLK_DISABLE();
		HAL_GPIO_DeInit(GPIOA, GPIO_PIN_9 | GPIO_PIN_10);
		HAL_NVIC_DisableIRQ(USART1_IRQn);
	}
}

void USART1_IRQHandler(void) {
	u32 timeout = 0;
	u32 maxDelay = 0x1FFFF;

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

void uart_clear_rec(struct UART_COMMON *this) {
	this->rx_flag = 0;
	this->rx_len = 0;
}

void uart_printf(struct UART_COMMON *this, char *fmt, ...) {	//通过任意串口发送字符串
	u16 i;
	va_list ap;
	va_start(ap, fmt);
	vsprintf((char*) this->tx_buf, fmt, ap);
	va_end(ap);
	i = strlen((const char*) this->tx_buf);		//此次发送数据的长度
	HAL_UART_Transmit(&this->uart_handle, (uint8_t*) this->tx_buf, i, 0xffff);
}

UART_COMMON* new_Uart(struct UART_COMMON *this, USART_TypeDef *port, u32 baud)	//串口的初始化与相关函数注册
{
	this = (struct UART_COMMON*) calloc(1, sizeof(struct UART_COMMON));
	this->uart_handle.Instance = port;
	this->uart_handle.Init.BaudRate = baud;
	this->uart_handle.Init.WordLength = UART_WORDLENGTH_8B;
	this->uart_handle.Init.StopBits = UART_STOPBITS_1;
	this->uart_handle.Init.Parity = UART_PARITY_NONE;
	this->uart_handle.Init.Mode = UART_MODE_TX_RX;
	this->uart_handle.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	this->uart_handle.Init.OverSampling = UART_OVERSAMPLING_16;
	if (HAL_UART_Init(&this->uart_handle) != HAL_OK) {
		Error_Handler();
	}
	HAL_UART_Receive_IT(&this->uart_handle, (u8*) aRxBuffer, 1);
	huart1=this->uart_handle;
	this->UClearRec = uart_clear_rec;
	this->UPrintf = uart_printf;
	return this;
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
	if (huart->Instance == USART1) //如果是串口1
	{
		if (((uart1_common->rx_flag) & 0x80) == 0) //接收未完成
				{
			if ((uart1_common->rx_flag) & 0x40) //接收到了0x0d
					{
				if (aRxBuffer[0] != 0x0a) {
					uart_clear_rec(uart1_common); //接收错误,重新开始
				} else
					uart1_common->rx_flag |= 0x80;	//接收完成了
			} else //还没收到0X0D
			{
				if (aRxBuffer[0] == 0x0d)
					uart1_common->rx_flag |= 0x40;
				else {
					uart1_common->rx_buf[uart1_common->rx_len++] = aRxBuffer[0];
					if (uart1_common->rx_len > (MAX_RX_LENGTH - 1)) {
						uart_clear_rec(uart1_common);	//接收错误,重新开始
					}
				}
			}
		}
	}
}
