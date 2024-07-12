#ifndef __GPIO_H_
#define __GPIO_H_


#include"stm32f10x_gpio.h" 
#include"stm32f10x_rcc.h" 
#include "stm32f10x_usart.h" 

#define KEY0_PIN   			GPIO_Pin_4    	//����KEY0�ܽ�
#define KEY1_PIN    		GPIO_Pin_3    	//����KEY1�ܽ�
#define KEY2_PIN    		GPIO_Pin_2    	//����KEY2�ܽ�
#define KEY_UP_PIN  		GPIO_Pin_0  	//����KEY_UP�ܽ�

#define KEY_PORT 			GPIOE 		//����˿�
#define KEY_UP_PORT 		GPIOA 		//����˿�

//�����������ֵ  
#define KEY_UP_PRESS 	1
#define KEY0_PRESS		2
#define KEY1_PRESS		3
#define KEY2_PRESS		4

void PinConfig(void) ;
u8 KEY_Scan();

#endif