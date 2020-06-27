/**
  ******************************************************************************
  * File Name          : gpio.h
  * Description        : This file contains all the functions prototypes for 
  *                      the gpio  
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
#ifndef __gpio_H
#define __gpio_H
#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

void MX_GPIO_Init(void);

/* USER CODE BEGIN Prototypes */

typedef struct GPIO_COMMON{
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_TypeDef  *GPIO_Port;
	void (*WritePin)(struct GPIO_COMMON *this,GPIO_PinState PinState);			//写入管脚电平
	void (*TogglePin)(struct GPIO_COMMON *this);											//翻转管脚电平
	GPIO_PinState (*ReadPin)(struct GPIO_COMMON *this);								//读取管脚电平
}GPIO_COMMON;
//GPIO类初始化
GPIO_COMMON *new_Gpio(struct GPIO_COMMON *this,GPIO_TypeDef  *port,uint32_t pin,uint32_t mode,uint32_t pull,uint32_t speed);
/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif
#endif /*__ pinoutConfig_H */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
