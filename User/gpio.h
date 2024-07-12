#ifndef __GPIO_H_
#define __GPIO_H_


#include"stm32f10x_gpio.h" 
#include"stm32f10x_rcc.h" 
#include "stm32f10x_usart.h" 

#define KEY0_PIN   			GPIO_Pin_4    	//定义KEY0管脚
#define KEY1_PIN    		GPIO_Pin_3    	//定义KEY1管脚
#define KEY2_PIN    		GPIO_Pin_2    	//定义KEY2管脚
#define KEY_UP_PIN  		GPIO_Pin_0  	//定义KEY_UP管脚

#define KEY_PORT 			GPIOE 		//定义端口
#define KEY_UP_PORT 		GPIOA 		//定义端口

//定义各个按键值  
#define KEY_UP_PRESS 	1
#define KEY0_PRESS		2
#define KEY1_PRESS		3
#define KEY2_PRESS		4

void PinConfig(void) ;
u8 KEY_Scan();

#endif