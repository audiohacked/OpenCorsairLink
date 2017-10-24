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

#ifndef _PRINT_H
#define _PRINT_H

enum msglevel {
	MSG_ERROR	= 0,
	MSG_WARN	= 1,
	MSG_INFO	= 2,
	MSG_DEBUG	= 3,
	MSG_DEBUG2	= 4,
	MSG_SPEW	= 5,
};

extern int verbose;

int print(enum msglevel level, const char *fmt, ...) __attribute__((format(printf, 2, 3)));
int dump_packet(uint8_t *packet,int size);

#define msg_err(...)	print(MSG_ERROR, __VA_ARGS__)
#define msg_warn(...)	print(MSG_WARN, __VA_ARGS__)
#define msg_info(...)	print(MSG_INFO, __VA_ARGS__)
#define msg_debug(...)	print(MSG_DEBUG, __VA_ARGS__)
#define msg_debug2(...)	print(MSG_DEBUG2, __VA_ARGS__)
#define msg_spew(...)	print(MSG_SPEW, __VA_ARGS__)

#endif
