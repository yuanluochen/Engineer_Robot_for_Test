#include "can.h"


static uint16_t can1_init(uint32_t tsjw, uint32_t tsg1, uint32_t tsg2, uint32_t mode, uint32_t brp );
static uint16_t can2_init(uint32_t tsjw, uint32_t tsg1, uint32_t tsg2, uint32_t mode, uint32_t brp );


void can_init(void)
{
    can1_init(CAN1_TSJW, CAN1_TSG1, CAN1_TSG2, CAN1_MODE, CAN1_BRP);
    can2_init(CAN2_TSJW, CAN1_TSG1, CAN2_TSG2, CAN2_MODE, CAN2_BRP);
}


/**
 * @brief can初始化函数，以下参数皆为类型为CAN_InitTypeDef型的结构体变量的成员变量的数值，此结构体位于头文件 stm32f4xx_hal_can.h
 * 
 * @param tsjw 为成员变量SyncJumpWidth的数值
 * 
 * @param tsg1 为成员变量TimeSeg1的数值
 *  
 * @param tsg2 为成员变量TimeSeg2的数值
 * 
 * @param mode 为成员变量Mode的数值
 * 
 * @param brp  为成员变量Prescaler的数值
 * 
 * @retval init_success 初始化成功； init_fail 初始化失败
 * 
 */
static uint16_t can1_init(uint32_t tsjw, uint32_t tsg1, uint32_t tsg2, uint32_t mode, uint32_t brp )
{
    //初始化标志
    uint8_t init_flag = 0;

    GPIO_InitTypeDef GPIO_InitStructure;
    CAN_HandleTypeDef CAN_InitStructure;

    //rcc使能
    __HAL_RCC_GPIOD_CLK_ENABLE();
    __HAL_RCC_CAN1_CLK_SLEEP_ENABLE();
    //PD0 PD1 复用
    GPIO_InitStructure.Pin = GPIO_PIN_0 | GPIO_PIN_1;
    GPIO_InitStructure.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStructure.Pull = GPIO_PULLUP;
    GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStructure.Alternate = GPIO_AF9_CAN1; 
    HAL_GPIO_Init(GPIOD, &GPIO_InitStructure);

    //can初始化
    CAN_InitStructure.Instance = CAN1;
    CAN_InitStructure.Init.SyncJumpWidth = tsjw;
    CAN_InitStructure.Init.TimeSeg1 = tsg1;
    CAN_InitStructure.Init.TimeSeg2 = tsg2;
    CAN_InitStructure.Init.Mode = mode;
    CAN_InitStructure.Init.Prescaler = brp;
    CAN_InitStructure.Init.AutoBusOff = ENABLE;
    CAN_InitStructure.Init.AutoRetransmission = DISABLE;
    CAN_InitStructure.Init.AutoWakeUp = DISABLE;
    CAN_InitStructure.Init.ReceiveFifoLocked = DISABLE;
    CAN_InitStructure.Init.TimeTriggeredMode = DISABLE;
    CAN_InitStructure.Init.TransmitFifoPriority = DISABLE;
    while (HAL_CAN_Init(&CAN_InitStructure) == HAL_OK)
    {
        //初始化完成
        if (init_flag++ == 500)
        {
            //发生初始化失败信息
            Error_handle(CAN2_Error);
            //初始化失败
            return init_fail;
        }
    }
    //初始化成功
    return init_success;
}

/**
 * @brief can初始化函数，以下参数皆为类型为CAN_InitTypeDef型的结构体变量的成员变量的数值，此结构体位于头文件 stm32f4xx_hal_can.h
 * 
 * @param tsjw 为成员变量SyncJumpWidth的数值
 * 
 * @param tsg1 为成员变量TimeSeg1的数值
 *  
 * @param tsg2 为成员变量TimeSeg2的数值
 * 
 * @param mode 为成员变量Mode的数值
 * 
 * @param brp  为成员变量Prescaler的数值
 * 
 * @retval init_success 初始化成功； init_fail 初始化失败
 * 
 */
static uint16_t can2_init(uint32_t tsjw, uint32_t tsg1, uint32_t tsg2, uint32_t mode, uint32_t brp )
{
    //初始化标准
    int init_flag = 0;

    GPIO_InitTypeDef GPIO_Initstructure;
    CAN_HandleTypeDef CAN_InitStructure;
    //RCC使能
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_CAN2_CLK_ENABLE();
    //PB12 PB13配置
    GPIO_Initstructure.Pin = GPIO_PIN_12 | GPIO_PIN_13;
    GPIO_Initstructure.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_Initstructure.Mode = GPIO_MODE_AF_PP;
    GPIO_Initstructure.Pull = GPIO_PULLUP;
    GPIO_Initstructure.Alternate = GPIO_AF9_CAN2;
    HAL_GPIO_Init(GPIOB, &GPIO_Initstructure);

    //CAN2初始化
    CAN_InitStructure.Instance = CAN2;
    CAN_InitStructure.Init.SyncJumpWidth = tsjw;
    CAN_InitStructure.Init.TimeSeg1 = tsg1;
    CAN_InitStructure.Init.TimeSeg2 = tsg2;
    CAN_InitStructure.Init.Mode = mode;
    CAN_InitStructure.Init.Prescaler = brp;
    CAN_InitStructure.Init.AutoBusOff = ENABLE;
    CAN_InitStructure.Init.AutoRetransmission = DISABLE;
    CAN_InitStructure.Init.AutoWakeUp = DISABLE;
    CAN_InitStructure.Init.ReceiveFifoLocked = DISABLE;
    CAN_InitStructure.Init.TimeTriggeredMode = DISABLE;
    CAN_InitStructure.Init.TransmitFifoPriority = DISABLE;
    while(HAL_CAN_Init(&CAN_InitStructure) == HAL_OK)
    {
        //初始化完成
        if(init_flag++ == 500)
        {
            //发生初始化失败信息
            Error_handle(CAN1_Error);
            //初始化失败
            return init_fail;
        }
    }
    //初始化成功
    return init_success;
}
