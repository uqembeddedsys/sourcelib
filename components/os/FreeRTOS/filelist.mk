###################################################
# LIBRARY SOURCES

CPUFLAGS += -mlittle-endian -mthumb -mcpu=cortex-m4 -mthumb-interwork
CPUFLAGS += -mfloat-abi=hard -mfpu=fpv4-sp-d16

FREERTOS_PATH=$(SOURCELIB_ROOT)/components/os/FreeRTOS

CFLAGS += -DUSE_FREERTOS_SYSTICK
CFLAGS += -I$(FREERTOS_PATH)/include -I$(FREERTOS_PATH)/portable/GCC/ARM_CM4F

LIBSRCS += $(wildcard $(FREERTOS_PATH)/*.c) $(wildcard $(FREERTOS_PATH)/portable/GCC/ARM_CM4F/*.c)

