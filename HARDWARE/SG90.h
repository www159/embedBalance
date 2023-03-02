#ifndef _SG90_H_
#define _SG90_H_

// PWM时钟定义
#define SG90_PWM_TIM    TIM3
#define SG90_PWM_TIM_APBENR APB1ENR
#define SG90_PWM_TIM_APBENR_OFST    1
// PWM模式1，CC1输出，输出比较预装载
#define SG90_PWM_TIM_CCMR   CCMR2
#define SG90_PWM_TIM_CCMR_MASK 0X00FF
#define SG90_PWM_TIM_CCMR_CNF 0X6800
// 比较输出，高电平有效
#define SG90_PWM_TIM_CCER_MASK  0X0FFF
#define SG90_PWM_TIM_CCER_CNF   0X1000
// 向上计数，计时器时钟使能，计数器预装载
#define SG90_PWM_TIM_CCR    CCR4
#define SG90_PWM_TIM_CR1_MASK   0XFF0E
#define SG90_PWM_TIM_CR1_CNF    0X0081

/**
 * PWM频率
 * F = 72M / (ARR + 1) / (PSC + 1) = 50 Hz
*/
#define SG90_PWM_TIM_ARR    1000
#define SG90_PWM_TIM_PSC    1438

// PWM计算定义
/**
 * SG90的占空比从2.5%到12.5%
 * 
*/

// GPIO定义
#define SG90_PORT   GPIOB
#define SG90_PORT_APBENR    APB2ENR
#define SG90_PORT_APBENR_OFST   3
#define SG90_PORT_OUT   (PBout(1))
#define SG90_PORT_CR  CRL
// 复用功能推挽输出， 最大50Mhz
#define SG90_PORT_CR_MASK   0XFFFFFF0F
#define SG90_PORT_CR_CNF    0X000000B0


// 接口
void SG90_init();
/**
 * @brief 旋转舵机
 * @param linear 取值 [25-125] 对应degree [0-180]
 * @return
 *  0 - 设置成功
 *  1 - 设置错误
*/
int32_t SG90_rotate(uint8_t linear);
int32_t SG90_reset();
#endif