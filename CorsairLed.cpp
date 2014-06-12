#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "CorsairLinkProto.h"
#include "CorsairLink.h"
#include "CorsairLed.h"

extern CorsairLink *cl;

#define DEBUG 0

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
	cl->buf[0] = 0x03; // Length
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
#if 0
	int i = 0;
	for (i = 0; i < 10; i++)
	{
		fprintf(stdout, "Debug: %i\n", cl->buf[i]);
	}
#endif
	return cl->buf[2];
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

#if 0
	int i = 0;
	for (i = 0; i < 10; i++)
	{
		fprintf(stdout, "Debug: %i\n", cl->buf[i]);
	}
#endif
	return cl->buf[4];
}

int CorsairLed::GetColor(int ledIndex, CorsairLedColor *led)
{
	memset(cl->buf,0x00,sizeof(cl->buf));
	// Read fan Mode
	cl->buf[0] = 0x08; // Length
	cl->buf[1] = cl->CommandId++; // Command ID
	cl->buf[2] = WriteOneByte; // Command Opcode
	cl->buf[3] = LED_SelectCurrent; // Command data...
	cl->buf[4] = ledIndex;
	cl->buf[5] = cl->CommandId++; // Command ID
	cl->buf[6] = ReadThreeBytes; // Command Opcode
	cl->buf[7] = LED_CurrentColor; // Command data...
	cl->buf[8] = 3; // Register Length

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
#if DEBUG
	int i = 0;
	for (i = 0; i < 10; i++)
	{
		fprintf(stdout, "Debug: %i\n", cl->buf[i]);
	}
#endif
	led->red = cl->buf[5];
#if DEBUG
	fprintf(stdout, "Debug: Red: %i\n", led->red);
#endif
	led->green = cl->buf[6];
#if DEBUG
	fprintf(stdout, "Debug: Green: %i\n", led->green);
#endif
	led->blue = cl->buf[7];
#if DEBUG
	fprintf(stdout, "Debug: Blue: %i\n", led->blue);
#endif

	return 0;
}

int CorsairLed::GetTempControlledMode(int ledIndex)
{
	memset(cl->buf,0x00,sizeof(cl->buf));
	// Read fan Mode
	cl->buf[0] = 0x07; // Length
	cl->buf[1] = cl->CommandId++; // Command ID
	cl->buf[2] = WriteOneByte; // Command Opcode
	cl->buf[3] = LED_SelectCurrent; // Command data...
	cl->buf[4] = ledIndex;
	cl->buf[5] = cl->CommandId++; // Command ID
	cl->buf[6] = ReadTwoBytes; // Command Opcode
	cl->buf[7] = LED_TemperatureColor; // Command data...

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

int CorsairLed::Get_TempMode_Temp(int ledIndex)
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
	cl->buf[7] = LED_TemperatureMode; // Command data...
	cl->buf[8] = 6;

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

	//cl->buf[5];

	return 0;
}

int CorsairLed::Get_TempMode_Color(int ledIndex)
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
	cl->buf[7] = LED_TemperatureModeColors; // Command data...
	cl->buf[8] = 9;

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

int CorsairLed::GetLedCycleColors(int ledIndex)
{
	memset(cl->buf,0x00,sizeof(cl->buf));
	// Read fan Mode
	cl->buf[0] = 0x08; // Length
	cl->buf[1] = cl->CommandId++; // Command ID
	cl->buf[2] = WriteOneByte; // Command Opcode
	cl->buf[3] = LED_SelectCurrent; // Command data...
	cl->buf[4] = ledIndex;
	cl->buf[5] = cl->CommandId++; // Command ID
	cl->buf[6] = ReadThreeBytes; // Command Opcode
	cl->buf[7] = LED_CycleColors; // Command data...
	cl->buf[8] = 12; // Number of Bytes that follow

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

int CorsairLed::SetMode(int ledIndex, int mode)
{
	memset(cl->buf,0x00,sizeof(cl->buf));
	// Read fan Mode
	cl->buf[0] = 0x07; // Length
	cl->buf[1] = cl->CommandId++; // Command ID
	cl->buf[2] = WriteOneByte; // Command Opcode
	cl->buf[3] = LED_SelectCurrent; // Command data...
	cl->buf[4] = ledIndex;
	cl->buf[5] = cl->CommandId++; // Command ID
	cl->buf[6] = WriteOneByte; // Command Opcode
	cl->buf[7] = LED_Mode; // Command data...

	cl->buf[8] = mode;

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

#if 0
int CorsairLed::SetColor(int ledIndex, int red, int green, int blue)
{
	memset(cl->buf,0x00,sizeof(cl->buf));
	// Read fan Mode
	cl->buf[0] = 0x0B; // Length
	cl->buf[1] = cl->CommandId++; // Command ID
	cl->buf[2] = WriteOneByte; // Command Opcode
	cl->buf[3] = LED_SelectCurrent; // Command data...
	cl->buf[4] = ledIndex;
	cl->buf[5] = cl->CommandId++; // Command ID
	cl->buf[6] = WriteThreeBytes; // Command Opcode
	cl->buf[7] = LED_CurrentColor; // Command data...
	cl->buf[8] = 3; // Command Data Length
	cl->buf[9] = red;
	cl->buf[10] = green;
	cl->buf[11] = blue;

	int res = hid_write(cl->handle, cl->buf, 16);
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
#endif

int CorsairLed::SetTempControlledMode(int ledIndex)
{
	memset(cl->buf,0x00,sizeof(cl->buf));
	// Read fan Mode
	cl->buf[0] = 0x0A; // Length
	cl->buf[1] = cl->CommandId++; // Command ID
	cl->buf[2] = WriteOneByte; // Command Opcode
	cl->buf[3] = LED_SelectCurrent; // Command data...
	cl->buf[4] = ledIndex;
	cl->buf[5] = cl->CommandId++; // Command ID
	cl->buf[6] = WriteTwoBytes; // Command Opcode
	cl->buf[7] = LED_TemperatureMode; // Command data...

	//cl->buf[8] = byte1;
	//cl->buf[9] = byte2;

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

int CorsairLed::Set_TempMode_Temp(int ledIndex, int temp1, int temp2, int temp3)
{
	memset(cl->buf,0x00,sizeof(cl->buf));
	// Read fan Mode
	cl->buf[0] = 0x0D; // Length
	cl->buf[1] = cl->CommandId++; // Command ID
	cl->buf[2] = WriteOneByte; // Command Opcode
	cl->buf[3] = LED_SelectCurrent; // Command data...
	cl->buf[4] = ledIndex;

	cl->buf[5] = cl->CommandId++; // Command ID
	cl->buf[6] = WriteThreeBytes; // Command Opcode
	cl->buf[7] = LED_TemperatureMode; // Command data...
	cl->buf[8] = 6;

	cl->buf[9] = temp1 & 0x00ff;
	cl->buf[10] = temp1 >> 8;

	cl->buf[11] = temp2 & 0x00ff;
	cl->buf[12] = temp2 >> 8;

	cl->buf[13] = temp3 & 0x00ff;
	cl->buf[14] = temp3 >> 8;


	int res = hid_write(cl->handle, cl->buf, 16);
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

int CorsairLed::Set_TempMode_Color(int ledIndex, CorsairLedColor colorLeds[])
{
	memset(cl->buf,0x00,sizeof(cl->buf));
	// Read fan Mode
	cl->buf[0] = 0x10; // Length

	cl->buf[1] = cl->CommandId++; // Command ID
	cl->buf[2] = WriteOneByte; // Command Opcode
	cl->buf[3] = LED_SelectCurrent; // Command data...
	cl->buf[4] = ledIndex;

	cl->buf[5] = cl->CommandId++; // Command ID
	cl->buf[6] = WriteThreeBytes; // Command Opcode
	cl->buf[7] = LED_TemperatureModeColors; // Command data...
	cl->buf[8] = 9;

	cl->buf[ 9] = colorLeds[0].red;
	cl->buf[10] = colorLeds[0].green;
	cl->buf[11] = colorLeds[0].blue;

	cl->buf[12] = colorLeds[1].red;
	cl->buf[13] = colorLeds[1].green;
	cl->buf[14] = colorLeds[1].blue;

	cl->buf[15] = colorLeds[2].red;
	cl->buf[16] = colorLeds[2].green;
	cl->buf[17] = colorLeds[2].blue;

	int res = hid_write(cl->handle, cl->buf, 20);
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

int CorsairLed::SetLedCycleColors(int ledIndex, CorsairLedColor *leds)
{
	memset(cl->buf,0x00,sizeof(cl->buf));
	// Read fan Mode
	cl->buf[0] = 0x14; // Length
	cl->buf[1] = cl->CommandId++; // Command ID
	cl->buf[2] = WriteOneByte; // Command Opcode
	cl->buf[3] = LED_SelectCurrent; // Command data...
	cl->buf[4] = ledIndex;
	cl->buf[5] = cl->CommandId++; // Command ID
	cl->buf[6] = WriteThreeBytes; // Command Opcode
	cl->buf[7] = LED_CycleColors; // Command data...
	cl->buf[8] = 0x0C;

	cl->buf[9] = leds[0].red;
	cl->buf[10] = leds[0].green;
	cl->buf[11] = leds[0].blue;

	cl->buf[12] = leds[1].red;
	cl->buf[13] = leds[1].green;
	cl->buf[14] = leds[1].blue;

	cl->buf[15] = leds[2].red;
	cl->buf[16] = leds[2].green;
	cl->buf[17] = leds[2].blue;

	cl->buf[18] = leds[3].red;
	cl->buf[19] = leds[3].green;
	cl->buf[20] = leds[3].blue;

	int res = hid_write(cl->handle, cl->buf, 24);
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
