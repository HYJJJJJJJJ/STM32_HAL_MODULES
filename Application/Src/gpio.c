/**
  ******************************************************************************
  * File Name          : gpio.c
  * Description        : This file provides code for the configuration
  *                      of all used GPIO pins.
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
#include "gpio.h"
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/*----------------------------------------------------------------------------*/
/* Configure GPIO                                                             */
/*----------------------------------------------------------------------------*/
/* USER CODE BEGIN 1 */

/* USER CODE END 1 */

/** Configure pins as 
        * Analog 
        * Input 
        * Output
        * EVENT_OUT
        * EXTI
*/
void MX_GPIO_Init(void)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET);

  /*Configure GPIO pin : PB0 */
  GPIO_InitStruct.Pin = GPIO_PIN_0;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

}

/* USER CODE BEGIN 2 */

void GPIO_WritePin(GPIO_COMMON *this,GPIO_PinState PinState)
{
	HAL_GPIO_WritePin(this->GPIO_Port, this->GPIO_InitStruct.Pin, PinState);
}

void GPIO_TogglePin(GPIO_COMMON *this)
{
	HAL_GPIO_TogglePin(this->GPIO_Port, this->GPIO_InitStruct.Pin);
}

GPIO_PinState GPIO_ReadPin(GPIO_COMMON *this)
{
	return HAL_GPIO_ReadPin(this->GPIO_Port, this->GPIO_InitStruct.Pin);
}

GPIO_COMMON *new_Gpio(struct GPIO_COMMON *this,GPIO_TypeDef *port,uint32_t pin,uint32_t mode,uint32_t pull,uint32_t speed)
{
	this=(struct GPIO_COMMON*)calloc(1,sizeof(struct GPIO_COMMON));
	this->GPIO_Port=port;
	if(this->GPIO_Port==GPIOA)
		__HAL_RCC_GPIOA_CLK_ENABLE();
	else if(this->GPIO_Port==GPIOB)
		__HAL_RCC_GPIOB_CLK_ENABLE();
	else if(this->GPIO_Port==GPIOC)
		__HAL_RCC_GPIOC_CLK_ENABLE();
	else if(this->GPIO_Port==GPIOD)
		__HAL_RCC_GPIOD_CLK_ENABLE();
	else if(this->GPIO_Port==GPIOE)
		__HAL_RCC_GPIOE_CLK_ENABLE();
#ifdef STM32F103xE
	else if(this->GPIO_Port==GPIOF)
		__HAL_RCC_GPIOF_CLK_ENABLE();
	else if(this->GPIO_Port==GPIOG)
		__HAL_RCC_GPIOG_CLK_ENABLE();
	else if(this->GPIO_Port==GPIOH)
		__HAL_RCC_GPIOH_CLK_ENABLE();
#endif
	this->GPIO_InitStruct.Pin=pin;
	this->GPIO_InitStruct.Mode = mode;
	this->GPIO_InitStruct.Pull = pull;
	this->GPIO_InitStruct.Speed = speed;
	HAL_GPIO_Init(this->GPIO_Port, &this->GPIO_InitStruct);
	this->ReadPin=GPIO_ReadPin;
	this->WritePin=GPIO_WritePin;
	this->TogglePin=GPIO_TogglePin;
	return this;
}
/* USER CODE END 2 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
