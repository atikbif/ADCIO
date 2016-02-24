/*
**
**                           Main.c
**
**
**********************************************************************/
/*
   Last committed:     $Revision: 00 $
   Last changed by:    $Author: $
   Last changed date:  $Date:  $
   ID:                 $Id:  $

**********************************************************************/
#include "stm32f0xx_conf.h"

#include "crc.h"
#include "modbus.h"
#include "uart.h"
#include "pwm.h"

volatile unsigned short gl_tmr = 0;

request req;

void led_init(void);
void toggle_led(void);
void led_on(void);
void led_off(void);
void delay_ms(unsigned short value);


int main(void)
{
    unsigned short tmp;
    unsigned char* rx_buf;
    unsigned char* tx_buf;
    FLASH_Unlock();

    if (SysTick_Config(SystemCoreClock / 1000)) {/* Capture error */ while (1);}
    led_init();
    init_uart();
    init_pwm();
    rx_buf = get_rx_ptr();
    tx_buf = get_tx_ptr();
    req.tx_buf = tx_buf; req.rx_buf = rx_buf;

    while(1)
    {
        if((get_rx_cnt())&&(get_pc_tmr()>10))
		{
		    toggle_led();
		    if((rx_buf[0]==0x01)&&(GetCRC16(rx_buf,get_rx_cnt())==0))
			{
				// разбор команд
				switch(rx_buf[1])
				{
				    case 0x00:break;
				    case 0x03:
						req.addr=((unsigned short)rx_buf[2]<<8) | rx_buf[3];
						req.cnt=((unsigned short)rx_buf[4]<<8) | rx_buf[5];
						tmp = read_holdregs(&req);
						write_canal(tmp);
						break;
                    case 0x06:
						req.addr=((unsigned short)rx_buf[2]<<8) | rx_buf[3];
						req.cnt=((unsigned short)rx_buf[4]<<8) | rx_buf[5];
						tmp = write_single_reg(&req);
						write_canal(tmp);
						break;
					case 0x10:
						req.addr=((unsigned short)rx_buf[2]<<8) | rx_buf[3];
						req.cnt=((unsigned short)rx_buf[4]<<8) | rx_buf[5];
						tmp=write_multi_regs(&req);
						write_canal(tmp);
						break;
				}
			}
			clear_rx_cnt();
		}
		delay_ms(5);
    }
}

void led_init(void)
{
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
    GPIO_InitTypeDef    GPIO_InitStructure;

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_SetBits(GPIOA, GPIO_Pin_0);
}

void toggle_led(void)
{
    GPIOA->ODR ^= GPIO_Pin_0;
}

void led_on(void)
{

}

void led_off(void)
{

}

void delay_ms(unsigned short value)
{
    unsigned short wait_tmr = gl_tmr + value;
    while(gl_tmr!=wait_tmr);
}

