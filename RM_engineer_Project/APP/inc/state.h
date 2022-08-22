#ifndef __ERROR_H_
#define __ERROR_H_

#include "stm32f4xx_hal.h"
#include "LED.h"

//错误类型
enum Error
{
    CAN1_Error,//CAN1初始化失败，LED1闪烁5次，延时500ms
    CAN2_Error,//led2闪烁5次，每次之间延时500ms
    CAN1_Tx_Error,//LED3闪烁5次延时500ms
};


//初始化状态
enum init_state
{
    init_success,
    init_fail,
};

/**
 * @brief 该函数为错误处理函数，详细反馈在枚举Error成员注释
 * 
 * @param Error_Type 参数为类型为Error的枚举 
 */
void Error_handle(uint16_t Error_Type);

#endif
