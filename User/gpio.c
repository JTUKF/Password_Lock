
#include "gpio.h"

void PinConfig(void) 
{ 
		GPIO_InitTypeDef GPIO_InitStructure; 
	 
	 
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOE|RCC_APB2Periph_AFIO,ENABLE); 
	 
		GPIO_InitStructure.GPIO_Pin=GPIO_Pin_5; //LED 
		GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz; 
		GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP; 
		GPIO_Init(GPIOB,&GPIO_InitStructure); 
	
		GPIO_InitStructure.GPIO_Pin=GPIO_Pin_5; //LED 
		GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz; 
		GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP; 
		GPIO_Init(GPIOE,&GPIO_InitStructure); 
	
		GPIO_InitStructure.GPIO_Pin=KEY_UP_PIN;	   
		GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPD;//下拉输入  
		GPIO_Init(KEY_UP_PORT,&GPIO_InitStructure);		  
		
		GPIO_InitStructure.GPIO_Pin=KEY0_PIN|KEY1_PIN|KEY2_PIN;
		GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;	//上拉输入
		GPIO_Init(KEY_PORT,&GPIO_InitStructure);
	 
} 

u8 KEY_Scan()
{
	u8 KEY_UP = GPIO_ReadInputDataBit(KEY_UP_PORT, KEY_UP_PIN);
	u8 KEY0 = GPIO_ReadInputDataBit(KEY_PORT, KEY0_PIN);
	u8 KEY1 = GPIO_ReadInputDataBit(KEY_PORT, KEY1_PIN);
	u8 KEY2 = GPIO_ReadInputDataBit(KEY_PORT, KEY2_PIN);

	if(KEY_UP==1||KEY0==0||KEY1==0||KEY2==0) //任意一个按键按下
	{
		if(KEY_UP==1)
			return KEY_UP_PRESS; 
		else if(KEY0==0)
			return KEY0_PRESS; 
		else if(KEY1==0)
			return KEY1_PRESS; 
		else if(KEY2==0)
			return KEY2_PRESS; 
	}
	
	return 0;
}