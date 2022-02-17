
LIBSTM32=libstm32.a

CC=arm-none-eabi-gcc
AR=arm-none-eabi-ar
OBJCOPY=arm-none-eabi-objcopy

# note: cpu flags are required for both compile and link
CPUFLAGS ?= -mlittle-endian -mthumb -mcpu=cortex-m4 -mthumb-interwork
#CPUFLAGS += -mfloat-abi=soft -mfpu=fpv4-sp-d16
LDFLAGS = $(CPUFLAGS) -T$(BRD_PATH)/$(LINKER_FILE).ld

CFLAGS += -g -O0 -Wmaybe-uninitialized -Wextra -std=gnu99 -Wsign-compare
CFLAGS += $(CPUFLAGS)

LD_SYS_LIBS = -lm -lc -lgcc -lnosys

STARTUP = $(BRD_PATH)/$(STARTUP_FILE).s	# add startup file to build
####################################################
# Startup file, linker file and CPU model settings

STARTUP_FILE=startup_stm32f401xe
LINKER_FILE=STM32F401RETx_FLASH
#LINKER_FILE=STM32F401VEHx_FLASH
CPU_MODEL=STM32F401xE
 
###################################################
# LIBRARY SOURCES

BRD_PATH=$(SOURCELIB_ROOT)/components/boards/nucleo-f401re
HAL_PATH=$(SOURCELIB_ROOT)/components/hal/STM32F4xx_HAL_Driver
UTIL_PATH=$(SOURCELIB_ROOT)/components/util
CMSIS_PATH=$(SOURCELIB_ROOT)/components/hal/CMSIS/Include
CMSISRTOS_PATH=$(SOURCELIB_ROOT)/components/os/CMSIS_RTOS

CFLAGS += -I$(CMSIS_PATH) -I$(BRD_PATH)/Inc -I$(HAL_PATH)/Inc
CFLAGS += -I$(UTIL_PATH) -D$(CPU_MODEL)

# Use UART serial port
ifdef ENABLE_DEBUG_UART
CFLAGS += -DENABLE_DEBUG_UART  #Enable UART4 for debug printf
endif

ifdef MYLIB		#Custom libraries
LIBSRCS += $(wildcard $(MYLIB)/*.c)
endif

#HAL and Board libraries
LIBSRCS += $(wildcard $(HAL_PATH)/Src/*.c) $(wildcard $(UTIL_PATH)/*.c)
LIBSRCS += $(wildcard $(BRD_PATH)/Src/*.c)

OBJDIR = obj
LIBOBJ = $(addprefix $(OBJDIR)/, $(notdir $(LIBSRCS:%.c=%.o)))
LIBOBJAS = $(addprefix $(OBJDIR)/, $(notdir $(STARTUP:%.s=%.o)))

PROJOBJ = $(LIBOBJ) $(LIBOBJAS)

obj/%.o:
	@echo "Building $^"
	@$(CC) $(CFLAGS) -c $^ -o $@

obj/$(STARTUP_FILE).o: $(STARTUP)
	$(CC) $(CFLAGS) -c $(STARTUP) -o $@

define make-deps
$(OBJDIR)/$(notdir $(1:%.c=%.o)): $1
endef

$(foreach d, $(LIBSRCS), $(eval $(call make-deps,$d)))
