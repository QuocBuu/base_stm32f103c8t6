
#include "sys_startup.h"

// #include "task.h"
// #include "message.h"

#include "system_stm32f1xx.h"
// #include "sys_irq.h"
// #include "sys_cfg.h"

// #include "app.h"
// #include "io_cfg.h"

// #include "platform.h"
// #include "led.h"

/*****************************************************************************/
/* linker variable                                                           */
/*****************************************************************************/
extern uint32_t _ldata;
extern uint32_t _sdata;
extern uint32_t _edata;
extern uint32_t _sbss;
extern uint32_t _ebss;
extern uint32_t _estack;

/*****************************************************************************/
/* static function prototype                                                 */
/*****************************************************************************/
/*****************************/
/* system interrupt function */
/*****************************/
void default_handler();
void reset_handler();
int main();

/* cortex-M processor fault exceptions */
void nmi_handler()          __attribute__ ((weak));
void hard_fault_handler()   __attribute__ ((weak));
void mem_manage_handler()   __attribute__ ((weak));
void bus_fault_handler()    __attribute__ ((weak));
void usage_fault_handler()  __attribute__ ((weak));

/* cortex-M processor non-fault exceptions */
void svc_handler()          __attribute__ ((weak, alias("default_handler")));
void dg_monitor_handler()   __attribute__ ((weak, alias("default_handler")));
void pendsv_handler()       __attribute__ ((weak, alias("default_handler")));
void systick_handler();

/* external interrupts */
static void shell_handler();

/* STM32F030 cortex-M0 processor exception vector table */
__attribute__((section(".isr_vector"))) void (*const g_pfnVectors[])(void) = {
		((void (*)())(uint32_t)&_estack),		//	The initial stack pointer
		reset_handler,							//	The reset handler
		nmi_handler,							//	The NMI handler
		hard_fault_handler,						//	The hard fault handler
		mem_manage_handler,						//	The MPU fault handler
		bus_fault_handler,						//	The bus fault handler
		usage_fault_handler,					//	The usage fault handler
		0,										//	Reserved
		0,										//	Reserved
		0,										//	Reserved
		0,										//	Reserved
		svc_handler,							//	SVCall handler
		dg_monitor_handler,						//	Debug monitor handler
		0,										//	Reserved
		pendsv_handler,							//	The PendSV handler
		systick_handler,						//	The SysTick handler

		//External Interrupts
		default_handler,	// Window Watchdog
		default_handler,	// PVD through EXTI Line detect
		default_handler,	// Tamper
		default_handler,	// RTC
		default_handler,	// Flash
		default_handler,	// RCC
		default_handler,	// EXTI Line 0
		default_handler,	// EXTI Line 1
		default_handler,	// EXTI Line 2
		default_handler,	// EXTI Line 3
		default_handler,	// EXTI Line 4
		default_handler,	// DMA1 Channel 1
		default_handler,	// DMA1 Channel 2
		default_handler,	// DMA1 Channel 3
		default_handler,	// DMA1 Channel 4
		default_handler,	// DMA1 Channel 5
		default_handler,	// DMA1 Channel 6
		default_handler,	// DMA1 Channel 7
		default_handler,	// ADC1 & ADC2
		default_handler,	// USB High Priority or CAN1 TX
		default_handler,	// USB Low  Priority or CAN1 RX0
		default_handler,	// CAN1 RX1
		default_handler,	// CAN1 SCE
		default_handler,	// EXTI Line 9..5
		default_handler,	// TIM1 Break
		default_handler,	// TIM1 Update
		default_handler,	// TIM1 Trigger and Commutation
		default_handler,	// TIM1 Capture Compare
		default_handler,	// TIM2
		default_handler,	// TIM3
		default_handler,	// TIM4
		default_handler,	// I2C1 Event
		default_handler,	// I2C1 Error
		default_handler,	// I2C2 Event
		default_handler,	// I2C2 Error
		default_handler,	// SPI1
		default_handler,	// SPI2
		shell_handler,	// USART1
		default_handler,	// USART2
		default_handler,	// USART3
		default_handler,	// EXTI Line 15..10
		default_handler,	// RTC Alarm through EXTI Line
		default_handler,	// USB Wakeup from suspend
		default_handler,	// TIM8 Break
		default_handler,	// TIM8 Update
		default_handler,	// TIM8 Trigger and Commutation
		default_handler,	// TIM8 Capture Compare
		default_handler,	// ADC3
		default_handler,	// FSMC
		default_handler,	// SDIO
		default_handler,	// TIM5
		default_handler,	// SPI3
		default_handler,	// UART4
		default_handler,	// UART5
		default_handler,	// TIM6
		default_handler,	// TIM7
		default_handler,	// DMA2 Channel1
		default_handler,	// DMA2 Channel2
		default_handler,	// DMA2 Channel3
		default_handler,	// DMA2 Channel4 & Channel5
};

void sys_delay_us(volatile uint32_t count) {
	volatile uint32_t delay_value = 0;
	delay_value = count * 4;
	while(delay_value--);
}

void sys_delay_ms(volatile uint32_t count) {
	volatile uint32_t delay = 1000 * count;
	sys_delay_us(delay);
}

/*****************************************************************************/
/* static function defination                                                */
/*****************************************************************************/
void default_handler() {
}

uint8_t power_on_reset __attribute__ ((section ("non_clear_ram")));

uint8_t sys_is_power_on_reset() {
	return (uint8_t)power_on_reset;
}

void reset_handler() {
	/* init system */
	SystemInit();

	/* pointer start address in flash */
	uint32_t *pSrc	= &_ldata;
	/* pointer start address in sram */
	uint32_t *pDest	= &_sdata;

	while (pDest < &_edata) {
        *pDest++ = *pSrc++;
    }

	/* zero fill to .bss section */
    pDest = &_sbss;
    while (pDest < &_ebss) {
        *pDest++ = 0;
    }

	// ENTRY_CRITICAL();
	sys_cfg_tick(); 

	main();
}

static void shell_handler() {
    // Function implementation here
}

extern uint32_t __heap_start__;
caddr_t _sbrk (uint32_t incr) {
	static uint8_t* heap = NULL;
	uint8_t* prev_heap;

	if (heap == NULL) {
		heap = (uint8_t*)((uint32_t)&__heap_start__);
	}

	prev_heap = heap;
	heap += incr;

	return (caddr_t) prev_heap;
}

static uint32_t millis_current;
void systick_handler() {
	static uint32_t div_counter = 0;

	/* increasing millis counter */
	millis_current++;

	if (div_counter == 10) {
		div_counter = 0;
	}

	switch (div_counter) {
	case 0:
		sys_delay_ms(1);
		break;

	default:
		break;
	}

	div_counter++;

}