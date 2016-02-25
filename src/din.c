#include "din.h"
#include "stm32f0xx_conf.h"


void init_din(void)
{
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
    GPIO_InitTypeDef    GPIO_InitStructure;

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
}

unsigned char get_din(unsigned char num)
{
    if(num==1) {if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)==Bit_RESET) return 1;}
    else if(num==2) {if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_1)==Bit_RESET) return 1;}
    return 0;
}
