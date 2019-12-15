/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2018-05-07     aozima       the first version
 * 2018-11-16     Ernest Chen  add finsh command and update adc function
 */

#include <rtthread.h>
#include <rtdevice.h>

#include <string.h>
#include <stdlib.h>

#define DBG_TAG "adc"
#define DBG_LVL DBG_INFO
#include <rtdbg.h>

static rt_size_t _adc_read(rt_device_t dev, rt_off_t pos, void *buffer, rt_size_t size)
{
    rt_err_t result = RT_EOK;
    rt_size_t i;
    struct rt_adc_device *adc = (struct rt_adc_device *)dev;
    rt_uint32_t *value = (rt_uint32_t *)buffer;

    for (i = 0; i < size; i += sizeof(int))
    {
        result = adc->ops->convert(adc, pos + i, value);
        if (result != RT_EOK)
        {
            return 0;
        }
        value++;
    }

    return i;
}

static rt_err_t _adc_control(rt_device_t dev, int cmd, void *args)
{
    rt_err_t result = RT_EOK;
    rt_adc_device_t adc = (struct rt_adc_device *)dev;

    if (adc->ops->enabled == RT_NULL)
    {
        return -RT_ENOSYS;
    }
    if (cmd == RT_ADC_CMD_ENABLE)
    {
        result = adc->ops->enabled(adc, (rt_uint32_t)args, RT_TRUE);
    }
    else if (cmd == RT_ADC_CMD_DISABLE)
    {
        result = adc->ops->enabled(adc, (rt_uint32_t)args, RT_FALSE);
    }

    return result;
}

#ifdef RT_USING_DEVICE_OPS
const static struct rt_device_ops adc_ops =
{
    RT_NULL,
    RT_NULL,
    RT_NULL,
    _adc_read,
    RT_NULL,
    _adc_control,
};
#endif

rt_err_t rt_hw_adc_register(rt_adc_device_t device, const char *name, const struct rt_adc_ops *ops, const void *user_data)
{
    rt_err_t result = RT_EOK;
    RT_ASSERT(ops != RT_NULL && ops->convert != RT_NULL);

    device->parent.type = RT_Device_Class_Miscellaneous;
    device->parent.rx_indicate = RT_NULL;
    device->parent.tx_complete = RT_NULL;

#ifdef RT_USING_DEVICE_OPS
    device->parent.ops         = &adc_ops;
#else
    device->parent.init        = RT_NULL;
    device->parent.open        = RT_NULL;
    device->parent.close       = RT_NULL;
    device->parent.read        = _adc_read;
    device->parent.write       = RT_NULL;
    device->parent.control     = _adc_control;
#endif
    device->ops = ops;
    device->parent.user_data = (void *)user_data;

    result = rt_device_register(&device->parent, name, RT_DEVICE_FLAG_RDWR);

    return result;
}
//#include "board.h"

//ADC_HandleTypeDef hadc1;
rt_uint32_t rt_adc_read(rt_adc_device_t dev, rt_uint32_t channel)
{
    rt_uint32_t value;

    RT_ASSERT(dev);
//	HAL_ADC_Start(&hadc1);
    dev->ops->convert(dev, channel, &value);

    return value;
}


//void config()
//{
//    GPIO_InitTypeDef GPIO_InitStructure;//

//		ADC_InitTypeDef ADC_InitStructyre;
//		ADC_HandleTypeDef AdcHandle;
//		__HAL_RCC_GPIOB_CLK_ENABLE();
//		__HAL_RCC_ADC1_CLK_ENABLE();
//	
//    GPIO_InitStructure.Mode = GPIO_MODE_ANALOG;
//    GPIO_InitStructure.Pin = GPIO_PIN_0;
//    GPIO_InitStructure.Pull = GPIO_NOPULL;
////    GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
//    HAL_GPIO_Init(GPIOB,&GPIO_InitStructure);
//	//

//	
//  ADC_ChannelConfTypeDef sConfig = {0};//

//  /* USER CODE BEGIN ADC1_Init 1 *///

//  /* USER CODE END ADC1_Init 1 */
//  /** Configure the global features of the ADC (Clock, Resolution, Data Alignment and number of conversion) 
//  */
//  hadc1.Instance = ADC1;
//  hadc1.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV4;
//  hadc1.Init.Resolution = ADC_RESOLUTION_12B;
//  hadc1.Init.ScanConvMode = DISABLE;
//  hadc1.Init.ContinuousConvMode = ENABLE;
//  hadc1.Init.DiscontinuousConvMode = DISABLE;
//  hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
//  hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
//  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
//  hadc1.Init.NbrOfConversion = 1;
//  hadc1.Init.DMAContinuousRequests = DISABLE;
//  hadc1.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
//  if (HAL_ADC_Init(&hadc1) != HAL_OK)
//  {
//    rt_kprintf("HAL_ADC_Init err");
//  }
//  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time. 
//  */
//  sConfig.Channel = ADC_CHANNEL_8;
//  sConfig.Rank = 1;
//  sConfig.SamplingTime = ADC_SAMPLETIME_3CYCLES;
//  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
//  {
//     rt_kprintf("HAL_ADC_ConfigChannel err");
//  }
////	HAL_ADC_Start(&AdcHandle);
//}
rt_err_t rt_adc_enable(rt_adc_device_t dev, rt_uint32_t channel)
{
    rt_err_t result = RT_EOK;
//		config();
    RT_ASSERT(dev);
    if (dev->ops->enabled != RT_NULL)
    {
        result = dev->ops->enabled(dev, channel, RT_TRUE);
    }
    else
    {
        result = -RT_ENOSYS;
    }

    return result;
}

rt_err_t rt_adc_disable(rt_adc_device_t dev, rt_uint32_t channel)
{
    rt_err_t result = RT_EOK;

    RT_ASSERT(dev);
    if (dev->ops->enabled != RT_NULL)
    {
        result = dev->ops->enabled(dev, channel, RT_FALSE);
    }
    else
    {
        result = -RT_ENOSYS;
    }

    return result;
}

#ifdef FINSH_USING_MSH

static int adc(int argc, char **argv)
{
    int value = 0;
    int result = RT_EOK;
    static rt_adc_device_t adc_device = RT_NULL;
    char *result_str;
    if (argc > 1)
    {
        if (!strcmp(argv[1], "probe"))
        {
            if (argc == 3)
            {
                adc_device = (rt_adc_device_t)rt_device_find(argv[2]);
                result_str = (adc_device == RT_NULL) ? "failure" : "success";
                rt_kprintf("probe %s %s \n", argv[2], result_str);
            }
            else
            {
                rt_kprintf("adc probe <adc_name>   - probe adc by name\n");
            }
        }
        else
        {
            if (adc_device == RT_NULL)
            {
                rt_kprintf("Please using 'adc probe <adc_name>' first\n");
                return -RT_ERROR;
            }
            if (!strcmp(argv[1], "enable"))
            {
                if (argc == 3)
                {
                    result = rt_adc_enable(adc_device, atoi(argv[2]));
                    result_str = (result == RT_EOK) ? "success" : "failure";
                    rt_kprintf("%s channel %d enables %s \n", adc_device->parent.parent.name, atoi(argv[2]), result_str);
                }
                else
                {
                    rt_kprintf("adc enable <channel>   - enable adc channel\n");
                }
            }
            else if (!strcmp(argv[1], "read"))
            {
                if (argc == 3)
                {
                    value = rt_adc_read(adc_device, atoi(argv[2]));
                    rt_kprintf("%s channel %d  read value is 0x%08X \n", adc_device->parent.parent.name, atoi(argv[2]), value);
                }
                else
                {
                    rt_kprintf("adc read <channel>     - read adc value on the channel\n");
                }
            }
            else if (!strcmp(argv[1], "disable"))
            {
                if (argc == 3)
                {
                    result = rt_adc_disable(adc_device, atoi(argv[2]));
                    result_str = (result == RT_EOK) ? "success" : "failure";
                    rt_kprintf("%s channel %d disable %s \n", adc_device->parent.parent.name, atoi(argv[2]), result_str);
                }
                else
                {
                    rt_kprintf("adc disable <channel>  - disable adc channel\n");
                }
            }
            else
            {
                rt_kprintf("Unknown command. Please enter 'adc' for help\n");
            }
        }
    }
    else
    {
        rt_kprintf("Usage: \n");
        rt_kprintf("adc probe <adc_name>   - probe adc by name\n");
        rt_kprintf("adc read <channel>     - read adc value on the channel\n");
        rt_kprintf("adc disable <channel>  - disable adc channel\n");
        rt_kprintf("adc enable <channel>   - enable adc channel\n");
        result = -RT_ERROR;
    }
    return RT_EOK;
}
MSH_CMD_EXPORT(adc, adc function);

#endif /* FINSH_USING_MSH */
