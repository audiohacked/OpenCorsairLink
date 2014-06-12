#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "CorsairLinkProto.h"
#include "CorsairLink.h"
#include "CorsairLed.h"

extern CorsairLink *cl;

#define DEBUG 0

int CorsairLed::SelectLed(int ledIndex)
{
	memset(cl->buf,0x00,sizeof(cl->buf));
	// Read fan Mode
	cl->buf[0] = 0x04; // Length
	cl->buf[1] = cl->CommandId++; // Command ID
	cl->buf[2] = WriteOneByte; // Command Opcode
	cl->buf[3] = LED_SelectCurrent; // Command data...
	cl->buf[4] = ledIndex;

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
	return cl->buf[2];
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
	return cl->buf[2];
}

int CorsairLed::GetMode()
{
	memset(cl->buf,0x00,sizeof(cl->buf));
	// Read fan Mode
	cl->buf[0] = 0x03; // Length
	cl->buf[1] = cl->CommandId++; // Command ID
	cl->buf[2] = ReadOneByte; // Command Opcode
	cl->buf[3] = LED_Mode; // Command data...

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

	return cl->buf[2];
}

int CorsairLed::GetColor(CorsairLedColor *led)
{
	memset(cl->buf,0x00,sizeof(cl->buf));
	// Read fan Mode
	cl->buf[0] = 0x04; // Length
	cl->buf[1] = cl->CommandId++; // Command ID
	cl->buf[2] = ReadThreeBytes; // Command Opcode
	cl->buf[3] = LED_CurrentColor; // Command data...
	cl->buf[4] = 3; // Register Length

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
	for (i = 0; i < 6; i++)
	{
		fprintf(stdout, "Debug: %i\n", cl->buf[i]);
	}
#endif
	led->red = cl->buf[3];
#if DEBUG
	fprintf(stdout, "Debug: Red: %i\n", led->red);
#endif
	led->green = cl->buf[4];
#if DEBUG
	fprintf(stdout, "Debug: Green: %i\n", led->green);
#endif
	led->blue = cl->buf[5];
#if DEBUG
	fprintf(stdout, "Debug: Blue: %i\n", led->blue);
#endif
	this->color_set_by_func = 1;

	return 0;
}

int CorsairLed::GetTempColor(CorsairLedColor *led)
{
	memset(cl->buf,0x00,sizeof(cl->buf));
	// Read fan Mode
	cl->buf[0] = 0x04; // Length
	cl->buf[1] = cl->CommandId++; // Command ID
	cl->buf[2] = ReadThreeBytes; // Command Opcode
	cl->buf[3] = LED_TemperatureColor; // Command data...
	cl->buf[4] = 3;

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
	for (i = 0; i < 6; i++)
	{
		fprintf(stdout, "Debug: %i\n", cl->buf[i]);
	}
#endif
	led->red = cl->buf[3];
#if DEBUG
	fprintf(stdout, "Debug: Red: %i\n", led->red);
#endif
	led->green = cl->buf[4];
#if DEBUG
	fprintf(stdout, "Debug: Green: %i\n", led->green);
#endif
	led->blue = cl->buf[5];
#if DEBUG
	fprintf(stdout, "Debug: Blue: %i\n", led->blue);
#endif

	this->color_set_by_func = 1;

	return 0;

}

int CorsairLed::Get_TempMode_Temp(int &temp1, int &temp2, int &temp3)
{
	memset(cl->buf,0x00,sizeof(cl->buf));
	// Read fan Mode
	cl->buf[0] = 0x04; // Length
	cl->buf[1] = cl->CommandId++; // Command ID
	cl->buf[2] = ReadThreeBytes; // Command Opcode
	cl->buf[3] = LED_TemperatureMode; // Command data...
	cl->buf[4] = 6;

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

	temp1 = cl->buf[5] << 8;
	temp1 += cl->buf[4];
	
	temp2 = cl->buf[7] << 8;
	temp2 += cl->buf[6];

	temp3 = cl->buf[9] << 8;
	temp3 += cl->buf[8];

	return 0;
}

int CorsairLed::Get_TempMode_Color(CorsairLedColor *led)
{
	memset(cl->buf,0x00,sizeof(cl->buf));
	// Read fan Mode
	cl->buf[0] = 0x04; // Length
	cl->buf[1] = cl->CommandId++; // Command ID
	cl->buf[2] = ReadThreeBytes; // Command Opcode
	cl->buf[3] = LED_TemperatureModeColors; // Command data...
	cl->buf[4] = 9;

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
	
	led[0].red = cl->buf[4];
	led[0].green = cl->buf[5];
	led[0].blue = cl->buf[6];

	led[1].red = cl->buf[7];
	led[1].green = cl->buf[8];
	led[1].blue = cl->buf[9];

	led[2].red = cl->buf[10];
	led[2].green = cl->buf[11];
	led[2].blue = cl->buf[12];

	this->color_set_by_func = 3;

	return 0;
}

int CorsairLed::GetLedCycleColors(CorsairLedColor *leds)
{
	memset(cl->buf,0x00,sizeof(cl->buf));
	// Read fan Mode
	cl->buf[0] = 0x04; // Length
	cl->buf[1] = cl->CommandId++; // Command ID
	cl->buf[2] = ReadThreeBytes; // Command Opcode
	cl->buf[3] = LED_CycleColors; // Command data...
	cl->buf[4] = 12; // Number of Bytes that follow

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

	leds[0].red = cl->buf[4];
	leds[0].green = cl->buf[5];
	leds[0].blue = cl->buf[6];

	leds[1].red = cl->buf[7];
	leds[1].green = cl->buf[8];
	leds[1].blue = cl->buf[9];

	leds[2].red = cl->buf[10];
	leds[2].green = cl->buf[11];
	leds[2].blue = cl->buf[12];

	leds[3].red = cl->buf[13];
	leds[3].green = cl->buf[14];
	leds[3].blue = cl->buf[15];

	this->color_set_by_func = 4;

	return 0;
}

int CorsairLed::SetMode(int mode)
{
	memset(cl->buf,0x00,sizeof(cl->buf));
	// Read fan Mode
	cl->buf[0] = 0x04; // Length
	cl->buf[1] = cl->CommandId++; // Command ID
	cl->buf[2] = WriteOneByte; // Command Opcode
	cl->buf[3] = LED_Mode; // Command data...

	cl->buf[4] = mode;

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

	return cl->buf[2];
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

int CorsairLed::SetTempColor(CorsairLedColor *led)
{
	if (this->color_set_by_opts < 1) {
		fprintf(stderr, "Error: You did not define the Color\n");
		return -1;
	}
	memset(cl->buf,0x00,sizeof(cl->buf));
	// Read fan Mode
	cl->buf[0] = 0x07; // Length
	cl->buf[1] = cl->CommandId++; // Command ID
	cl->buf[2] = WriteThreeBytes; // Command Opcode
	cl->buf[3] = LED_TemperatureMode; // Command data...
	cl->buf[4] = 3;
	
	cl->buf[5] = led[0].red;
	cl->buf[6] = led[0].green;
	cl->buf[7] = led[0].blue;
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

int CorsairLed::Set_TempMode_Temp(int temp1, int temp2, int temp3)
{
	memset(cl->buf,0x00,sizeof(cl->buf));
	// Read fan Mode
	cl->buf[0] = 0x0D; // Length

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

int CorsairLed::Set_TempMode_Color(CorsairLedColor *leds)
{
	if (this->color_set_by_opts < 3) {
		fprintf(stderr, "Error: You did not define the Colors, or the correct number of Colors\n");
		return -1;
	}
	memset(cl->buf,0x00,sizeof(cl->buf));
	// Read fan Mode
	cl->buf[0] = 0x11; // Length

	cl->buf[5] = cl->CommandId++; // Command ID
	cl->buf[6] = WriteThreeBytes; // Command Opcode
	cl->buf[7] = LED_TemperatureModeColors; // Command data...
	cl->buf[8] = 9;

	cl->buf[ 9] = leds[0].red;
	cl->buf[10] = leds[0].green;
	cl->buf[11] = leds[0].blue;

	cl->buf[12] = leds[1].red;
	cl->buf[13] = leds[1].green;
	cl->buf[14] = leds[1].blue;

	cl->buf[15] = leds[2].red;
	cl->buf[16] = leds[2].green;
	cl->buf[17] = leds[2].blue;

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

int CorsairLed::SetLedCycleColors(CorsairLedColor *leds)
{
	//if (this->color_set_by_opts < 3) {
	//	fprintf(stderr, "Error: You did not define the Colors\n");
	//	return -1;
	//}
	memset(cl->buf,0x00,sizeof(cl->buf));
	// Read fan Mode
	cl->buf[0] = 0x10; // Length
	cl->buf[1] = cl->CommandId++; // Command ID
	cl->buf[2] = WriteThreeBytes; // Command Opcode
	cl->buf[3] = LED_CycleColors; // Command data...
	cl->buf[4] = 0x0C;

	cl->buf[5] = leds[0].red;
	cl->buf[6] = leds[0].green;
	cl->buf[7] = leds[0].blue;

	cl->buf[8] = leds[1].red;
	cl->buf[9] = leds[1].green;
	cl->buf[10] = leds[1].blue;

	cl->buf[11] = leds[2].red;
	cl->buf[12] = leds[2].green;
	cl->buf[13] = leds[2].blue;

	cl->buf[14] = leds[3].red;
	cl->buf[15] = leds[3].green;
	cl->buf[16] = leds[3].blue;

#if DEBUG
	int i = 0;
	for (i = 0; i < 18; i++)
	{
		fprintf(stdout, "Debug: %i\n", cl->buf[i]);
	}
#endif

	int res = hid_write(cl->handle, cl->buf, 18);
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
