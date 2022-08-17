#include "state.h"

//错误显示 500ms 延时闪烁
#define ERROR_LED_FLASH_DELAY 500
//错误显示 闪烁5次
#define ERROR_LED_FLASH_TIME 5

//函数声明
static void CAN1_Error_Handle(void);
static void CAN2_Error_Handle(void);

void Error_handle(uint16_t Error_Type)
{
    switch(Error_Type)
    {
    case CAN1_Error:
        CAN1_Error_Handle();
        break;
    case CAN2_Error:
        CAN2_Error_Handle();
        break;
    }
}


static void CAN1_Error_Handle(void)
{
    //led1闪烁5次警示
    for(int i = 0; i < ERROR_LED_FLASH_TIME; i++)
        LED_flash(LED1, ERROR_LED_FLASH_DELAY);
}

static void CAN2_Error_Handle(void)
{
    //led2闪烁5次警示
    for(int i = 0; i < ERROR_LED_FLASH_TIME; i++)
        LED_flash(LED2, ERROR_LED_FLASH_DELAY);
}
