#ifndef __CAN_H_
#define __CAN_H_

#include "stm32f427xx.h"
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_can.h"
#include "LED.h"
#include "state.h"

//can1初始化配置参数，具体查找can1_init函数注释
#define CAN1_TSJW CAN_SJW_1TQ
#define CAN1_TSG1 CAN_BS1_3TQ
#define CAN1_TSG2 CAN_BS2_5TQ
#define CAN1_MODE CAN_MODE_NORMAL
#define CAN1_BRP 5
//can2初始化配置参数，具体查找can2_init函数注释
#define CAN2_TSJW CAN_SJW_4TQ
#define CAN2_TSG1 CAN_BS1_3TQ
#define CAN2_TSG2 CAN_BS2_5TQ
#define CAN2_MODE CAN_MODE_NORMAL
#define CAN2_BRP 5



/**
 * @brief can初始化函数，初始化can1，can2
 * 
 * @retval none
 * 
 */
void can_init(void);



#endif
