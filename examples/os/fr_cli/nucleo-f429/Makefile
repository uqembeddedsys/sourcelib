# Add additional project sources like this:
# SRCS += X.c
#
# all the files will be generated with this name (main.elf, main.bin, main.hex, etc)

#location of freertos source files
FREERTOS_PATH=$(SOURCELIB_ROOT)/components/os/FreeRTOS
FREERTOSCLI_PATH=$(SOURCELIB_ROOT)/components/os/FreeRTOS-Plus/Source/FreeRTOS-Plus-CLI

#Name of project
PROJ_NAME=main

#Your local source files must ONLY be added to SRCS
SRCS = $(PROJ_NAME).c 
CFLAGS += -I.  -DENABLE_DEBUG_UART #enable Com Port

###################################################
ROOT=$(shell pwd)

OBJS=$(SRCS:.c=.o)

###################################################

.PHONY: $(shell mkdir -p obj) lib proj all depend
all: proj

#local filelist.mk file
include ./filelist.mk

#Include freertos makefile
include $(FREERTOS_PATH)/filelist.mk

#Include freertos makefile
include $(FREERTOSCLI_PATH)/filelist.mk

#include nucelo-f429zi makefile - NOTE MUST BE LAST TO INCLUDE
include $(SOURCELIB_ROOT)/components/boards/nucleo-f429zi/board.mk
include $(SOURCELIB_ROOT)/components/mk/m_launch.mk
include $(SOURCELIB_ROOT)/components/mk/m_common.mk

lib:
	$(MAKE) -C lib

proj: 	$(PROJ_NAME).elf

include .depend
