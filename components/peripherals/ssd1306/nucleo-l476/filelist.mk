###################################################
# LIBRARY SOURCES

SSD1306_PATH=$(SOURCELIB_ROOT)/components/peripherals/ssd1306/nucleo-l476

CFLAGS += -I$(SSD1306_PATH)

LIBSRCS += $(wildcard $(SSD1306_PATH)/*.c)

