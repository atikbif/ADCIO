#include "adc.h"
#include "stm32f0xx_conf.h"

#define ADC_CH 2  //количество каналов
#define OVER_SAMPL 32 //количество семплов для усреднения
#define ADC1_DR_Address    ((uint32_t)0x40012440)
#define ADC_SampleTime ADC_SampleTime_55_5Cycles

uint32_t ADC_SUM[ADC_CH]; //буффер для суммирования
volatile uint16_t curr_sample;//считаем количество семлов
volatile uint16_t AI[ADC_CH],ADC_VAL[ADC_CH];//АI- финальный буфер, ADC_VAL- в нее пишем DMA

void init_adc(void)
{
    GPIO_InitTypeDef    GPIO_InitStructure;
    NVIC_InitTypeDef    NVIC_InitStructure;
    DMA_InitTypeDef     DMA_InitStructure; //Variable used to setup the DMA
    ADC_InitTypeDef     ADC_InitStructure;

    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
    ADC_DeInit(ADC1);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    //Настройка параметров  ADC1

    ADC_StructInit(&ADC_InitStructure);

    ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
    ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
    ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
    ADC_InitStructure.ADC_ScanDirection = ADC_ScanDirection_Upward;
    ADC_Init(ADC1, &ADC_InitStructure);

    //Порядок оцифровки

    ADC_ChannelConfig(ADC1, ADC_Channel_5,  ADC_SampleTime);
    ADC_ChannelConfig(ADC1, ADC_Channel_6,  ADC_SampleTime);

    //Калибровка ADC1
    ADC_GetCalibrationFactor(ADC1);
    ADC_DMARequestModeConfig(ADC1, ADC_DMAMode_Circular);

    ADC_DMACmd(ADC1, ENABLE); //Enable ADC1 DMA
    ADC_Cmd(ADC1, ENABLE); //Enable ADC1

    while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_ADRDY));

    ADC_StartOfConversion(ADC1);

    //==Configure DMA1 - Channel1==
    DMA_DeInit(DMA1_Channel1); //Set DMA registers to default values
    DMA_InitStructure.DMA_PeripheralBaseAddr = ADC1_DR_Address; //Address of peripheral the DMA must map to
    DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t) & ADC_VAL; //Variable to which ADC values will be stored
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
    DMA_InitStructure.DMA_BufferSize = ADC_CH; //Buffer size (8 because we using 8 channels)
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
    DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
    DMA_InitStructure.DMA_Priority = DMA_Priority_High;
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;


    //Настройка Прерывание -по окончании трансфера

    NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel1_IRQn ;
    NVIC_InitStructure.NVIC_IRQChannelPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    DMA_ITConfig(DMA1_Channel1, DMA1_IT_TC1, ENABLE);
    DMA_Init(DMA1_Channel1, &DMA_InitStructure); //Initialise the DMA




    DMA_Cmd(DMA1_Channel1, ENABLE);
}

void DMA1_Channel1_IRQHandler(void)
{
    DMA_ClearITPendingBit( DMA1_IT_TC1);
    DMA_ITConfig(DMA1_Channel1, DMA1_IT_TC1, DISABLE);

    if(curr_sample<OVER_SAMPL)
    {
        ADC_SUM[0]+=ADC_VAL[0];
        ADC_SUM[1]+=ADC_VAL[1];
    }

    curr_sample++;

    DMA_ITConfig(DMA1_Channel1, DMA1_IT_TC1, ENABLE);

    if(curr_sample>=OVER_SAMPL)
    DMA_ITConfig(DMA1_Channel1, DMA1_IT_TC1, DISABLE);
}

unsigned short get_adc(unsigned char num)
{
    uint32_t value = 0;
    if((num<=ADC_CH)&&(num>0)) value = AI[num-1];
    value = (value * 147)/142;
    if(value>0xFFFF) value = 0xFFFF;
    return value;
}

void update_adc(void)
{
    int16_t i;

    if(curr_sample>=OVER_SAMPL)
    {
        for(i=0;i<ADC_CH;i++)
        {
            //averaging
            AI[i]=(AI[i]*2+(ADC_SUM[i]/OVER_SAMPL))/3;
            ADC_SUM[i]=0;
        }
        curr_sample=0;
        DMA_ITConfig(DMA1_Channel1, DMA1_IT_TC1, ENABLE);
    }
}
