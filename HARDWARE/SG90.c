#include "sys.h"
#include "SG90.h"


void SG90_init()
{
    printf("\r\n初始化SG90模块\r\n");
    // 使能GPIO和TIM的时钟
    RCC->SG90_PORT_APBENR |= 1 << SG90_PORT_APBENR_OFST;
    RCC->SG90_PWM_TIM_APBENR |= 1<< SG90_PWM_TIM_APBENR_OFST;
    // GPIO初始化
    SG90_PORT->SG90_PORT_CR &= SG90_PORT_CR_MASK;
    SG90_PORT->SG90_PORT_CR |= SG90_PORT_CR_CNF;

    // TIM初始化
    // 计数器值和预分频值
    SG90_PWM_TIM->ARR = SG90_PWM_TIM_ARR;
    SG90_PWM_TIM->PSC = SG90_PWM_TIM_PSC;

    SG90_PWM_TIM->SG90_PWM_TIM_CCMR &= SG90_PWM_TIM_CCMR_MASK;
    SG90_PWM_TIM->SG90_PWM_TIM_CCMR |= SG90_PWM_TIM_CCMR_CNF;

    SG90_PWM_TIM->CCER &= SG90_PWM_TIM_CCER_MASK;
    SG90_PWM_TIM->CCER |= SG90_PWM_TIM_CCER_CNF;

    SG90_PWM_TIM->CR1 &= SG90_PWM_TIM_CR1_MASK;
    SG90_PWM_TIM->CR1 |= SG90_PWM_TIM_CR1_CNF;
}

int32_t SG90_rotate(uint8_t linear)
{
    if(linear < 25 || linear > 125) {
        return 1;
    }
    SG90_PWM_TIM->SG90_PWM_TIM_CCR = linear;
    return 0;
}

int32_t SG90_reset() {
    return SG90_rotate(0);
}