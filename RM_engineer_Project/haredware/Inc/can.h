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

/**
 * @brief can2初始化控制，由于现阶段can2配置不明，暂不进行配置can2 
 * 
 */
#define CAN2_INIT 0

#if CAN2_INIT
//can2初始化配置参数，具体查找can2_init函数注释
#define CAN2_TSJW CAN_SJW_4TQ
#define CAN2_TSG1 CAN_BS1_3TQ
#define CAN2_TSG2 CAN_BS2_5TQ
#define CAN2_MODE CAN_MODE_NORMAL
#define CAN2_BRP 5
#endif



/**
 * @brief can初始化函数，初始化can1，can2
 * 
 * @retval none
 * 
 */
void can_init(void);

/**
 * @brief can1发送函数 
 * 
 * @param i1 发送给电调1电流大小 
 * @param i2 发送给电调2电流大小 
 * @param i3 发送给电调3电流大小
 * @param i4 发送给电调4的电流大小
 * @return 返回值为"1"发送失败，返回值为"0"发送成功。 
 */
uint16_t can1_SendMessage(uint16_t i1, uint16_t i2, uint16_t i3, uint16_t i4);



#endif
