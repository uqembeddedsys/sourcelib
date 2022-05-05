###################################################
# LIBRARY SOURCES

CLI_PATH=$(SOURCELIB_ROOT)/components/os/FreeRTOS-Plus/Source/FreeRTOS-Plus-CLI

CFLAGS += -I$(CLI_PATH)

LIBSRCS += $(wildcard $(CLI_PATH)/*.c)

