#ifndef __SYS_CFG_H__
#define __SYS_CFG_H__

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>
#include "stm32f10x_conf.h"
#include "ring_buffer.h"

/* USART Communication boards Interface */
#define USARTx                           USART1
#define USARTx_CLK                       RCC_APB2Periph_USART1
#define USARTx_IRQn                      USART1_IRQn
#define USARTx_IRQHandler                USART1_IRQHandler

#define USARTx_TX_PIN                    GPIO_Pin_9
#define USARTx_TX_GPIO_PORT              GPIOA
#define USARTx_TX_GPIO_CLK               RCC_AHBPeriph_GPIOA
#define USARTx_TX_SOURCE                 GPIO_PinSource9
#define USARTx_TX_AF                     GPIO_AF_USART1

#define USARTx_RX_PIN                    GPIO_Pin_10
#define USARTx_RX_GPIO_PORT              GPIOA
#define USARTx_RX_GPIO_CLK               RCC_AHBPeriph_GPIOA
#define USARTx_RX_SOURCE                 GPIO_PinSource10
#define USARTx_RX_AF                     GPIO_AF_USART1

typedef struct {
	uint32_t cpu_clock;
	uint32_t tick;
	uint32_t console_baudrate;
	uint32_t flash_used;
	uint32_t ram_used;
	uint32_t data_init_size;
	uint32_t data_non_init_size;
	uint32_t stack_avail;
	uint32_t heap_avail;
	uint32_t ram_other;
} system_info_t;

extern system_info_t system_info;

extern ring_buffer_char_t ring_buffer_char_shell_send;

extern void sys_cfg_tick();
extern void sys_cfg_console();
extern void sys_ctrl_delay_ms(volatile uint32_t count);

#ifdef __cplusplus
}
#endif

#endif //__SYS_CFG_H__