CFLAGS   += -I./libraries/stm32f1xx/CMSIS/CM3/CoreSupport
CFLAGS   += -I./libraries/stm32f1xx/CMSIS/CM3/DeviceSupport/ST/STM32F10x

CPPFLAGS += -I./libraries/stm32f1xx/CMSIS/CM3/CoreSupport
CPPFLAGS   += -I./libraries/stm32f1xx/CMSIS/CM3/DeviceSupport/ST/STM32F10x


VPATH += libraries/stm32f1xx/CMSIS/CM3/CoreSupport
VPATH += libraries/stm32f1xx/CMSIS/CM3/DeviceSupport/ST/STM32F10x

SOURCES += libraries/stm32f1xx/CMSIS/CM3/CoreSupport/core_cm3.c
SOURCES += libraries/stm32f1xx/CMSIS/CM3/DeviceSupport/ST/STM32F10x/system_stm32f10x.c