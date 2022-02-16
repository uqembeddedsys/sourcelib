
LIBSTM32=libstm32.a

CC=arm-none-eabi-gcc
AR=arm-none-eabi-ar
OBJCOPY=arm-none-eabi-objcopy

# note: cpu flags are required for both compile and link
CPUFLAGS ?= -mlittle-endian -mthumb -mcpu=cortex-m4 -mthumb-interwork
#CPUFLAGS += -mfloat-abi=hard -mfpu=fpv4-sp-d16
LDFLAGS = $(CPUFLAGS) -T$(BRD_PATH)/$(LINKER_FILE).ld

CFLAGS += -g -O0 -Wmaybe-uninitialized -Wextra -std=gnu99 -Wsign-compare
CFLAGS += $(CPUFLAGS)

LD_SYS_LIBS = -lm -lc -lgcc -lnosys

STARTUP = $(BRD_PATH)/$(STARTUP_FILE).s	# add startup file to build
####################################################
# Startup file, linker file and CPU model settings

STARTUP_FILE=startup_stm32f429xx
LINKER_FILE=STM32F429ZITx_FLASH
CPU_MODEL=STM32F429xx
 
###################################################
# LIBRARY SOURCES

BRD_PATH=$(SOURCELIB_ROOT)/components/boards/nucleo-f429zi
HAL_PATH=$(SOURCELIB_ROOT)/components/hal/STM32F4xx_HAL_Driver
UTIL_PATH=$(SOURCELIB_ROOT)/components/util
CMSIS_PATH=$(SOURCELIB_ROOT)/components/hal/CMSIS/Include
CMSISRTOS_PATH=$(SOURCELIB_ROOT)/components/os/CMSIS_RTOS
ETHERNET_PATH=$(BRD_PATH)/netif

USB_PATH=$(BRD_PATH)/usb
VCP_PATH=$(USB_PATH)/vcp
HID_PATH=$(USB_PATH)/hid

USBCORE_PATH=$(SOURCELIB_ROOT)/components/hal/stm32/STM32_USB_Device_Library/Core
USBCDC_PATH=$(SOURCELIB_ROOT)/components/hal/stm32/STM32_USB_Device_Library/Class/CDC
USBHID_PATH=$(SOURCELIB_ROOT)/components/hal/stm32/STM32_USB_Device_Library/Class/HID

CFLAGS += -I$(USBCORE_PATH)/Inc -I$(USBCDC_PATH)/Inc -I$(VCP_PATH) -I$(HID_PATH) -I$(USB_PATH) -I. -I$(USBHID_PATH)/Inc
CFLAGS += -I$(CMSIS_PATH) -I$(BRD_PATH)/Inc -I$(HAL_PATH)/Inc
CFLAGS += -I$(UTIL_PATH) -D$(CPU_MODEL)

# Use UART serial port
ifdef ENABLE_DEBUG_UART
CFLAGS += -DENABLE_DEBUG_UART  #Enable UART4 for debug printf
endif

ifdef MYLIB		#Custom libraries
LIBSRCS += $(wildcard $(MYLIB)/*.c)
endif
ifdef ENABLE_VCP	#USB VCP libraries
CFLAGS += -DENABLE_VCP
LIBSRCS += $(wildcard $(USBCORE_PATH)/Src/*.c) $(wildcard $(USB_PATH)/*.c) $(wildcard $(VCP_PATH)/*.c) $(wildcard $(USBCDC_PATH)/Src/*.c)
endif
ifdef ENABLE_HID	#USB HID libraries
CFLAGS += -DENABLE_HID
LIBSRCS += $(wildcard $(USBCORE_PATH)/Src/*.c) $(wildcard $(USBHID_PATH)/Src/*.c)
LIBSRCS += $(wildcard $(USB_PATH)/*.c) $(wildcard $(HID_PATH)/*.c)
endif

ifdef ENABLE_ETHERNET
CFLAGS += -I$(ETHERNET_PATH) -I$(CMSISRTOS_PATH)
LIBSRCS += $(wildcard $(ETHERNET_PATH)/*.c) $(wildcard $(CMSISRTOS_PATH)/*.c)
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
