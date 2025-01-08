#ifndef __IO_CFG_H__
#define __IO_CFG_H__

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>
#include "stm32f10x_conf.h"
#include "system_stm32f10x.h"
#include "core_cm3.h"

#include "ring_buffer.h"

/*****************************************************************************
 *Pin map led life
******************************************************************************/
#define LED_IO_PIN					(GPIO_Pin_2)
#define LED_IO_PORT					(GPIOB)
#define LED_IO_CLOCK				(RCC_AHBPeriph_GPIOB)

/******************************************************************************
* led status function
*******************************************************************************/
extern void led_init();
extern void led_on();
extern void led_off();

#ifdef __cplusplus
}
#endif

#endif //__IO_CFG_H__
