#
# This file is part of OpenCorsairLink.
# Copyright (C) 2014  Sean Nelson <audiohacked@gmail.com>
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
# along with Foobar.  If not, see <http://www.gnu.org/licenses/>.
#

CC = g++
CFLAGS = -Wall -g
LDFLAGS = -lhidapi-hidraw

all: OpenCorsairLink

%.o: %.cpp
	$(CC) -o $@ -c $< $(CFLAGS)

OpenCorsairLink: main.o options.o CorsairLink.o CorsairFan.o CorsairLed.o CorsairTemp.o
	$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS)

clean:
	rm -rf *.o OpenCorsairLink

