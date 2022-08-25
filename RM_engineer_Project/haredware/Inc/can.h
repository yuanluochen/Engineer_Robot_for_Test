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

//电机数据帧的ID
enum CAN_Motor_ID
{
    //向所有电机发送数据的ID
    CAN_Tx_ALL_ID  = 0x200,
    //电盘四个电机接收数据时CAN的ID
    CAN_3508_M1_ID = 0x201,
    CAN_3508_M2_ID = 0x202,
    CAN_3508_M3_ID = 0x203,
    CAN_3508_M4_ID = 0x204
};

//接收can数据结构体
typedef struct 
{
    uint16_t motor_mechine_angle;     //最新转子机械角度
    uint16_t motor_last_mechine_angle;//上一次转子机械角度 
    uint16_t motor_speed;             //转子转速
    uint16_t motor_current;           //实际转矩电流
    uint16_t motor_temprerate;        //电机温度
}motor_measure_t;

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
 * @brief 发送底盘电机电流数据
 * 
 * @param Motor_1 向底盘电机1发送的电流大小数据
 * @param Motor_2 向底盘电机2发送的电流大小数据
 * @param Motor_3 向底盘电机3发送的电流大小数据
 * @param Motor_4 向底盘电机4发送的电流大小数据
 * @return 返回值为1发送失败，返回值为0发送成功
 */
uint16_t can1_SendMessage(uint16_t Motor_1, uint16_t Motor_2, uint16_t Motor_3, uint16_t Motor_4);

/**
 * @brief 获取底盘电机转子机械角度
 * 
 * @param CAN_Rx_ID 电机ID
 * @return 此时底盘电机转子机械角度数值
 */
uint16_t Get_motor_mechine_angle(uint16_t CAN_Rx_ID);
/**
 * @brief 获取底盘电机转子转速 
 * 
 * @param CAN_Rx_ID 电机ID 
 * @return 此时底盘电机转子转速 
 */
uint16_t Get_motor_speed(uint16_t CAN_Rx_ID);
/**
 * @brief 获取底盘电机转子转速 
 * 
 * @param CAN_Rx_ID 电机ID
 * @return 此时电盘电流大小 
 */
uint16_t Get_motor_current(uint16_t CAN_Rx_ID);
/**
 * @brief 获取底盘电机温度
 * 
 * @param CAN_Rx_ID 电机ID
 * @return 此时电机温度
 */
uint16_t Get_motor_temprerate(uint16_t CAN_Rx_ID);



#endif
