/*
#include <rtdevice.h>
#include <rthw.h>
#include <rtthread.h>
#include <rtdevice.h>
#include <board.h>

static void adc_pin_config(void);
static void adc_reg_config(void);

static rt_err_t drv_config()
{


}

static void adc_pin_config(void)
{
	__HAL_RCC_GPIOA_CLK_ENABLE();
	GPIO_InitTypeDef GPIO_InitStruct;
	
	GPIO_InitStruct.Pin = GPIO_PIN_4;
  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}

static void adc_reg_config(void)
{
	ADC_HandleTypeDef ADC_HandleStruct;
	ADC_HandleStruct.Instance = ADC1;
	ADC_HandleStruct.Init.ClockPrescaler = 
	
	
	HAL_ADC_Init(ADC_HandleTypeDef* hadc)
	
	ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;  //不用DMA
	ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;  //独立采样
	ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div8;  //速度尽可能低
	ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_20Cycles;
	ADC_CommonInit(&ADC_CommonInitStructure);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE);
}
*/