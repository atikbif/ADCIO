#include "pwm.h"
#include "stm32f0xx_conf.h"

volatile unsigned short pwm1=0, pwm2=0;

void init_pwm(void)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    TIM_OCInitTypeDef  TIM_OCInitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;

    /* TIM1 clock enable */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);

    /* GPIOA clock enable */
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);

    /* GPIOA Configuration: TIM1 CH3 (PA10) */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    /* Connect TIM Channels to AF2 */
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_2);

    TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);

    TIM_OCStructInit(&TIM_OCInitStructure);

    /* Time base configuration */
    TIM_TimeBaseStructure.TIM_Period = 0xFFFF;
    TIM_TimeBaseStructure.TIM_Prescaler = 0;
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

    TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);

    /* Output Compare Active Mode configuration: Channel1 */
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;
    TIM_OCInitStructure.TIM_Pulse = 0;
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;
    TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;
    TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
    TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCIdleState_Reset;

    TIM_OC3Init(TIM1, &TIM_OCInitStructure);

    /* TIM1 counter enable */
    TIM_Cmd(TIM1, ENABLE);

    /* TIM1 Main Output Enable */
    TIM_CtrlPWMOutputs(TIM1, ENABLE);

    /* TIM1 clock enable */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

    /* GPIOA clock enable */
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);

    /* GPIOA Configuration: TIM3 CH4 (PB1) */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    /* Connect TIM Channels to AF1 */
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource1, GPIO_AF_1);

    TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);

    TIM_OCStructInit(&TIM_OCInitStructure);

    /* Time base configuration */
    TIM_TimeBaseStructure.TIM_Period = 0xFFFF;
    TIM_TimeBaseStructure.TIM_Prescaler = 0;
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

    /* Output Compare Active Mode configuration: Channel1 */
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;
    TIM_OCInitStructure.TIM_Pulse = 0;
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;
    TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;
    TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
    TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCIdleState_Reset;

    TIM_OC4Init(TIM3, &TIM_OCInitStructure);

    /* TIM1 counter enable */
    TIM_Cmd(TIM3, ENABLE);

    /* TIM1 Main Output Enable */
    TIM_CtrlPWMOutputs(TIM3, ENABLE);
}

unsigned short get_pwm(unsigned char num)
{
    if(num==1) {return pwm1;}else if(num==2) {return pwm2;}
    return 0;
}

void set_pwm(unsigned char num, unsigned short value)
{
    if(num==1) {TIM1->CCR3 = pwm1 = value;}
    else if(num==2) {TIM3->CCR4 = pwm2 = value;}
}
