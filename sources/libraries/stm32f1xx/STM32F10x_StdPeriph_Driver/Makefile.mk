CFLAGS   += -I./libraries/stm32f1xx/STM32F10x_StdPeriph_Driver/inc
CPPFLAGS += -I./libraries/stm32f1xx/STM32F10x_StdPeriph_Driver/inc

VPATH += libraries/stm32f1xx/STM32F10x_StdPeriph_Driver/src


SOURCES += $(wildcard libraries/stm32f1xx/STM32F10x_StdPeriph_Driver/src/*.c)

# SOURCES += libraries/stm32f1xx/STM32F10x_StdPeriph_Driver/src/misc.c
# SOURCES += libraries/stm32f1xx/STM32F10x_StdPeriph_Driver/src/stm32f10x_can.c
# SOURCES += libraries/stm32f1xx/STM32F10x_StdPeriph_Driver/src/stm32f10x_dac.c
# SOURCES += libraries/stm32f1xx/STM32F10x_StdPeriph_Driver/src/stm32f10x_exti.c
# SOURCES += libraries/stm32f1xx/STM32F10x_StdPeriph_Driver/src/stm32f10x_gpio.c
# SOURCES += libraries/stm32f1xx/STM32F10x_StdPeriph_Driver/src/stm32f10x_pwr.c
# SOURCES += libraries/stm32f1xx/STM32F10x_StdPeriph_Driver/src/stm32f10x_sdio.c
# SOURCES += libraries/stm32f1xx/STM32F10x_StdPeriph_Driver/src/stm32f10x_usart.c
# SOURCES += libraries/stm32f1xx/STM32F10x_StdPeriph_Driver/src/stm32f10x_adc.c
# SOURCES += libraries/stm32f1xx/STM32F10x_StdPeriph_Driver/src/stm32f10x_cec.c
# SOURCES += libraries/stm32f1xx/STM32F10x_StdPeriph_Driver/src/stm32f10x_dbgmcu.c
# SOURCES += libraries/stm32f1xx/STM32F10x_StdPeriph_Driver/src/stm32f10x_flash.c
# SOURCES += libraries/stm32f1xx/STM32F10x_StdPeriph_Driver/src/stm32f10x_i2c.c
# SOURCES += libraries/stm32f1xx/STM32F10x_StdPeriph_Driver/src/stm32f10x_rcc.c
# SOURCES += libraries/stm32f1xx/STM32F10x_StdPeriph_Driver/src/stm32f10x_spi.c
# SOURCES += libraries/stm32f1xx/STM32F10x_StdPeriph_Driver/src/stm32f10x_wwdg.c
# SOURCES += libraries/stm32f1xx/STM32F10x_StdPeriph_Driver/src/stm32f10x_bkp.c
# SOURCES += libraries/stm32f1xx/STM32F10x_StdPeriph_Driver/src/stm32f10x_crc.c
# SOURCES += libraries/stm32f1xx/STM32F10x_StdPeriph_Driver/src/stm32f10x_dma.c
# SOURCES += libraries/stm32f1xx/STM32F10x_StdPeriph_Driver/src/stm32f10x_fsmc.c
# SOURCES += libraries/stm32f1xx/STM32F10x_StdPeriph_Driver/src/stm32f10x_iwdg.c
# SOURCES += libraries/stm32f1xx/STM32F10x_StdPeriph_Driver/src/stm32f10x_rtc.c
# SOURCES += libraries/stm32f1xx/STM32F10x_StdPeriph_Driver/src/stm32f10x_tim.c