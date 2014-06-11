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
	cl->buf[6] = ReadSixBytes; // Command Opcode
	cl->buf[7] = LED_TemperatureMode; // Command data...

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
	cl->buf[6] = ReadNineBytes; // Command Opcode
	cl->buf[7] = LED_TemperatureModeColors; // Command data...

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
	cl->buf[0] = 0x07; // Length
	cl->buf[1] = cl->CommandId++; // Command ID
	cl->buf[2] = WriteOneByte; // Command Opcode
	cl->buf[3] = LED_SelectCurrent; // Command data...
	cl->buf[4] = ledIndex;
	cl->buf[5] = cl->CommandId++; // Command ID
	cl->buf[6] = ReadTwelveBytes; // Command Opcode
	cl->buf[7] = LED_CycleColors; // Command data...

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
	cl->buf[6] = ReadNineBytes; // Command Opcode
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

	return 0;
}

int CorsairLed::SetColor(int ledIndex, int red, int green, int blue)
{
	memset(cl->buf,0x00,sizeof(cl->buf));
	// Read fan Mode
	cl->buf[0] = 0x0A; // Length
	cl->buf[1] = cl->CommandId++; // Command ID
	cl->buf[2] = WriteOneByte; // Command Opcode
	cl->buf[3] = LED_SelectCurrent; // Command data...
	cl->buf[4] = ledIndex;
	cl->buf[5] = cl->CommandId++; // Command ID
	cl->buf[6] = WriteThreeBytes; // Command Opcode
	cl->buf[7] = LED_CurrentColor; // Command data...
	cl->buf[8] = red;
	cl->buf[9] = green;
	cl->buf[10] = blue;

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
	cl->buf[6] = WriteSixBytes; // Command Opcode
	cl->buf[7] = LED_TemperatureMode; // Command data...

	cl->buf[8] = temp1 & 0x00ff;
	cl->buf[9] = temp1 ;

	cl->buf[10] = temp2 & 0x00ff;
	cl->buf[11] = temp2 ;

	cl->buf[12] = temp3 & 0x00ff;
	cl->buf[13] = temp3 ;


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
	cl->buf[6] = WriteNineBytes; // Command Opcode
	cl->buf[7] = LED_TemperatureModeColors; // Command data...

	cl->buf[ 8] = colorLeds[0].red;
	cl->buf[ 9] = colorLeds[0].green;
	cl->buf[10] = colorLeds[0].blue;

	cl->buf[11] = colorLeds[1].red;
	cl->buf[12] = colorLeds[1].green;
	cl->buf[13] = colorLeds[1].blue;

	cl->buf[14] = colorLeds[2].red;
	cl->buf[15] = colorLeds[2].green;
	cl->buf[16] = colorLeds[2].blue;

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

int CorsairLed::SetLedCycleColors(int ledIndex)
{
	memset(cl->buf,0x00,sizeof(cl->buf));
	// Read fan Mode
	cl->buf[0] = 0x0A; // Length
	cl->buf[1] = cl->CommandId++; // Command ID
	cl->buf[2] = WriteOneByte; // Command Opcode
	cl->buf[3] = LED_SelectCurrent; // Command data...
	cl->buf[4] = ledIndex;
	cl->buf[5] = cl->CommandId++; // Command ID
	cl->buf[6] = WriteTwelveBytes; // Command Opcode
	cl->buf[7] = LED_CycleColors; // Command data...


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
