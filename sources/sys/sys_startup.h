#ifndef __SYS_STARTUP_H__
#define __SYS_STARTUP_H__

#ifdef __cplusplus
 extern "C" {
#endif 

#include <stdint.h>
#include <stdio.h>

extern int main();
extern void sys_delay_us(volatile uint32_t count);
extern void sys_delay_ms(volatile uint32_t count);

#ifdef __cplusplus
}
#endif

#endif /* __SYS_STARTUP_H__ */