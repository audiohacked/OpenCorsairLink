#
# This file is part of OpenCorsairLink.
# Copyright (C) 2017,2018  Sean Nelson <audiohacked@gmail.com>
#
# OpenCorsairLink is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 2 of the License, or
# any later version.
#
# OpenCorsairLink is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with OpenCorsairLink.  If not, see <http://www.gnu.org/licenses/>.
#

CC ?= gcc
CFLAGS ?= -std=c99 -pedantic-errors
LDFLAGS ?= -lm

# pkg-config for libusb-1.0
CFLAGS += $(shell pkg-config --cflags libusb-1.0)
LDFLAGS += $(shell pkg-config --libs libusb-1.0)

OBJS = \
	main.o \
	commander.o \
	device.o \
	driver.o \
	hydro.o \
	options.o \
	print.o \
	psu.o \
	scan.o

OBJS_LL = \
	lowlevel/asetek.o \
	lowlevel/commanderpro.o \
	lowlevel/hid.o \
	lowlevel/rmi.o

OBJS_PROTO = \
	protocol/asetek/core.o \
	protocol/asetek/fan.o \
	protocol/asetek/led.o \
	protocol/asetek/pump.o \
	protocol/asetek/temperature.o \
	protocol/commanderpro/core.o \
	protocol/commanderpro/fan.o \
	protocol/commanderpro/power.o \
	protocol/commanderpro/temperature.o \
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
