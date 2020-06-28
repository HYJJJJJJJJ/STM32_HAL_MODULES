/*
 * gpio.h
 *
 *  Created on: Jun 29, 2020
 *      Author: HYJ
 */

#ifndef __gpio_H
#define __gpio_H
#ifdef __cplusplus
 extern "C" {
#endif

#include "main.h"

typedef struct GPIO_COMMON{
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_TypeDef  *GPIO_Port;
	void (*WritePin)(struct GPIO_COMMON *this,GPIO_PinState PinState);			//写入管脚电平
	void (*TogglePin)(struct GPIO_COMMON *this);											//翻转管脚电平
	GPIO_PinState (*ReadPin)(struct GPIO_COMMON *this);								//读取管脚电平
}GPIO_COMMON;
//GPIO类初始化
GPIO_COMMON *new_Gpio(struct GPIO_COMMON *this,GPIO_TypeDef  *port,uint32_t pin,uint32_t mode,uint32_t pull,uint32_t speed);

#ifdef __cplusplus
}
#endif
#endif

