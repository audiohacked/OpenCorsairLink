/*
 * This file is part of OpenCorsairLink.
 * Copyright (C) 2017  Sean Nelson <audiohacked@gmail.com>

 * OpenCorsairLink is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * any later version.

 * OpenCorsairLink is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with OpenCorsairLink.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <libusb.h>
#include "print.h"
#include <stdarg.h>


int verbose = MSG_INFO;

int print(enum msglevel level, const char *fmt, ...)
{
	va_list ap;
	int ret = 0;
	FILE *output_type = stdout;
	
	if (level < MSG_INFO)
		output_type = stderr;
	
	if (level <= verbose) {
		va_start(ap, fmt);
		ret = vfprintf(output_type, fmt, ap);
		va_end(ap);
		if (level != MSG_SPEW)
			fflush(output_type);
	}
	return 0;
}

int dump_packet(uint8_t *packet, int size)
{
	msg_debug2("\n---- Packet dump: -----------------------------");	
	for (int8_t j = 0; j < size; j++)
	{
		if(j % 16 == 0) msg_debug2("\n");
		msg_debug2("%02x ",packet[j]);
	}
	msg_debug2("\n-----------------------------------------------\n");

	return 0;
}