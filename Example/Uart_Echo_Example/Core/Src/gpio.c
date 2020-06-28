/*
 * gpio.c
 *
 *  Created on: Jun 29, 2020
 *      Author: HYJ
 */

#include "gpio.h"

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
