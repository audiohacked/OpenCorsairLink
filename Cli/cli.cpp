#include <unistd.h>
#include <string.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <errno.h>
#include <getopt.h>

#include "Proto.h"
#include "Link.h"
#include "Fan.h"
#include "Led.h"
#include "Temperature.h"

int parseArguments(int argc, char **argv, int &info, 
	int &fanNumber, int &fanHighNumber, int &fanMode, int &fanPWM, int &fanRPM, int &fanThreshold,
	int &ledNumber, int &ledMode, CorsairLed::CorsairLedColor *leds,
	int &tempNumber);

CorsairLink *cl = new CorsairLink();
CorsairFan *f = new CorsairFan();
CorsairLed *l = new CorsairLed();
CorsairTemp *t = new CorsairTemp();

int main(int argc, char **argv) {
	//fprintf(stdout, "Open Corsair Link\n");

	int info = 0;
	int fanNumber = -1, fanHighNumber = 0, fanMode = 0, fanPWM = 0, fanRPM = 0, fanThreshold = 0;
	int ledNumber = 0, ledMode = -1;
	int tempNumber = 0;

	if(parseArguments(argc, argv, info,
	   fanNumber, fanHighNumber, fanMode, fanPWM, fanRPM, fanThreshold,
	   ledNumber, ledMode, l->color,
	   tempNumber)) {
		return 1;
	}

	if(!cl->Initialize()) {
		fprintf(stdout, "Cannot initialize link.\n");
		delete cl;
		return 1;
	}

	if (info == 1) {
		int i = 0;
		char prod[9] = "";
		cl->GetProductName(prod);
		fprintf(stdout, "%s: Firmware v%02X\n\n", prod, cl->GetFirmwareVersion());
		if (ledNumber > 0) {
			fprintf(stdout, "Number of Controllable LEDs: %i\n", l->GetLedCount());
			fprintf(stdout, "LED Mode: %02X\n", l->GetMode());
			l->GetColor(&l->color[0]);
			fprintf(stdout, "LED Color:\n\tRed: %i\n\tGreen: %i\n\tBlue: %i\n", l->color[0].red, l->color[0].green, l->color[0].blue );
		}
		if (tempNumber > 0) {
			fprintf(stdout, "Number of Temperature Sensors: %i\n", t->GetTempSensors());
			t->SelectSensor(tempNumber - 1);
			fprintf(stdout, "Temperature: %.2f C\n", (float)t->GetTemp()/256);
		}
		if (fanNumber > -1) {
			for(i = fanNumber-1 ; i<fanHighNumber; i++) {
				f->ReadFanInfo(i, &f->fanInfo[i]);
			}
			for(i = fanNumber-1 ; i<fanHighNumber; i++) {
				f->PrintInfo(f->fanInfo[i]);
			}
		}
	}
	else {
		if(fanNumber > -1) {
			if(fanMode != 0 || fanRPM != 0) {
				if(fanMode == FixedRPM && fanRPM <= 0) {
					fprintf(stderr, "Fan RMP missing for Fixed RPM fan mode.\n");
					return 1;
				}
				else {
					CorsairFan::CorsairFanInfo newFanInfo;
					//fan->SelectFan(fanNumber - 1);
					if(fanMode != 0) {
						fprintf(stdout, "Setting fan to mode %s\n", CorsairFan::GetFanModeString(fanMode));
						newFanInfo.Mode = fanMode;
					}
					if(fanRPM != 0) {
						fprintf(stdout, "Setting fan RPM to %i\n", fanRPM);
						newFanInfo.RPM = fanRPM;
					}
					f->SetFansInfo(fanNumber - 1, newFanInfo);
				}
			} else {
				fprintf(stdout, "No mode or fan RPM specified for the fan.\n");
				return 1;
			}
		}
		else if(fanMode != 0 || fanRPM != 0) {
				fprintf(stderr, "Cannot set fan to a specific mode or fixed RPM without specifying the fan number\n");
				return 1;
		}

		if(ledNumber != 0) {
			l->SelectLed(ledNumber - 1);
			fprintf(stdout, "Setting LED\n");
			if (ledMode > -1) {
				l->SetMode(ledMode);
			}
			int current_mode = l->GetMode();
			//fprintf(stdout, "DEBUG Current LED Mode: %02x\n", current_mode);
			if ((current_mode == 0x0C) && (l->color_set_by_opts == 3)) {
				l->Set_TempMode_Color(l->color);
			}
			if ((current_mode != 0x0C)&&(l->color_set_by_opts > 0)) {
				//fprintf(stdout, "DEBUG Setting Led Colorset; colors defined: %i\n", l->color_set_by_opts);
				l->SetLedCycleColors(l->color);
			}
		}
	}

	if(cl != NULL) {
		delete cl;
	}
	if(f != NULL) {
		delete f;
	}
	if(l != NULL) {
		delete l;
	}
	if(t != NULL) {
		delete t;
	}
	return 0;
} 

extern CorsairLed *l;

static struct option long_options[] = {
	{"help",              no_argument,          0, 'h'},
	{"info",              no_argument,          0, 'i'},
	{"print",             no_argument,          0, 'p'},

	{"led",               required_argument,    0, 'l'},
	{"led-mode",          required_argument, NULL,  10},
	{"rgb1",              required_argument, NULL,  11},
	{"rgb2",              required_argument, NULL,  12},
	{"rgb3",              required_argument, NULL,  13},
	{"rgb4",              required_argument, NULL,  14},

	{"temperature",       required_argument,    0, 't'},
	{"temperature-limit", required_argument, NULL,  20},

	{"fan",               required_argument,    0, 'f'},
	{"fan-mode",          required_argument, NULL,  30},
	{"fan-pwm",           required_argument, NULL,  31},
	{"fan-rpm",           required_argument, NULL,  32},
	{"fan-external",      required_argument, NULL,  33},
	{"fan-warning",       required_argument, NULL,  34},
	{"fan-table-rpm",     required_argument, NULL,  35},
	{"fan-table-temp",    required_argument, NULL,  36},

	{0, 0, 0, 0}
};

void printHelp() {
	fprintf(stdout, "OpenCorsairLink [options]\n");
	fprintf(stdout, "Options:\n");
	fprintf(stdout, "\t-h, --help :Prints this Message\n");
	fprintf(stdout, "\t-i, --info :Displays information about the Fans, Pumps, or LEDs.\n");
	fprintf(stdout, "\t-p, --print :Displays all information about the Fans, Pumps, and LEDs.\n");

	fprintf(stdout, "\t-l, --led <led number> :Selects a LED to setup. LEDs are numbered 1 & up.\n");
	fprintf(stdout, "\t\t--led-mode <led mode> :Sets LED Mode (in HH format).\n");
	fprintf(stdout, "\t\t\tModes:\n");
	fprintf(stdout, "\t\t\t\t 0x00 - Static Color\n");
	fprintf(stdout, "\t\t\t\t 0x40 - 2-Color Cycle (requries to specify RGB1 & RGB2)\n");
	fprintf(stdout, "\t\t\t\t 0x80 - 4-Color Cycle (requires to specify RGB1, RGB2, RGB3, & RGB4)\n");
	fprintf(stdout, "\t\t\t\t 0xC0 - Temperature Mode (requires to specify RGB1, RGB2, & RGB3)\n");
	fprintf(stdout, "\t\t\t\t\tLow Nibble controls Cycle Speed or Temperature Channel\n");
	fprintf(stdout, "\t\t\t\t\t(0 = internal sensor; 7 = manual)\n");
	fprintf(stdout, "\t\t--rgb1 <HTML Color Code> :Define Color for LEDs\n");
	fprintf(stdout, "\t\t--rgb2 <HTML Color Code> :Define Color for LEDs\n");
	fprintf(stdout, "\t\t--rgb3 <HTML Color Code> :Define Color for LEDs\n");
	fprintf(stdout, "\t\t--rgb4 <HTML Color Code> :Define Color for LEDs\n");

	fprintf(stdout, "\t-t, --temperature <sensor number> :Selects a Temperature Sensor.\n");
	fprintf(stdout, "\t\t--temperature-limit <temp limit> :Sets Max Temperature (high temp warning).\n");

	fprintf(stdout, "\t-f, --fan <fan number>{:<fan number>} :Selects a fan to setup. Accepted values are 1, 2, 3 or 4.\n");
	fprintf(stdout, "\t\t--fan-mode <fan mode> :Sets the mode for the selected fan\n");
	fprintf(stdout, "\t\t\tModes:\n");
	fprintf(stdout, "\t\t\t\t 2 - Fixed PWM (requires to sepcify the PWM)\n");
	fprintf(stdout, "\t\t\t\t 4 - Fixed RPM (requires to specify the RPM)\n");
	fprintf(stdout, "\t\t\t\t 6 - Default\n");
	fprintf(stdout, "\t\t\t\t 8 - Quiet\n");
	fprintf(stdout, "\t\t\t\t10 - Balanced\n");
	fprintf(stdout, "\t\t\t\t12 - Performance\n");
	fprintf(stdout, "\t\t--fan-pwm <fan PWM> :The desired PWM speed for the selected fan. NOTE: it only works when fan mode is set to Fixed PWM\n");
	fprintf(stdout, "\t\t--fan-rpm <fan RPM> :The desired RPM for the selected fan. NOTE: it works only when fan mode is set to Fixed RPM\n");
	fprintf(stdout, "\t\t--fan-warning <fan threshold> :Sets the Fan Warning Limit (Sets Underspeed Threshold).\n");
}

int parseArguments(int argc, char **argv, int &info, 
	int &fanNumber, int &fanHighNumber, int &fanMode, int &fanPWM, int &fanRPM, int &fanThreshold,
	int &ledNumber, int &ledMode, CorsairLed::CorsairLedColor *leds,
	int &tempNumber)
{
	int c, returnCode = 0;
	char sep;
	while (1) {
		int option_index = 0;

		c = getopt_long (argc, argv, "hil:t:f:", long_options, &option_index);
		if (c == -1 || returnCode != 0)
			break;
		switch (c) {
		case 'l':// led select
			ledNumber = strtol(optarg, NULL, 10);
			if(ledNumber < 1) {
				fprintf(stderr, "Led number is invalid.");
				returnCode = 1;
			}
			break;
		case 10://led-mode
			sscanf(optarg, "%2x", &ledMode);
			break;
		case 11://rpg1
			sscanf(optarg, "%2x%2x%2x", &leds[0].red, &leds[0].green, &leds[0].blue);
			l->color_set_by_opts++;
			break;
		case 12://rpg2
			sscanf(optarg, "%2x%2x%2x", &leds[1].red, &leds[1].green, &leds[1].blue);
			l->color_set_by_opts++;
			break;
		case 13://rpg3
			sscanf(optarg, "%2x%2x%2x", &leds[2].red, &leds[2].green, &leds[2].blue);
			l->color_set_by_opts++;
			break;
		case 14://rpg4
			sscanf(optarg, "%2x%2x%2x", &leds[3].red, &leds[3].green, &leds[3].blue);
			l->color_set_by_opts++;
			break;
		case 'f'://fan select
			errno = 0;
			sscanf(optarg, "%i%c%i", &fanNumber, &sep, &fanHighNumber);
			if (strncmp(&sep,":",1)!=0) {
				fanHighNumber=fanNumber;
			}
			if(fanNumber < 1 || fanNumber > 5){
				fprintf(stderr, "Fan number is invalid. Accepted values are 1, 2, 3 or 4.");
				returnCode = 1;
			}
			break;

		case 30://fan-mode
			errno = 0;
			fanMode = strtol(optarg, NULL, 10);
			if(fanMode != Performance && fanMode != FixedRPM &&
				fanMode != Default && fanMode != Balanced && fanMode != Quiet){

				fprintf(stderr, "Fan mode is not allowed. Accepted values are:\n");
				fprintf(stderr, "\t 4 - Fixed RPM\n");
				fprintf(stderr, "\t 6 - Default\n");
				fprintf(stderr, "\t 8 - Quiet\n");
				fprintf(stderr, "\t10 - Balanced\n");
				fprintf(stderr, "\t12 - Performance\n");
				returnCode = 1;
			}
			break;
		case 31://fan-pwm
			errno = 0;
			fanPWM = strtol(optarg, NULL, 10);
			if(fanPWM < 0){
				fprintf(stderr, "Fan PWM cannot be a negative value.\n");
				returnCode = 1;
			}
			break;
		case 32://fan-rpm
			errno = 0;
			fanRPM = strtol(optarg, NULL, 10);
			if(fanRPM < 0){
				fprintf(stderr, "Fan RPM cannot be a negative value.\n");
				returnCode = 1;
			}
			break;
		case 34://fan-warning
			errno = 0;
			break;	
		case 'i'://info
			info = 1;
			break;
		case 'p'://print all info
			info = 1;
			ledNumber = 1;
			tempNumber = 1;
			fanNumber = 1;
			fanHighNumber = 5;
			break;
		case 'h'://help
			printHelp();
			exit(0);
			break;
		default:
			exit(1);
			returnCode = 0;
		}
	}
	return returnCode;
}
