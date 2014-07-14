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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Proto.h"
#include "Link.h"
#include "Fan.h"

extern CorsairLink *cl;

int CorsairFan::SelectFan(int index) {
	memset(cl->buf, 0x00, sizeof(cl->buf));
	cl->buf[0] = 0x04;
	cl->buf[1] = cl->CommandId++;
	cl->buf[2] = WriteOneByte;
	cl->buf[3] = FAN_Select;
	cl->buf[4] = index;

	cl->hid_wrapper(cl->handle, cl->buf, 8);
	return cl->buf[2];
}

int CorsairFan::CurrentFan() {
	memset(cl->buf, 0x00, sizeof(cl->buf));
	cl->buf[0] = 0x03;
	cl->buf[1] = cl->CommandId++;
	cl->buf[2] = ReadOneByte;
	cl->buf[3] = FAN_Select;

	cl->hid_wrapper(cl->handle, cl->buf, 8);
	return cl->buf[2];
}

int CorsairFan::GetFanCount() {
	memset(cl->buf, 0x00, sizeof(cl->buf));
	cl->buf[0] = 0x03;
	cl->buf[1] = cl->CommandId++;
	cl->buf[2] = WriteOneByte;
	cl->buf[3] = FAN_Count;
	cl->buf[4] = 0x00;

	cl->hid_wrapper(cl->handle, cl->buf, 8);
	return cl->buf[2];
}

int CorsairFan::GetFanMode() {
	memset(cl->buf, 0x00, sizeof(cl->buf));
	cl->buf[0] = 0x03;
	cl->buf[1] = cl->CommandId++;
	cl->buf[2] = ReadOneByte;
	cl->buf[3] = FAN_Mode;
	cl->buf[4] = 0x00;

	cl->hid_wrapper(cl->handle, cl->buf, 8);
	return cl->buf[2];
}

int CorsairFan::SetFanMode(int mode) {
	memset(cl->buf, 0x00, sizeof(cl->buf));
	cl->buf[0] = 0x03;
	cl->buf[1] = cl->CommandId++;
	cl->buf[2] = WriteOneByte;
	cl->buf[3] = FAN_Mode;
	cl->buf[4] = mode;
	cl->hid_wrapper(cl->handle, cl->buf, 8);
	return 0;
}

int CorsairFan::GetFanPWM() {
	//ReadOneByte
	memset(cl->buf, 0x00, sizeof(cl->buf));
	cl->buf[0] = 0x03;
	cl->buf[1] = cl->CommandId++;
	cl->buf[2] = ReadOneByte;
	cl->buf[3] = FAN_FixedPWM;
	cl->hid_wrapper(cl->handle, cl->buf, 8);
	return cl->buf[2];
}

int CorsairFan::SetFanPWM(int pwm) {
	//WriteOneByte
	memset(cl->buf, 0x00, sizeof(cl->buf));
	cl->buf[0] = 0x04;
	cl->buf[1] = cl->CommandId++;
	cl->buf[2] = WriteOneByte;
	cl->buf[3] = FAN_FixedPWM;
	cl->buf[4] = pwm;
	cl->hid_wrapper(cl->handle, cl->buf, 8);
	return 0;
}

int CorsairFan::GetFanRPM() {
	//ReadTwoBytes
	memset(cl->buf, 0x00, sizeof(cl->buf));
	cl->buf[0] = 0x04;
	cl->buf[1] = cl->CommandId++;
	cl->buf[2] = ReadTwoBytes;
	cl->buf[3] = FAN_FixedRPM;
	cl->buf[4] = 0x00;
	cl->hid_wrapper(cl->handle, cl->buf, 8);
	int RPM = cl->buf[3]<<8;
	RPM += cl->buf[2];
	return RPM;
}

int CorsairFan::SetFanRPM(int rpm) {
	//WriteTwoBytes
	memset(cl->buf, 0x00, sizeof(cl->buf));
	cl->buf[0] = 0x05;
	cl->buf[1] = cl->CommandId++;
	cl->buf[2] = WriteTwoBytes;
	cl->buf[3] = FAN_FixedRPM;
	cl->buf[4] = rpm & 0x00ff;
	cl->buf[5] = rpm>>8;
	cl->hid_wrapper(cl->handle, cl->buf, 8);
	return 0;
}

int CorsairFan::GetExternalReport() {
	//ReadTwoBytes
	return -1;
}

int CorsairFan::SetExternalReport() {
	//WriteTwoBytes
	return -1;
}

int CorsairFan::GetCurrentRPM() {
	//ReadTwoBytes
	memset(cl->buf, 0x00, sizeof(cl->buf));	
	cl->buf[0] = 0x03;
	cl->buf[1] = cl->CommandId++;
	cl->buf[2] = ReadTwoBytes;
	cl->buf[3] = FAN_ReadRPM;
	cl->hid_wrapper(cl->handle, cl->buf, 8);
	int RPM = cl->buf[3]<<8;
	RPM += cl->buf[2];
	return RPM;
}

int CorsairFan::GetMaxRPM() {
	//ReadTwoBytes
	memset(cl->buf, 0x00, sizeof(cl->buf));
	cl->buf[0] = 0x03;
	cl->buf[1] = cl->CommandId++;
	cl->buf[2] = ReadTwoBytes;
	cl->buf[3] = FAN_MaxRecordedRPM;
	cl->hid_wrapper(cl->handle, cl->buf, 8);
	int MaxRPM = cl->buf[3]<<8;
	MaxRPM += cl->buf[2];
	return MaxRPM;
}

int CorsairFan::GetFanUnderspeedThreshold() {
	//ReadTwoBytes
	memset(cl->buf, 0x00, sizeof(cl->buf));
	cl->buf[0] = 0x03;
	cl->buf[1] = cl->CommandId++;
	cl->buf[2] = ReadTwoBytes;
	cl->buf[3] = FAN_UnderSpeedThreshold;
	cl->hid_wrapper(cl->handle, cl->buf, 8);
	int threshold = cl->buf[3]<<8;
	threshold += cl->buf[2];
	return 0;
}

int CorsairFan::SetFanUnderspeedThreshold(int threshold) {
	//WriteTwoBytes
	memset(cl->buf, 0x00, sizeof(cl->buf));
	cl->buf[0] = 0x05;
	cl->buf[1] = cl->CommandId++;
	cl->buf[2] = WriteTwoBytes;
	cl->buf[3] = FAN_UnderSpeedThreshold;
	cl->buf[4] = threshold & 0x00FF;
	cl->buf[5] = threshold >> 8;
	return 0;
}

int CorsairFan::GetRPMTable(int &rpm1, int &rpm2, int &rpm3, int &rpm4, int &rpm5) {
	memset(cl->buf, 0x00, sizeof(cl->buf));
	cl->buf[0] = 0x0F;
	cl->buf[1] = cl->CommandId++;
	cl->buf[2] = ReadThreeBytes;
	cl->buf[3] = FAN_RPMTable;
	cl->buf[4] = 0x0A;
	cl->hid_wrapper(cl->handle, cl->buf, 24);

	rpm1 = cl->buf[4]<<8;
	rpm1 += cl->buf[3];

	rpm2 = cl->buf[6]<<8;
	rpm2 += cl->buf[5];

	rpm3 = cl->buf[8]<<8;
	rpm3 += cl->buf[7];

	rpm4 = cl->buf[10]<<8;
	rpm4 += cl->buf[9];

	rpm5 = cl->buf[12]<<8;
	rpm5 += cl->buf[11];

	return 0;
}

int CorsairFan::SetRPMTable(int rpm1, int rpm2, int rpm3, int rpm4, int rpm5) {
	memset(cl->buf, 0x00, sizeof(cl->buf));	
	cl->buf[0] = 0x0F;
	cl->buf[1] = cl->CommandId++;
	cl->buf[2] = WriteThreeBytes;
	cl->buf[3] = FAN_RPMTable;
	cl->buf[4] = 0x0A;

	cl->buf[5] = rpm1 & 0x00FF;
	cl->buf[6] = rpm1>>8;

	cl->buf[7] = rpm2 & 0x00FF;
	cl->buf[8] = rpm2>>8;

	cl->buf[9] = rpm3 & 0x00FF;
	cl->buf[10] = rpm3>>8;

	cl->buf[11] = rpm4 & 0x00FF;
	cl->buf[12] = rpm4>>8;

	cl->buf[13] = rpm5 & 0x00FF;
	cl->buf[14] = rpm5>>8;

	cl->hid_wrapper(cl->handle, cl->buf, 24);

	return 0;
}

int CorsairFan::GetTemperatureTable(int &temp1, int &temp2, int &temp3, int &temp4, int &temp5) {
	memset(cl->buf, 0x00, sizeof(cl->buf));
	cl->buf[0] = 0x0F;
	cl->buf[1] = cl->CommandId++;
	cl->buf[2] = ReadThreeBytes;
	cl->buf[3] = FAN_TempTable;
	cl->buf[4] = 0x0A;
	cl->hid_wrapper(cl->handle, cl->buf, 24);

	temp1 = cl->buf[4]<<8;
	temp1 += cl->buf[3];

	temp2 = cl->buf[6]<<8;
	temp2 += cl->buf[5];

	temp3 = cl->buf[8]<<8;
	temp3 += cl->buf[7];

	temp4 = cl->buf[10]<<8;
	temp4 += cl->buf[9];

	temp5 = cl->buf[12]<<8;
	temp5 += cl->buf[11];

	return 0;
}

int CorsairFan::SetTemperatureTable(int temp1, int temp2, int temp3, int temp4, int temp5) {
	memset(cl->buf, 0x00, sizeof(cl->buf));	
	cl->buf[0] = 0x0F;
	cl->buf[1] = cl->CommandId++;
	cl->buf[2] = WriteThreeBytes;
	cl->buf[3] = FAN_TempTable;
	cl->buf[4] = 0x0A;

	cl->buf[5] = temp1 & 0x00FF;
	cl->buf[6] = temp1>>8;

	cl->buf[7] = temp2 & 0x00FF;
	cl->buf[8] = temp2>>8;

	cl->buf[9] = temp3 & 0x00FF;
	cl->buf[10] = temp3>>8;

	cl->buf[11] = temp4 & 0x00FF;
	cl->buf[12] = temp4>>8;

	cl->buf[13] = temp5 & 0x00FF;
	cl->buf[14] = temp5>>8;

	cl->hid_wrapper(cl->handle, cl->buf, 24);

	return 0;
}

void CorsairFan::PrintInfo(CorsairFanInfo fan) {
	fprintf(stdout, "%s:\n", fan.Name );
	fprintf(stdout, "\tMode: %s\n", GetFanModeString(fan.Mode) );
	fprintf(stdout, "\tRPM: %i\n", fan.RPM );
}

char* CorsairFan::GetFanModeString(int mode)
{
	char *modeString = NULL;
	switch(mode){
		case FixedPWM:
			asprintf(&modeString,"Fixed PWM");
			break;
		case FixedRPM:
			asprintf(&modeString, "Fixed RPM");
			break;
		case Default:
			asprintf(&modeString, "Default");
			break;
		case Quiet:
			asprintf(&modeString, "Quiet");
			break;
		case Balanced:
			asprintf(&modeString, "Balanced");
			break;
		case Performance:
			asprintf(&modeString, "Performance");
			break;
		case Custom:
			asprintf(&modeString, "Custom");
			break;
		default:
			asprintf(&modeString, "N/A (%02X)", mode );
			break;
	}
	return modeString;
}

int CorsairFan::ConnectedFans() {
	int fans = 0, i = 0, fanMode = 0;

	for (i = 0; i < 5; i++) {
		memset(cl->buf,0x00,sizeof(cl->buf));
		// Read fan Mode
		cl->buf[0] = 0x07; // Length
		cl->buf[1] = cl->CommandId++; // Command ID
		cl->buf[2] = WriteOneByte; // Command Opcode
		cl->buf[3] = FAN_Select; // Command data...
		cl->buf[4] = i; // select fan
		cl->buf[5] = cl->CommandId++; // Command ID
		cl->buf[6] = ReadOneByte; // Command Opcode
		cl->buf[7] = FAN_Mode; // Command data...

		int res = hid_write(cl->handle, cl->buf, 11);
		if (res < 0) {
			fprintf(stderr, "Error: Unable to write() %s\n", (char*)hid_error(cl->handle) );
		}

		res = cl->hid_read_wrapper(cl->handle, cl->buf);
		if (res < 0) {
			fprintf(stderr, "Error: Unable to read() %s\n", (char*)hid_error(cl->handle) );
		}
		fanMode = cl->buf[4];

		if(fanMode != 0x03){
			fans++;
		}
	}

	return fans;
}

void CorsairFan::ReadFanInfo(int fanIndex, CorsairFanInfo *fan){
	int res = 0;

	if(fanIndex < 4){
		asprintf(&fan->Name, "Fan %i", fanIndex + 1);
	}
	else {
		asprintf(&fan->Name, "Pump");
	}

	memset(cl->buf,0x00,sizeof(cl->buf));
	// Read fan Mode
	cl->buf[0] = 0x07; // Length
	cl->buf[1] = cl->CommandId++; // Command ID
	cl->buf[2] = WriteOneByte; // Command Opcode
	cl->buf[3] = FAN_Select; // Command data...
	cl->buf[4] = fanIndex; // select fan
	cl->buf[5] = cl->CommandId++; // Command ID
	cl->buf[6] = ReadOneByte; // Command Opcode
	cl->buf[7] = FAN_Mode; // Command data...
	
	res = hid_write(cl->handle, cl->buf, 11);
	if (res < 0) {
		fprintf(stderr, "Error: Unable to write() %s\n", (char*)hid_error(cl->handle) );
	}

	res = cl->hid_read_wrapper(cl->handle, cl->buf);
	if (res < 0) {
		fprintf(stderr, "Error: Unable to read() %s\n", (char*)hid_error(cl->handle) );
	}
	fan->Mode = cl->buf[4] & 0x0E;

	memset(cl->buf,0x00,sizeof(cl->buf));
	// Read fan RPM
	cl->buf[0] = 0x07; // Length
	cl->buf[1] = cl->CommandId++; // Command ID
	cl->buf[2] = WriteOneByte; // Command Opcode
	cl->buf[3] = FAN_Select; // Command data...
	cl->buf[4] = fanIndex; // select fan
	cl->buf[5] = cl->CommandId++; // Command ID
	cl->buf[6] = ReadTwoBytes; // Command Opcode
	cl->buf[7] = FAN_ReadRPM; // Command data...

	res = hid_write(cl->handle, cl->buf, 11);
	if (res < 0) {
		fprintf(stderr, "Error: Unable to write() %s\n", (char*)hid_error(cl->handle) );
	}

	res = cl->hid_read_wrapper(cl->handle, cl->buf);
	//All data is little-endian.
	int rpm = cl->buf[5] << 8;
	rpm += cl->buf[4];

	fan->RPM = rpm;
}

int CorsairFan::SetFansInfo(int fanIndex, CorsairFanInfo fanInfo){
	memset(cl->buf,0x00,sizeof(cl->buf));

	if(fanInfo.Mode == FixedPWM || fanInfo.Mode == FixedRPM
		|| fanInfo.Mode == Default || fanInfo.Mode == Quiet
		|| fanInfo.Mode == Balanced	|| fanInfo.Mode == Performance
		|| fanInfo.Mode == Custom) {

		cl->buf[0] = 0x0b; // Length
		cl->buf[1] = cl->CommandId++; // Command ID
		cl->buf[2] = WriteOneByte; // Command Opcode
		cl->buf[3] = FAN_Select; // Command data...
		cl->buf[4] = fanIndex; // select fan
		cl->buf[5] = cl->CommandId++; // Command ID
		cl->buf[6] = WriteOneByte; // Command Opcode
		cl->buf[7] = FAN_Mode; // Command data...
		cl->buf[8] = fanInfo.Mode;
		cl->buf[9] = cl->CommandId++; // Command ID
		cl->buf[10] = ReadOneByte; // Command Opcode
		cl->buf[11] = FAN_Mode; // Command data...

		int res = hid_write(cl->handle, cl->buf, 17);
		if (res < 0) {
			fprintf(stderr, "%s", (char*)hid_error(cl->handle) );
			return 1;
		}

		res = cl->hid_read_wrapper(cl->handle, cl->buf);
		if (res < 0) {
			fprintf(stderr, "%s", (char*)hid_error(cl->handle) );
			return 1;
		}
		if(fanInfo.Mode != cl->buf[6]){
			fprintf(stderr, "Cannot set fan mode.");
			return 1;
		}
	} else {
		fprintf(stderr, "Invalid fan mode." );
		return 1;
	}
	if(fanInfo.RPM != 0) {
		memset(cl->buf,0x00,sizeof(cl->buf));

		cl->buf[0] = 0x0b; // Length
		cl->buf[1] = cl->CommandId++; // Command ID
		cl->buf[2] = WriteOneByte; // Command Opcode
		cl->buf[3] = FAN_Select; // Command data...
		cl->buf[4] = fanIndex; // select fan
		cl->buf[5] = cl->CommandId++; // Command ID
		cl->buf[6] = WriteTwoBytes; // Command Opcode
		cl->buf[7] = FAN_FixedRPM; // Command data...
		cl->buf[8] = fanInfo.RPM & 0x00FF;
		cl->buf[9] = fanInfo.RPM >> 8;
		cl->buf[10] = cl->CommandId++; // Command ID
		cl->buf[11] = ReadTwoBytes; // Command Opcode
		cl->buf[12] = FAN_ReadRPM; // Command data...

		int res = hid_write(cl->handle, cl->buf, 18);
		if (res < 0) {
			fprintf(stderr, "%s", (char*)hid_error(cl->handle) );
			return 1;
		}

		res = cl->hid_read_wrapper(cl->handle, cl->buf);
		if (res < 0) {
			fprintf(stderr, "%s", (char*)hid_error(cl->handle) );
			return 1;
		}
		//All data is little-endian.
		int rpm = cl->buf[5] << 8;
		rpm += cl->buf[4];
		if(fanInfo.RPM != rpm){
			fprintf(stderr, "Cannot set fan RPM.");
			return 1;
		}
	}

	return 0;
}

