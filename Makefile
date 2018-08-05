####################################################################################################
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
####################################################################################################

CC ?= gcc

# CFLAGS ?=
CFLAGS += -std=c99
# CFLAGS += -pedantic-errors
CFLAGS += -Iinclude
# CFLAGS += -D_XOPEN_SOURCE=500

LDFLAGS ?= -lm

# pkg-config for libusb-1.0
CFLAGS += $(shell pkg-config --cflags libusb-1.0)
LDFLAGS += $(shell pkg-config --libs libusb-1.0)

####################################################################################################

MAINLOGIC_SOURCE := \
	main.c \
	device.c \
	driver.c \
	print.c \
	logic/options.c \
	logic/options_fan.c \
	logic/options_led.c \
	logic/options_pump.c \
	logic/scan.c \
	logic/settings/commanderpro.c \
	logic/settings/hydro_asetek.c \
	logic/settings/hydro_asetekpro.c \
	logic/settings/hydro_coolit.c \
	logic/settings/psu.c

LOWLEVEL_SOURCE := \
	lowlevel/asetek.c \
	lowlevel/commanderpro.c \
	lowlevel/coolit.c \
	lowlevel/rmi.c

PROTOCOL_SOURCE := \
	protocol/asetek/core.c \
	protocol/asetek/fan.c \
	protocol/asetek/led.c \
	protocol/asetek/pump.c \
	protocol/asetek/temperature.c \
	protocol/asetekpro/core.c \
	protocol/asetekpro/fan.c \
	protocol/asetekpro/led.c \
	protocol/asetekpro/pump.c \
	protocol/asetekpro/temperature.c \
	protocol/commanderpro/core.c \
	protocol/commanderpro/fan.c \
	protocol/commanderpro/power.c \
	protocol/commanderpro/temperature.c \
	protocol/rmi/core.c \
	protocol/rmi/power.c \
	protocol/rmi/temperature.c \
	protocol/rmi/time.c \
	protocol/coolit/core.c \
	protocol/coolit/fan.c \
	protocol/coolit/led.c \
	protocol/coolit/pump.c \
	protocol/coolit/temperature.c

HEADER := $(shell find ./include -name '*.h')

EXECUTABLE := OpenCorsairLink.elf

####################################################################################################

OBJS := \
	${MAINLOGIC_SOURCE:.c=.o}

OBJS_LL := \
	${LOWLEVEL_SOURCE:.c=.o} 

OBJS_PROTO := \
	${PROTOCOL_SOURCE:.c=.o} 

.PHONY: all
all: $(EXECUTABLE)

.PHONY: default
default: all

.PHONY: rebuild
rebuild: clean all

.PHONY: style
style: $(MAINLOGIC_SOURCE) $(LOWLEVEL_SOURCE) $(PROTOCOL_SOURCE) $(HEADER)
	clang-format -style=file -i $^

.PHONY: tidy
tidy: $(MAINLOGIC_SOURCE) $(LOWLEVEL_SOURCE) $(PROTOCOL_SOURCE)
	clang-tidy $^ -checks=* -- -std=c++11

.PHONY: clean
clean:
	$(RM) $(EXECUTABLE) $(OBJS) $(OBJS_LL) $(OBJS_PROTO)

%.o: %.c
	$(CC) $(CFLAGS) -g -c -o $@ $<

$(EXECUTABLE): $(OBJS) $(OBJS_PROTO) $(OBJS_LL)
	$(CC) $^ $(CFLAGS) $(LDFLAGS) -o $@

####################################################################################################
