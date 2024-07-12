#include "gpio.h"
#include "uart.h"
#include "stm32f10x.h"
#include "rtx_os.h"
#include "SysTick.h"
#include "tftlcd.h"
#include "picture.h"

static const char* PassWord = "41907";
static char input[100] = {};
static u32 input_cnt = 0;

//	判断输入和预设密码是否相等
static inline const u8 judge()
{
	if(input_cnt != 5)
		return 0;
	else
	{
		for(u32 i = 0; i < 5; i++)
			if(input[i] != PassWord[i])
				return 0;
		return 1;
	}
}

//	提示词指针
static char* Prompt_p;
																
typedef struct {
	int x;	// L-R
	int y;	// U-D
} point;

static point Pointer = {0, 0};

//	判断point是否相等
static inline const u8 point_is_equal(const point* const p1, const int x, const int y)
{
	if((p1->x == x) && (p1->y == y))
		return 1;
	else
		return 0;
}
	
static enum direct {
	UP,
	DOWN,
	LEFT,
	RIGHT
};

//	管理悬停指针位置
static void manage_Pointer(int dir)
{
	if(dir == UP)
	{
		Pointer.x = (Pointer.x + 4 - 1) % 4;
	}
	else if(dir == DOWN)
	{
		Pointer.x = (Pointer.x + 4 + 1) % 4;
	}
	else if(dir == LEFT)
	{
		Pointer.y = (Pointer.y + 3 - 1) % 3;
	}
	else if(dir == RIGHT)
	{
		Pointer.y = (Pointer.y + 3 + 1) % 3;
	}
}

//	检测长按某个按键
u8 detect_LongPressing(uint32_t Button)
{
	for(uint32_t i = 0; i < 10; i++)
	{
		if(KEY_Scan() != Button)
			return 0;
		osDelay(100);
	}
	return 1;
}

// 检测有无按键按下
static void detect_button(void *argument)
{
	while(1)
	{
		u8 button = KEY_Scan();
		if(button != 0 && detect_LongPressing(button))
		{
			if(point_is_equal(&Pointer, 0, 0))
			{
				input[input_cnt++] = '1';
			}
			else if(point_is_equal(&Pointer, 0, 1))
			{
				input[input_cnt++] = '2';
			}
			else if(point_is_equal(&Pointer, 0, 2))
			{
				input[input_cnt++] = '3';
			}
			else if(point_is_equal(&Pointer, 1, 0))
			{
				input[input_cnt++] = '4';
			}
			else if(point_is_equal(&Pointer, 1, 1))
			{
				input[input_cnt++] = '5';
			}
			else if(point_is_equal(&Pointer, 1, 2))
			{
				input[input_cnt++] = '6';
			}
			else if(point_is_equal(&Pointer, 2, 0))
			{
				input[input_cnt++] = '7';
			}
			else if(point_is_equal(&Pointer, 2, 1))
			{
				input[input_cnt++] = '8';
			}
			else if(point_is_equal(&Pointer, 2, 2))
			{
				input[input_cnt++] = '9';
			}
			else if(point_is_equal(&Pointer, 3, 0))
			{
				// 确认
				if(judge())
				{
					Prompt_p = "密码正确";
					LCD_ShowHZString(20, 70, Prompt_p);
					LCD_ShowString(20 + 32 * 4, 70, 320 - (20 + 32 * 4), 32, 32, "          ");
				}
				else
				{
					Prompt_p = "输入错误请重新输入";
					LCD_ShowHZString(20, 70, Prompt_p);
				}
				
				input_cnt = 0;
			}
			else if(point_is_equal(&Pointer, 3, 1))
			{
				input[input_cnt++] = '0';
			}
			else if(point_is_equal(&Pointer, 3, 2))
			{
				input[input_cnt--] = '\0';
			}
			while(KEY_Scan() == button)
			{
				osDelay(100);
			}
		}
		else
		{
			if(button == KEY_UP_PRESS)
			{
				manage_Pointer(UP);
			}
			else if(button == KEY0_PRESS)
			{
				manage_Pointer(RIGHT);
			}
			else if(button == KEY1_PRESS)
			{
				manage_Pointer(DOWN);
			}
			else if(button == KEY2_PRESS)
			{
				manage_Pointer(LEFT);
			}
			
			if(button	== 0)
				osDelay(10);
			else
				osDelay(100);
			}
	}
}

//	刷新
static void refresh_lcd(void *argument)
{
	while(1)
	{
		//	密码星号
		u8 stars[10] = {};
		for(u32 i = 0; i < input_cnt; i++)
			stars[i] = '*';
		for(u32 i = input_cnt; i < 5; i++)
			stars[i] = ' ';
		stars[5] = '\0';

		LCD_ShowString(30, 30, 210, 30, 24, stars);
			
		//	键盘
		for(int i = 0;i < 4;i++)
		{
			for(int j = 0;j < 3;j++)
			{
				if(point_is_equal(&Pointer, i, j))
				{
					LCD_ShowPicture(20 + 60 * j, 140 + 60 * i, 60, 60, gImage[i * 3 + j][1]);
				}
				else
				{
					LCD_ShowPicture(20 + 60 * j, 140 + 60 * i, 60, 60, gImage[i * 3 + j][0]);
				}
			}
		}
		osDelay(10);
	}
}


static const osThreadAttr_t thread_Start_Attr = 
{
	.name = "Start_App",
	.attr_bits = osThreadDetached,
	.priority = osPriorityHigh4,
	.stack_size = 2048,
};

static const osThreadAttr_t thread_Button_Attr = 
{
	.name = "Button_App",
	.attr_bits = osThreadDetached,
	.priority = osPriorityHigh3,
	.stack_size = 1024,
};

static const osThreadAttr_t thread_Lcd_Attr = 
{
	.name = "Lcd_App",
	.attr_bits = osThreadDetached,
	.priority = osPriorityHigh2,
	.stack_size = 2048,
};

osThreadId_t threadID_Button_Thread = NULL;
osThreadId_t threadID_Refresh_Thread = NULL;




static void app_main(void *argument)
{
	Prompt_p = "请输入密码";
	LCD_Fill(20, 20, 240, 60, WHITE);
	LCD_DrawRectangle(20, 20, 240, 60);
	LCD_ShowHZString(20, 70, Prompt_p);
	
	threadID_Button_Thread = osThreadNew(detect_button, NULL, &thread_Button_Attr); 
	threadID_Refresh_Thread = osThreadNew(refresh_lcd, NULL, &thread_Lcd_Attr); 
}




int main()
{
	PinConfig();
	
	USART1_Init(115200U);
	
	TFTLCD_Init();
	
	SystemCoreClockUpdate(); 
 
	osKernelInitialize(); 
	osThreadNew(app_main, NULL, &thread_Start_Attr); 
	osKernelStart(); 
 
	while(1); 
	return 0;
}
