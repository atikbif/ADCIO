#ifndef ADC_H_INCLUDED
#define ADC_H_INCLUDED

void init_adc(void);
unsigned short get_adc(unsigned char num);
void update_adc(void);
void DMA1_Channel1_IRQHandler(void);

#endif /* ADC_H_INCLUDED */
