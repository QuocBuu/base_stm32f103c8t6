#include "stdio.h"
#include "io_cfg.h"
#include "sys_startup.h"

int main() {
	printf("hello!!!");

	led_init();
	while (1)
	{
		led_on();
		sys_delay_ms(200);
		led_off();
		sys_delay_ms(200);
	}
	

	return 0;
}