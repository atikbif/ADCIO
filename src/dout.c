#include "dout.h"
#include "stm32f0xx_conf.h"

void init_dout(void)
{
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
    GPIO_InitTypeDef    GPIO_InitStructure;

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_SetBits(GPIOA, GPIO_Pin_7);
}

unsigned char get_dout(unsigned char num)
{
    unsigned char value = 0;
    if(num==1) {if(GPIO_ReadOutputDataBit(GPIOA,GPIO_Pin_7) == Bit_RESET) value = 1;}
    return value;
}

void set_dout(unsigned char num, unsigned char value)
{
    if(num==1) {if(value) GPIO_ResetBits(GPIOA, GPIO_Pin_7);else GPIO_SetBits(GPIOA, GPIO_Pin_7);}
}
