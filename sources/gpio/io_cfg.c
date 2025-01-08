#include <stdint.h>
#include <stdbool.h>

#include "io_cfg.h"

/******************************************************************************
* led status function
*******************************************************************************/
void led_init() {
	GPIO_InitTypeDef        GPIO_InitStructure;

	RCC_AHBPeriphClockCmd(LED_IO_CLOCK, ENABLE);

	GPIO_InitStructure.GPIO_Pin = LED_IO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(LED_IO_PORT, &GPIO_InitStructure);
}

void led_on() {
	GPIO_SetBits(LED_IO_PORT, LED_IO_PIN);
}

void led_off() {
	GPIO_ResetBits(LED_IO_PORT, LED_IO_PIN);
}