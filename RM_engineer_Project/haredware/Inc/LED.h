#ifndef __LED_H_
#define __LED_H_

#include "stm32f427xx.h"
#include "stm32f4xx_hal.h"

//LED控制结构体
typedef struct LED_handle
{
    GPIO_TypeDef* GPIOx;
    uint16_t GPIO_PIN_x;
}LED_handle;


//LED控制结构体声明
extern LED_handle LED1_BASE;
extern LED_handle LED2_BASE;
extern LED_handle LED3_BASE;
extern LED_handle LED4_BASE;
extern LED_handle LED5_BASE;
extern LED_handle LED6_BASE;
extern LED_handle LED7_BASE;
extern LED_handle LED8_BASE; 


//LED控制结构体指针
#define LED1 (&LED1_BASE)
#define LED2 (&LED2_BASE)
#define LED3 (&LED3_BASE)
#define LED4 (&LED4_BASE)
#define LED5 (&LED5_BASE)
#define LED6 (&LED6_BASE)
#define LED7 (&LED7_BASE)
#define LED8 (&LED8_BASE)

#define LED_Status GPIO_PinState
#define LED_OPEN  GPIO_PIN_RESET
#define LED_CLOSE GPIO_PIN_SET

//LED初始化验证, 若此项为1，则LED初始化完成后，会检测led灯是否成功使能。
#define LED_INIT_CHECK 1

/**
 * @brief LED 初始化函数，初始化所有板载led 从D1 到 D8
 * 
 * @retval none 
 * 
 */
void LED_init(void);

/**
* @brief LED控制函数
*
* @param LEDx 此参数为led控制结构体的地址，其参数为 LEDx
* 
* @param LEDstatus 为LED状态参数其状态有两种 LED_OPEN / LED_CLOSE
* 
* @retval none
*/
void LED_Control(const LED_handle* LEDx, LED_Status LEDstatus);

/**
 * @brief LED 闪烁函数,闪烁此数为一次
 * 
 * @param LEDx 为LED控制结构体的地址 
 * 
 * @param Delay led闪烁中间延时的时间，单位为ms
 * 
 * @retval none 
 */
void LED_flash(const LED_handle* LEDx, uint32_t Delay);

#endif
