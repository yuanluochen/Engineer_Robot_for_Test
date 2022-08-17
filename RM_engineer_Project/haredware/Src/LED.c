#include "LED.h"


//LED控制结构体
LED_handle LED1_BASE = { GPIOG, GPIO_PIN_1};
LED_handle LED2_BASE = { GPIOG, GPIO_PIN_2};
LED_handle LED3_BASE = { GPIOG, GPIO_PIN_3};
LED_handle LED4_BASE = { GPIOG, GPIO_PIN_4};
LED_handle LED5_BASE = { GPIOG, GPIO_PIN_5};
LED_handle LED6_BASE = { GPIOG, GPIO_PIN_6};
LED_handle LED7_BASE = { GPIOG, GPIO_PIN_7};
LED_handle LED8_BASE = { GPIOG, GPIO_PIN_8};

//LED初始化检测函数声明
static void LED_Init_check(void);


void LED_init(void)
{
    //led初始化结构体
    GPIO_InitTypeDef GPIO_InitStructure = { 0 };
    //enable rcc
    __HAL_RCC_GPIOG_CLK_ENABLE();
    GPIO_InitStructure.Pin = GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4 |
                             GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7 | GPIO_PIN_8;
    GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;//上拉
    GPIO_InitStructure.Pull = GPIO_PULLUP;
    GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    HAL_GPIO_Init(GPIOG, &GPIO_InitStructure);

    //默认关闭
    HAL_GPIO_WritePin(GPIOG, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4 |
                             GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7 | GPIO_PIN_8, GPIO_PIN_SET);

#if LED_INIT_CHECK
    //led灯依次亮起关闭
    LED_Init_check();
#endif
}


static void LED_Init_check(void)
{
    LED_flash(LED1, 500);
    LED_flash(LED2, 500);
    LED_flash(LED3, 500);
    LED_flash(LED4, 500);
    LED_flash(LED5, 500);
    LED_flash(LED6, 500);
    LED_flash(LED7, 500);
    LED_flash(LED8, 500);
}


void LED_Control(const LED_handle* LEDx, LED_Status LEDstatus)
{
    //check parameter LEDX
    if( LEDx == &LED1_BASE || LEDx == &LED2_BASE || LEDx == &LED3_BASE || LEDx == &LED4_BASE ||
        LEDx == &LED5_BASE || LEDx == &LED6_BASE || LEDx == &LED7_BASE || LEDx == &LED8_BASE )
    {
        //check parameter LESstatus
        if(LEDstatus == LED_OPEN || LEDstatus == LED_CLOSE)
        {
            //parameter ture0
            HAL_GPIO_WritePin(LEDx->GPIOx, LEDx->GPIO_PIN_x, LEDstatus);
        }
        else
        {
            //parameter LEDstatus fail
            return;
        }
    }
    else
    {
        //parameter LEDx fail
        return;
    }
    
}


void LED_flash(const LED_handle* LEDx, uint32_t Delay)
{
    //check parameter LEDx
    if( LEDx == &LED1_BASE || LEDx == &LED2_BASE || LEDx == &LED3_BASE || LEDx == &LED4_BASE ||
        LEDx == &LED5_BASE || LEDx == &LED6_BASE || LEDx == &LED7_BASE || LEDx == &LED8_BASE )
    {
        LED_Control(LEDx, LED_OPEN);
        HAL_Delay(Delay);
        LED_Control(LEDx, LED_CLOSE);
    }
    else
    {
        return;
    }
}
