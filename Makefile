CC ?= gcc
CFLAGS ?= -std=c99 -pedantic-errors `pkg-config --cflags libusb-1.0`
LDFLAGS ?= `pkg-config --libs libusb-1.0`

OBJS = main.o \
			options.o \
			device.o
OBJS_LL = lowlevel/asetek4.o \
			lowlevel/hid.o
OBJS_PROTO = protocol/asetek4/core.o \
			protocol/asetek4/fan.o \
			protocol/asetek4/led.o \
			protocol/asetek4/pump.o \
			protocol/hid/core.o \
			protocol/hid/led.o

default: all

all: OpenCorsairLink.elf
	
OpenCorsairLink.elf: $(OBJS_PROTO) $(OBJS_LL) $(OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@ 

%.o: %.c
	$(CC) $(CFLAGS) -g -c -o $@ $<

clean:
	$(RM) OpenCorsairLink.elf $(OBJS) $(OBJS_LL) $(OBJS_PROTO)
