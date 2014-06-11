#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "CorsairLinkProto.h"
#include "CorsairLink.h"
#include "CorsairLed.h"

extern CorsairLink *cl;

CorsairLed::CorsairLed()
{
}
	
CorsairLed::~CorsairLed()
{
}

int CorsairLed::GetLedCount()
{
	memset(cl->buf,0x00,sizeof(cl->buf));
	// Read fan Mode
	cl->buf[0] = 0x06; // Length
	cl->buf[1] = cl->CommandId++; // Command ID
	cl->buf[2] = ReadOneByte; // Command Opcode
	cl->buf[3] = LED_Count; // Command data...

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
	return cl->buf[4];
}

int CorsairLed::GetMode(int ledIndex)
{
	memset(cl->buf,0x00,sizeof(cl->buf));
	// Read fan Mode
	cl->buf[0] = 0x07; // Length
	cl->buf[1] = cl->CommandId++; // Command ID
	cl->buf[2] = WriteOneByte; // Command Opcode
	cl->buf[3] = LED_SelectCurrent; // Command data...
	cl->buf[4] = ledIndex;
	cl->buf[5] = cl->CommandId++; // Command ID
	cl->buf[6] = ReadOneByte; // Command Opcode
	cl->buf[7] = LED_Mode; // Command data...

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
	return cl->buf[4];
}

int CorsairLed::GetColor(int ledIndex, CorsairLedColor led)
{
	memset(cl->buf,0x00,sizeof(cl->buf));
	// Read fan Mode
	cl->buf[0] = 0x07; // Length
	cl->buf[1] = cl->CommandId++; // Command ID
	cl->buf[2] = WriteOneByte; // Command Opcode
	cl->buf[3] = LED_SelectCurrent; // Command data...
	cl->buf[4] = ledIndex;
	cl->buf[5] = cl->CommandId++; // Command ID
	cl->buf[6] = ReadThreeBytes; // Command Opcode
	cl->buf[7] = LED_CurrentColor; // Command data...

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
	led.red = cl->buf[4];
	led.green = cl->buf[5];
	led.blue = cl->buf[6];

	return 0;
}

/*int GetTempControlMode(int ledIndex);
int Get_TempMode_Temp(int ledIndex);
int Get_TempMode_Color(int ledIndex);
int GetLedCycleColors(int ledIndex);

int SetMode(int ledIndex, int mode);
int SetColor(int ledIndex, int red, int green, int blue);
int SetTempControlMode(int ledIndex);
int Set_TempMode_Temp(int ledIndex);
int Set_TempMode_Color(int ledIndex);
int SetLedCycleColors(int ledIndex);
*/