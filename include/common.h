/*
 * This file is part of OpenCorsairLink.
 * Copyright (C) 2017,2018  Sean Nelson <audiohacked@gmail.com>

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

#ifndef _COMMON_H
#define _COMMON_H

#define STR_HELPER( x ) #x
#define STR( x ) STR_HELPER( x )

#define MAJOR 0 // Full Rewrite
#define MINOR 9 // Major API Change
#define PATCH_MAJOR 0 // Backward-compatible API Change
#define PATCH_MINOR 0 // Patch/Fix Change

#define VERSION "v" STR( MAJOR ) "." STR( MINOR ) "." STR( PATCH_MAJOR ) "." STR( PATCH_MINOR )

#endif /* _COMMON_H */
