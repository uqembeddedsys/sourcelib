###################################################
# LIBRARY SOURCES

NRF24L01PLUS_PATH=$(SOURCELIB_ROOT)/components/peripherals/nrf24l01plus/nucleo-f429

CFLAGS += -I$(NRF24L01PLUS_PATH)

LIBSRCS += $(wildcard $(NRF24L01PLUS_PATH)/*.c)

