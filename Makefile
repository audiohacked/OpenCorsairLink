CC ?= gcc
CFLAGS ?= -std=c99 -pedantic-errors
LDFLAGS ?= -lm

# pkg-config for libusb-1.0
CFLAGS += $(shell pkg-config --cflags libusb-1.0)
LDFLAGS += $(shell pkg-config --libs libusb-1.0)

OBJS = main.o \
	device.o \
	driver.o \
	options.o \
	print.o \
	scan.o
OBJS_LL = lowlevel/asetek4.o \
	lowlevel/hid.o \
	lowlevel/rmi.o
OBJS_PROTO = protocol/asetek4/core.o \
	protocol/asetek4/fan.o \
	protocol/asetek4/led.o \
	protocol/asetek4/pump.o \
	protocol/asetek4/temperature.o \
	protocol/rmi/core.o \
	protocol/rmi/power.o \
	protocol/rmi/temperature.o \
	protocol/rmi/time.o \
	protocol/hid/core.o \
	protocol/hid/fan.o \
	protocol/hid/led.o \
	protocol/hid/pump.o \
	protocol/hid/temperature.o

default: all

all: OpenCorsairLink.elf

OpenCorsairLink.elf: $(OBJS) $(OBJS_PROTO) $(OBJS_LL) $(OBJS)
	$(CC) $^ $(CFLAGS) $(LDFLAGS) -o $@

%.o: %.c
	$(CC) $(CFLAGS) -g -c -o $@ $<

clean:
	$(RM) OpenCorsairLink.elf $(OBJS) $(OBJS_LL) $(OBJS_PROTO)
