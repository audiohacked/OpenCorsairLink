/*
 * This file is part of OpenCorsairLink.
 * Copyright (C) 2014  Sean Nelson <audiohacked@gmail.com>

 * OpenCorsairLink is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * any later version.

 * OpenCorsairLink is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with Foobar.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef _CORSAIRTEMP_H
#define _CORSAIRTEMP_H
/*
* 0c 	RW 		1 byte 		Select active temperature sensor
* 0d 	R 		1 byte 		Number of temperature sensors
* 0e 	R 		2 bytes 	Temperature as measured by selected sensor
* 0f 	RW 		2 bytes 	Temperature limit (when the temperature goes over this, status is set to 0xff) 
*/
class CorsairTemp {
	public:
		CorsairTemp(){}
		~CorsairTemp(){}

		int SelectSensor(int);
		int CurrentSensor();
		int GetTempSensors();
		int GetTemp();
		int GetTempLimit();
		int SetTempLimit(int);
};

#endif
