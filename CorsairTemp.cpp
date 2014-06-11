#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "CorsairLinkProto.h"
#include "CorsairLink.h"
#include "CorsairTemp.h"

extern CorsairLink *cl;

CorsairTemp::CorsairTemp()
{
}

CorsairTemp::~CorsairTemp()
{
}

int CorsairTemp::GetTempSensors()
{//1
	memset(cl->buf,0x00,sizeof(cl->buf));
	// Read fan Mode
	cl->buf[0] = 0x03; // Length
	cl->buf[1] = cl->CommandId++; // Command ID
	cl->buf[2] = ReadOneByte; // Command Opcode
	cl->buf[3] = TEMP_CountSensors; // Command data...

	int res = hid_write(cl->handle, cl->buf, 11);
	if (res < 0) {
		fprintf(stderr, "Error: Unable to write() %s\n", (char*)hid_error(cl->handle) );
		//return -1;
	}

	res = cl->hid_read_wrapper(cl->handle, cl->buf);
	if (res < 0) {
		fprintf(stderr, "Error: Unable to read() %s\n", (char*)hid_error(cl->handle) );
		//return -1;
	}

	return 0;
}

int CorsairTemp::GetTemp(int index)
{//2
	memset(cl->buf,0x00,sizeof(cl->buf));
	// Read fan Mode
	cl->buf[0] = 0x07; // Length
	cl->buf[1] = cl->CommandId++; // Command ID
	cl->buf[2] = WriteOneByte; // Command Opcode
	cl->buf[3] = TEMP_SelectActiveSensor; // Command data...
	cl->buf[4] = index;

	cl->buf[5] = cl->CommandId++; // Command ID
	cl->buf[6] = ReadTwoBytes; // Command Opcode
	cl->buf[7] = TEMP_Read; // Command data...


	int res = hid_write(cl->handle, cl->buf, 11);
	if (res < 0) {
		fprintf(stderr, "Error: Unable to write() %s\n", (char*)hid_error(cl->handle) );
		//return -1;
	}

	res = cl->hid_read_wrapper(cl->handle, cl->buf);
	if (res < 0) {
		fprintf(stderr, "Error: Unable to read() %s\n", (char*)hid_error(cl->handle) );
		//return -1;
	}
	int temp = cl->buf[5]<<8;
	temp += cl->buf[4];
	return temp;
}

int CorsairTemp::GetTempLimit(int index)
{//2
	memset(cl->buf,0x00,sizeof(cl->buf));
	// Read fan Mode
	cl->buf[0] = 0x07; // Length
	cl->buf[1] = cl->CommandId++; // Command ID
	cl->buf[2] = WriteOneByte; // Command Opcode
	cl->buf[3] = TEMP_SelectActiveSensor; // Command data...
	cl->buf[4] = index;

	cl->buf[5] = cl->CommandId++; // Command ID
	cl->buf[6] = ReadTwoBytes; // Command Opcode
	cl->buf[7] = TEMP_Limit; // Command data...


	int res = hid_write(cl->handle, cl->buf, 11);
	if (res < 0) {
		fprintf(stderr, "Error: Unable to write() %s\n", (char*)hid_error(cl->handle) );
		//return -1;
	}

	res = cl->hid_read_wrapper(cl->handle, cl->buf);
	if (res < 0) {
		fprintf(stderr, "Error: Unable to read() %s\n", (char*)hid_error(cl->handle) );
		//return -1;
	}

	return 0;
}

int CorsairTemp::SetTempLimit(int index)
{//2
	memset(cl->buf,0x00,sizeof(cl->buf));
	// Read fan Mode
	cl->buf[0] = 0x07; // Length
	cl->buf[1] = cl->CommandId++; // Command ID
	cl->buf[2] = WriteOneByte; // Command Opcode
	cl->buf[3] = TEMP_SelectActiveSensor; // Command data...
	cl->buf[4] = index;

	cl->buf[5] = cl->CommandId++; // Command ID
	cl->buf[6] = WriteTwoBytes; // Command Opcode
	cl->buf[7] = TEMP_Limit; // Command data...


	int res = hid_write(cl->handle, cl->buf, 11);
	if (res < 0) {
		fprintf(stderr, "Error: Unable to write() %s\n", (char*)hid_error(cl->handle) );
		//return -1;
	}

	res = cl->hid_read_wrapper(cl->handle, cl->buf);
	if (res < 0) {
		fprintf(stderr, "Error: Unable to read() %s\n", (char*)hid_error(cl->handle) );
		//return -1;
	}

	return 0;
}
