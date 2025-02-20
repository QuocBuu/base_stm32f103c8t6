# Utilitis define
Print = @echo "~"

# Name of build target ex: ak-base-kit-stm32l151-application.bin
NAME_MODULE = stm32f103-base
PROJECT = $(NAME_MODULE)

GCC_PATH			= $(HOME)/Workspace/Tools/gcc-arm-none-eabi-10.3-2021.10
PROGRAMER_PATH		= $(HOME)/Workspace/Tools/STMicroelectronics/STM32Cube/STM32CubeProgrammer/bin
OPENOCD_CFG_PATH	= /usr/local/share/openocd/scripts/board/stm32ldiscovery.cfg

# App start address, that need sync with declare in linker file and interrupt vector table.
APP_START_ADDR_VAL = 0x08000000
APP_ADDR_OPTION = -DAPP_START_ADDR=$(APP_START_ADDR_VAL)

# Define console baudrate, that is used to configure UART.
SYS_CONSOLE_BAUDRATE = 115200
CONSOLE_BAUDRATE_DEF = -DSYS_CONSOLE_BAUDRATE=$(SYS_CONSOLE_BAUDRATE)

# Include sources file
-include app/Makefile.mk
-include libraries/Makefile.mk
-include sys/Makefile.mk
-include common/Makefile.mk
-include gpio/Makefile.mk

#|---------------------------------------------------------------------------------------------------|
#| OPTIMIZE LEVELS                                                                                   |
#|------------|----------------------------------|--------------|---------|------------|-------------|
#|   option   | optimization levels              |execution time|code size|memory usage|complile time|
#|------------|----------------------------------|--------------|---------|------------|-------------|
#|   -O0      | compilation time                 |     (+)      |   (+)   |     (-)    |    (-)      |
#| -O1 || -O  | code size && execution time      |     (-)      |   (-)   |     (+)    |    (+)      |
#|   -O2      | more code size && execution time |     (--)     |         |     (+)    |    (++)     |
#|   -O3      | more code size && execution time |     (---)    |         |     (+)    |    (+++)    |
#|   -Os      | code size                        |              |   (--)  |            |    (++)     |
#|  -Ofast    | O3 with none math cals           |     (---)    |         |     (+)    |    (+++)    |
#|------------|----------------------------------|--------------|---------|------------|-------------|
OPTIMIZE_OPTION = -g -Os

# Release build option, that is used in fatal function, restart system when fatal, and debug levels.
RELEASE_OPTION = -URELEASE

# Enable PIN (using led life) interrupt analyzer function [entry IRQ -> led life ON] and [exit IRQ -> led life OFF].
IRQ_DEBUG_OPTION = -UAK_IO_IRQ_ANALYZER

# Interface compile option.
IF_LINK_OPTION = -DIF_LINK_UART_EN

OBJECTS_DIR = _build_$(NAME_MODULE)
TARGET = $(OBJECTS_DIR)/$(NAME_MODULE).axf

SOURCES		+=
SOURCES_	 = $(shell find $(SOURCES) -type f -printf "%f\n")
OBJECTS		+= $(patsubst %.c, $(OBJECTS_DIR)/%.o, $(SOURCES_))

# SOURCES		+=
# SOURCES_	 = $(shell find $(SOURCES) -type f -printf "%f\n")
# OBJECTS		+= $(patsubst %.c, $(OBJECTS_DIR)/%.d, $(SOURCES_))

# SOURCES_CPP	+=
# SOURCES_CPP_	 = $(shell find $(SOURCES_CPP) -type f -printf "%f\n")
# OBJECTS		+= $(patsubst %.cpp, $(OBJECTS_DIR)/%.o, $(SOURCES_CPP_))

# SOURCES_ASM	+=
# SOURCES_ASM_	 = $(shell find $(SOURCES_ASM) -type f -printf "%f\n")
# OBJECTS		+= $(patsubst %.s, $(OBJECTS_DIR)/%.o, $(SOURCES_ASM_))

LIBC		= $(GCC_PATH)/arm-none-eabi/lib/thumb/v7-m/nofp/libc.a
LIBM		= $(GCC_PATH)/arm-none-eabi/lib/thumb/v7-m/nofp/libm.a
LIBFPU		= $(GCC_PATH)/arm-none-eabi/lib/thumb/v7-m/nofp/libg.a
LIBRDPMON	= $(GCC_PATH)/arm-none-eabi/lib/thumb/v7-m/nofp/librdpmon.a
LIBSTDCPP_NANO	= $(GCC_PATH)/arm-none-eabi/lib/thumb/v7-m/nofp/libstdc++_nano.a

LIBGCC		= $(GCC_PATH)/lib/gcc/arm-none-eabi/10.3.1/thumb/v7-m/nofp/libgcc.a
LIBGCOV		= $(GCC_PATH)/lib/gcc/arm-none-eabi/10.3.1/thumb/v7-m/nofp/libgcov.a

LIB_PATH += -L$(GCC_PATH)/arm-none-eabi/lib/thumb/v7-m/nofp
LIB_PATH += -L$(GCC_PATH)/lib/gcc/arm-none-eabi/10.3.1/thumb/v7-m/nofp

# The command for calling the compiler.
CC		=	$(GCC_PATH)/bin/arm-none-eabi-gcc
CPP		=	$(GCC_PATH)/bin/arm-none-eabi-g++
AR		=	$(GCC_PATH)/bin/arm-none-eabi-ar
AS		=	$(GCC_PATH)/bin/arm-none-eabi-gcc -x assembler-with-cpp
LD		=	$(GCC_PATH)/bin/arm-none-eabi-ld
OBJCOPY		=	$(GCC_PATH)/bin/arm-none-eabi-objcopy
OBJDUMP		=	$(GCC_PATH)/bin/arm-none-eabi-objdump
OBJNM		=	$(GCC_PATH)/bin/arm-none-eabi-nm
ARM_GDB		=	$(GCC_PATH)/bin/arm-none-eabi-gdb-py
ARM_SIZE	=	$(GCC_PATH)/bin/arm-none-eabi-size

# Set the compiler CPU/FPU options.
# https://launchpadlibrarian.net/177524521/readme.txt
CPU = -mcpu=cortex-m3 -mthumb
FPU = 

# Interface option
IF_OPTION +=	$(IF_LINK_OPTION)		\

# Console log compile option
CONSOLE_OPTION +=			\
		-DLOGIN_PRINT_EN	\
		-DSYS_PRINT_EN		\
		-DAPP_PRINT_EN		\
		-DSYS_DBG_EN		\
		-DAPP_DBG_EN		\
		-DAPP_DBG_SIG_EN	\

GENERAL_FLAGS +=			\
		$(OPTIMIZE_OPTION)	\
		-DUSE_STDPERIPH_DRIVER	\
		-DSTM32F10X_MD		\
		$(CONSOLE_BAUDRATE_DEF)

COMPILER_FLAGS +=			\
		$(CPU)			\
		$(FPU)			\
		-ffunction-sections	\
		-fdata-sections		\
		-fstack-usage		\
		-MD			\
		-Wall			\
		-c			\

# C compiler flags
CFLAGS +=				\
		$(GENERAL_FLAGS)	\
		$(COMPILER_FLAGS)	\
		-std=c99		\

# C++ compiler flags
CPPFLAGS +=				\
		$(GENERAL_FLAGS)	\
		$(COMPILER_FLAGS)	\
		-std=c++11		\
		-fno-rtti		\
		-fno-exceptions		\
		-fno-use-cxa-atexit	\

# linker flags
LDFLAGS	=	-Map=$(OBJECTS_DIR)/$(PROJECT).map	\
		--gc-sections	\
		$(LIB_PATH)	\
		$(LIBC) $(LIBM) $(LIBSTDCPP_NANO) $(LIBGCC) $(LIBGCOV) $(LIBFPU) $(LIBRDPMON)

all: create $(TARGET)
	
create:
	$(Print) CREATE $(OBJECTS_DIR) folder
	@mkdir -p $(OBJECTS_DIR)

$(TARGET): $(OBJECTS) $(LIBC) $(LIBM) $(LIBSTDCPP_NANO) $(LIBGCC) $(LIBGCOV) $(LIBFPU) $(LIBRDPMON)
	$(Print) LD $@
	@$(LD) --entry reset_handler -T $(LDFILE) $(LDFLAGS) -o $(@) $(^)
	$(Print) OBJCOPY $(@:.axf=.bin)
	@$(OBJCOPY) -O binary $(@) $(@:.axf=.bin)
	@$(OBJCOPY) -O binary $(@) $(@:.axf=.out)
	@$(OBJCOPY) -O binary $(@) $(@:.axf=.elf)
	@$(ARM_SIZE) $(TARGET)

$(OBJECTS_DIR)/%.o: %.c
	$(Print) CC $@
	@$(CC) $(CFLAGS) -o $@ $<

$(OBJECTS_DIR)/%.o: %.cpp
	$(Print) CXX $@
	@$(CPP) $(CPPFLAGS) -o $@ $<

$(OBJECTS_DIR)/%.o: %.s
	$(Print) AS $@
	@$(AS) $(CFLAGS) -o $@ $<

.PHONY: flash
flash: all
	$(Print) BURNING $(TARGET:.axf=.bin) to target
ifdef dev
	ak-flash $(dev) $(TARGET:.axf=.bin) $(APP_START_ADDR_VAL)
else
	$(PROGRAMER_PATH)/STM32_Programmer.sh -c port=SWD -w $(TARGET:.axf=.bin) $(APP_START_ADDR_VAL) -rst
endif

.PHONY: com
com:
ifdef dev
	minicom -D $(dev) -b 115200
else
	$(Print) "Example: make com dev=/dev/ttyUSB0"
endif

.PHONY: debug
debug: all
	xterm -e openocd -f $(OPENOCD_PATH)/tcl/board/stm32ldiscovery.cfg &
ifeq ($(gdb),ddd)
	$(Print) DBG using DDD
	ddd --debugger $(ARM_GDB) --command=stm32l_init.gdb $(TARGET)
else
	$(Print) DBG GDB
	$(ARM_GDB) -w --command=stm32l_init.gdb $(TARGET)
endif

.PHONY: clean
clean:
	$(Print) CLEAN $(OBJECTS_DIR) folder
	@rm -rf $(OBJECTS_DIR)

asm: $(TARGET)
	$(Print) disassemble-all $(<:.axf=.asm)
	@$(OBJDUMP) -D $(<) >> $(<:.axf=.asm)

sym: $(TARGET)
	$(Print) export object name $(<:.axf=.sym)
	$(OBJNM) --size-sort --print-size $(<) >> $(<:.axf=.sym)
