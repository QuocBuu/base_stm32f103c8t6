
#include "sys_cfg.h"

#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_usart.h"
#include "misc.h"
#include "stm32f10x_i2c.h"
#include "stm32f10x_tim.h"
#include "stm32f10x_exti.h"
#include "stm32f10x_syscfg.h"

// #include "sys_irq.h"
#include "sys_platform.h"

#include "xprintf.h"

// #include "io_cfg.h"

static void xputchar(uint8_t c);

/*****************************************************************************/
/* linker variable                                                           */
/*****************************************************************************/
extern uint32_t _start_flash;
extern uint32_t _end_flash;
extern uint32_t _start_ram;
extern uint32_t _end_ram;
extern uint32_t _sdata;
extern uint32_t _edata;
extern uint32_t _sbss;
extern uint32_t _ebss;
extern uint32_t __heap_start__;
extern uint32_t __heap_end__;
extern uint32_t _sstack;
extern uint32_t _estack;

/******************************************************************************
* system configurating functions
*******************************************************************************/
void sys_cfg_tick() {
	volatile uint32_t ticks = SystemCoreClock / 1000;
	NVIC_InitTypeDef NVIC_InitStructure;

	NVIC_InitStructure.NVIC_IRQChannel = SysTick_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;

	SysTick->LOAD  = ticks - 1;                                  /* set reload register */

	NVIC_Init(&NVIC_InitStructure);

	SysTick->VAL   = 0;                                          /* Load the SysTick Counter Value */
	SysTick->CTRL  = SysTick_CTRL_CLKSOURCE_Msk |
			SysTick_CTRL_TICKINT_Msk   |
			SysTick_CTRL_ENABLE_Msk;                             /* Enable SysTick IRQ and SysTick Timer */
}

void sys_cfg_console() {
	USART_InitTypeDef USART_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	/* Enable GPIO clock */
	RCC_AHBPeriphClockCmd(USARTx_TX_GPIO_CLK | USARTx_RX_GPIO_CLK, ENABLE);

	/* Enable USART clock */
	RCC_APB2PeriphClockCmd(USARTx_CLK, ENABLE);

	/* Connect PXx to USARTx_Tx */
	GPIO_PinAFConfig(USARTx_TX_GPIO_PORT, USARTx_TX_SOURCE, USARTx_TX_AF);

	/* Connect PXx to USARTx_Rx */
	GPIO_PinAFConfig(USARTx_RX_GPIO_PORT, USARTx_RX_SOURCE, USARTx_RX_AF);

	/* Configure USART Tx and Rx as alternate function push-pull */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Pin = USARTx_TX_PIN;
	GPIO_Init(USARTx_TX_GPIO_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = USARTx_RX_PIN;
	GPIO_Init(USARTx_RX_GPIO_PORT, &GPIO_InitStructure);

	/* USARTx configuration */
	USART_InitStructure.USART_BaudRate = SYS_CONSOLE_BAUDRATE;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USARTx, &USART_InitStructure);

	/* Enable the USARTx Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = USARTx_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	USART_ClearITPendingBit(USARTx,USART_IT_RXNE);
	USART_ITConfig(USARTx, USART_IT_RXNE, ENABLE);

	/* Enable USART */
	USART_Cmd(USARTx, ENABLE);

	xfunc_out = xputchar;
}

/******************************************************************************
* system memory functions
*******************************************************************************/
static uint32_t sys_stack_size;

uint32_t sys_stack_fill() {
	volatile uint32_t* start_stack = &_sstack;
	uint32_t* current_stack = (uint32_t*)((uint32_t)__get_MSP());

	sys_stack_size = ((uint32_t)&_estack - (uint32_t)&_sstack);

	ENTRY_CRITICAL();
	while (start_stack < current_stack) {
		*start_stack++ = 0xDEADBEEBu;
	}
	EXIT_CRITICAL();

	return sys_stack_size;
}

uint32_t sys_stack_used() {
	volatile uint32_t* p_stack_check = &_sstack;
	while (*p_stack_check == 0xDEADBEEBu) {
		*p_stack_check++;
	}
	return ((uint32_t)&_estack - (uint32_t)p_stack_check);
}

uint32_t sys_stack_get_size() {
	return sys_stack_size;
}

/******************************************************************************
* system info function
*******************************************************************************/
system_info_t system_info;

void sys_ctrl_update_info() {
	RCC_ClocksTypeDef RCC_Clocks;

	RCC_GetClocksFreq(&RCC_Clocks);

	system_info.cpu_clock = RCC_Clocks.HCLK_Frequency;
	system_info.tick      = 1;
	system_info.console_baudrate = 115200;
	system_info.flash_used = ((uint32_t)&_end_flash - (uint32_t)&_start_flash) + ((uint32_t)&_edata - (uint32_t)&_sdata);
	system_info.ram_used = (uint32_t)&_estack - (uint32_t)&_start_ram;
	system_info.data_init_size = (uint32_t)&_edata - (uint32_t)&_sdata;
	system_info.data_non_init_size = (uint32_t)&_ebss - (uint32_t)&_sbss;
	system_info.stack_avail = (uint32_t)&_estack - (uint32_t)&_end_ram;
	system_info.heap_avail = (uint32_t)&__heap_end__ - (uint32_t)&__heap_start__;
	system_info.ram_other = system_info.ram_used - (system_info.heap_avail + system_info.stack_avail + system_info.data_non_init_size + system_info.data_init_size);
}

void sys_ctrl_show_info() {
	sys_ctrl_update_info();
	// printf("\n");
	// printf("\n");
	// printf("\n");
	// printf(" ___  ____  _  _ 	CPU clock:            %d Hz\n", system_info.cpu_clock);
	// printf("/ __)(_  _)( )/ )	Console baudrate:     %d bps\n", system_info.console_baudrate);
	// printf("\\__ \\  )(  (   (	Flash used:           %d\n", system_info.flash_used);
	// printf("(___/ (__) (_)\\_)	Data initialized:     %d\n", system_info.data_init_size);
	// printf("			Data non initialized: %d\n", system_info.data_non_init_size);
	// printf("Kernel version: %s	Stack available:      %d\n", "0.0.1", system_info.stack_avail);
	// printf("			Heap available:       %d\n", system_info.heap_avail);
	// printf("\n");
	// printf("-> Application start\n");
}

/******************************************************************************
* system utility functions
*******************************************************************************/
void sys_ctrl_delay_ms(volatile uint32_t count) {
	volatile uint32_t current_ticks = SysTick->VAL;

	/* number of ticks per millisecond */
	const uint32_t tick_per_ms = SysTick->LOAD + 1;

	/* number of ticks need to count */
	const uint32_t number_of_tick = ((count - ((count > 0) ? 1 : 0)) * tick_per_ms);

	/* number of elapsed ticks */
	uint32_t elapsed_tick = 0;
	volatile uint32_t prev_tick = current_ticks;
	while (number_of_tick > elapsed_tick) {
		current_ticks = SysTick->VAL;
		elapsed_tick += (prev_tick < current_ticks) ? (tick_per_ms + prev_tick - current_ticks) : (prev_tick - current_ticks);
		prev_tick = current_ticks;
	}
}

void sys_ctrl_reset() {
	NVIC_SystemReset();
}