#include "can.h"

//can1初始化结构体定义为全局变量为发送函数准备
static CAN_HandleTypeDef CAN1_InitStructure;

//can发送帧配置
#define CAN_TX_DLC 0x08
#define CAN_TX_RTR CAN_RTR_DATA
#define CAN_TX_IDE CAN_ID_STD

//can接收配置
#define CAN_RX_DLC 0x08

/**
 * @brief 电盘电机数据获取，将底盘电机数据存储到全局变量以便访问 
 * 
 * @param ptr 存储从底盘电机接收而来的数据的结构体指针变量
 * 
 * @param RxMessage 存储底盘电机接收数据的数组
 */
#define get_motor_measure(ptr, RxMessage)                                                     \
    {                                                                                         \
        (ptr)->motor_last_mechine_angle = (ptr)->motor_mechine_angle;                         \
        (ptr)->motor_mechine_angle = (uint16_t)RxMessage[0] << 8 | (uint16_t)RxMessage[1];    \
        (ptr)->motor_speed =  (uint16_t)RxMessage[2] << 8 | (uint16_t)RxMessage[3];           \
        (ptr)->motor_current = (uint16_t)RxMessage[4] << 8 | (uint16_t)RxMessage[5];          \
        (ptr)->motor_temprerate = (uint16_t)RxMessage[6];                                     \
    }

//can1 接收数据数组
static uint8_t RxDate[8] = { 0 };

//存储接收底盘数据的结构体数组
static motor_measure_t Chassis_Motor_measure[4];

static uint16_t can1_init(uint32_t tsjw, uint32_t tsg1, uint32_t tsg2, uint32_t mode, uint32_t brp );
#if CAN2_INIT
static uint16_t can2_init(uint32_t tsjw, uint32_t tsg1, uint32_t tsg2, uint32_t mode, uint32_t brp ); #endif
#endif

void can_init(void)
{
    can1_init(CAN1_TSJW, CAN1_TSG1, CAN1_TSG2, CAN1_MODE, CAN1_BRP);

    #if CAN2_INIT
    can2_init(CAN2_TSJW, CAN1_TSG1, CAN2_TSG2, CAN2_MODE, CAN2_BRP);
    #endif
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
    CAN_FilterTypeDef CAN1_FilterInit;

    // rcc使能
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
    CAN1_InitStructure.Instance = CAN1;
    CAN1_InitStructure.Init.SyncJumpWidth = tsjw;
    CAN1_InitStructure.Init.TimeSeg1 = tsg1;
    CAN1_InitStructure.Init.TimeSeg2 = tsg2;
    CAN1_InitStructure.Init.Mode = mode;
    CAN1_InitStructure.Init.Prescaler = brp;
    CAN1_InitStructure.Init.AutoBusOff = ENABLE;
    CAN1_InitStructure.Init.AutoRetransmission = DISABLE;
    CAN1_InitStructure.Init.AutoWakeUp = DISABLE;
    CAN1_InitStructure.Init.ReceiveFifoLocked = DISABLE;
    CAN1_InitStructure.Init.TimeTriggeredMode = DISABLE;
    CAN1_InitStructure.Init.TransmitFifoPriority = DISABLE;
    while (HAL_CAN_Init(&CAN1_InitStructure) == HAL_OK)
    {
        //初始化完成
        if (init_flag++ == 500)
        {
            //发生初始化失败信息
            Error_handle(CAN1_Error);
            //初始化失败
            return init_fail;
        }
    }
    
    //滤波器初始化

    CAN1_FilterInit.FilterIdHigh = 0x0000;//32位ID
    CAN1_FilterInit.FilterIdLow = 0x0000;
    CAN1_FilterInit.FilterMaskIdHigh = 0x0000;//32位MASK
    CAN1_FilterInit.FilterMaskIdLow = 0x0000;
    CAN1_FilterInit.FilterFIFOAssignment = CAN_FilterFIFO0;
    CAN1_FilterInit.FilterMode = CAN_FILTERMODE_IDMASK;
    CAN1_FilterInit.FilterScale = CAN_FILTERSCALE_32BIT;
    CAN1_FilterInit.FilterActivation = ENABLE;
    CAN1_FilterInit.FilterBank = 14;
    HAL_CAN_ConfigFilter(&CAN1_InitStructure, &CAN1_FilterInit);

    //开启can
    HAL_CAN_Start(&CAN1_InitStructure);

    //使能can接收中断
    HAL_CAN_ActivateNotification(&CAN1_InitStructure, CAN_IT_RX_FIFO0_MSG_PENDING);

    //初始化成功
    return init_success;
}

#if CAN2_INIT
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
    CAN_HandleTypeDef CAN2_InitStructure;
    CAN_FilterTypeDef CAN2_FilterInit;

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
    CAN2_InitStructure.Instance = CAN2;
    CAN2_InitStructure.Init.SyncJumpWidth = tsjw;
    CAN2_InitStructure.Init.TimeSeg1 = tsg1;
    CAN2_InitStructure.Init.TimeSeg2 = tsg2;
    CAN2_InitStructure.Init.Mode = mode;
    CAN2_InitStructure.Init.Prescaler = brp;
    CAN2_InitStructure.Init.AutoBusOff = ENABLE;
    CAN2_InitStructure.Init.AutoRetransmission = DISABLE;
    CAN2_InitStructure.Init.AutoWakeUp = DISABLE;
    CAN2_InitStructure.Init.ReceiveFifoLocked = DISABLE;
    CAN2_InitStructure.Init.TimeTriggeredMode = DISABLE;
    CAN2_InitStructure.Init.TransmitFifoPriority = DISABLE;

    CAN2_FilterInit.FilterIdHigh = 0x0000; // 32位ID
    CAN2_FilterInit.FilterIdLow = 0x0000;
    CAN2_FilterInit.FilterMaskIdHigh = 0x0000;//32位MASK
    CAN2_FilterInit.FilterMaskIdLow = 0x0000;
    CAN2_FilterInit.FilterFIFOAssignment = CAN_FilterFIFO0;
    CAN2_FilterInit.FilterMode = CAN_FILTERMODE_IDMASK;
    CAN2_FilterInit.FilterScale = CAN_FILTERSCALE_32BIT;
    CAN2_FilterInit.FilterActivation = ENABLE;
    CAN2_FilterInit.FilterBank = 14;
    HAL_CAN_ConfigFilter(&CAN2_InitStructure, &CAN2_FilterInit);


    //开启can
    HAL_CAN_Start(&CAN2_InitStructure);

    while(HAL_CAN_Init(&CAN2_InitStructure) == HAL_OK)
    {
        //初始化完成
        if(init_flag++ == 500)
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
#endif

uint16_t can1_SendMessage(uint16_t Motor_1, uint16_t Motor_2, uint16_t Motor_3, uint16_t Motor_4)
{
    //can 发送数据
    uint8_t TxDate[8] = {0};
    //can发送次数
    int TxCount = 0;
    //发送数据帧配置结构体
    CAN_TxHeaderTypeDef CAN_TxStructure;

    CAN_TxStructure.DLC =   CAN_TX_DLC;
    CAN_TxStructure.StdId = CAN_Tx_ALL_ID;
    CAN_TxStructure.RTR = CAN_TX_RTR;
    CAN_TxStructure.IDE = CAN_TX_IDE;

    //数据帧配置
    TxDate[0] = (Motor_1 >> 8);
    TxDate[1] = Motor_1;
    TxDate[2] = (Motor_2 >> 8);
    TxDate[3] = Motor_2;
    TxDate[4] = (Motor_3 >> 8);
    TxDate[5] = Motor_3;
    TxDate[6] = (Motor_4 >> 8);
    TxDate[7] = Motor_4;

    //发送邮箱CAN_TX_MAILBOX1
    while(HAL_CAN_AddTxMessage(&CAN1_InitStructure, &CAN_TxStructure, TxDate, (uint32_t*) CAN_TX_MAILBOX1) != HAL_OK)
    {
        if(TxCount++ == 1000)
        {
            Error_handle(CAN1_Tx_Error);
            return 1;//发送失败
        }
    }
    return 0;//发送成功

}

/**
 * @brief CAN接收接收数据处理函数,将所收到的不同ID的can数据帧,进行分类，分别接收不同的数据
 * 
 * @param CAN_RxStr CAN接收数据结构体
 * 
 * @retval none
 */
static void CAN_RxMsgHandle(CAN_RxHeaderTypeDef* CAN_RxStr)
{
    //判断传入指针是否为空
    if(CAN_RxStr == NULL)
    {
        //为空返回
        return;
    }
    //筛选数据
    switch(CAN_RxStr->StdId)
    {
        //电盘电机接收数据
    case CAN_3508_M1_ID:
    case CAN_3508_M2_ID:
    case CAN_3508_M3_ID:
    case CAN_3508_M4_ID:
    {
        #if 0
        uint8_t i = 0;
        //将电机编号转换为底盘电机结构体数组元素编号
        i = CAN_RxStr->StdId - CAN_3508_M1_ID; 
        //将发送的can数据帧存储到对应存储电机数据结构体中
        get_motor_measure(&Chassis_Motor_measure[i], RxDate);
        #else 
        get_motor_measure(&Chassis_Motor_measure[CAN_RxStr->StdId - CAN_3508_M1_ID], RxDate);
        #endif
    }
    break;
    }
}


uint16_t Get_motor_mechine_angle(uint16_t CAN_Rx_ID)
{
    return Chassis_Motor_measure[CAN_Rx_ID - CAN_3508_M1_ID].motor_mechine_angle;
}
uint16_t Get_motor_speed(uint16_t CAN_Rx_ID)
{
    return Chassis_Motor_measure[CAN_Rx_ID - CAN_3508_M1_ID].motor_speed;
}
uint16_t Get_motor_current(uint16_t CAN_Rx_ID)
{
    return Chassis_Motor_measure[CAN_Rx_ID - CAN_3508_M1_ID].motor_current;
}
uint16_t Get_motor_temprerate(uint16_t CAN_Rx_ID)
{
    return Chassis_Motor_measure[CAN_Rx_ID - CAN_3508_M1_ID].motor_temprerate;
}

void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
    CAN_RxHeaderTypeDef CAN_RxStructure;
    CAN_RxStructure.DLC = CAN_RX_DLC;
    //接收底盘数据
    HAL_CAN_GetRxMessage(&CAN1_InitStructure, CAN_IT_RX_FIFO0_MSG_PENDING, &CAN_RxStructure, RxDate);
    //处理接收到底盘电机数据
    CAN_RxMsgHandle(&CAN_RxStructure);
}


