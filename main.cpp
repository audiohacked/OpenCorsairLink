#include <unistd.h>
#include <string.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <getopt.h>

#include "CorsairLinkProto.h"
#include "CorsairLink.h"
#include "CorsairFan.h"
#include "CorsairLed.h"
#include "CorsairTemp.h"

CorsairLink *cl = new CorsairLink();
CorsairFan *fans = new CorsairFan();
CorsairLed *leds = new CorsairLed();
CorsairTemp *temp = new CorsairTemp();

static struct option long_options[] = {
	{"help",  no_argument, 0, 'h'},
	{"fan", required_argument, 0, 'f'},
	{"mode", required_argument, 0, 'm'},
	{"rpm",  required_argument, 0, 'r'},
	{"led", required_argument, 0, 'l'},
	{"led_mode", required_argument, 0, -1},
	{"led_red", required_argument, 0, -1},
	{"led_green", required_argument, 0, -1},
	{"led_blue", required_argument, 0, -1},
	{0, 0, 0, 0}
};

void printHelp() {
	fprintf(stdout, "OpenCorsairLink [options]\n");
	fprintf(stdout, "Options:\n");
	fprintf(stdout, "\t-f, --fan <fan number> Selects a fan to setup.\n");
	fprintf(stdout, "\t\tAccepted values are 1, 2, 3 or 4.\n");
	fprintf(stdout, "\t\t5 is H100i Pump.\n");
	fprintf(stdout, "\t-m, --mode <mode> Sets the mode for the selected fan\n");
	fprintf(stdout, "\t\tModes:\n");
	fprintf(stdout, "\t\t\t 4 - Fixed RPM (requires to specify the RPM)\n");
	fprintf(stdout, "\t\t\t 6 - Default\n");
	fprintf(stdout, "\t\t\t 8 - Quiet\n");
	fprintf(stdout, "\t\t\t10 - Balanced\n");
	fprintf(stdout, "\t\t\t12 - Performance\n");
	fprintf(stdout, "\t-r, --rpm <fan RPM> The desired RPM for the selected fan. NOTE: it works only when fan mode is set to Fixed RPM\n");
	fprintf(stdout, "\t-h, --help Prints this message\n");
	fprintf(stdout, "\nNot specifying any option will display information about the fans and pump\n\n");
}

int parseArguments(int argc, char **argv, int &fanNumber, int &fanMode, int &fanRPM, int &ledNumber, int &ledMode, int &ledRed, int &ledGreen, int &ledBlue) {
	int c, returnCode = 0;;
	while (1) {
		int option_index = 0;

		c = getopt_long (argc, argv, "f:m:r:l:h", long_options, &option_index);
		if (c == -1 || returnCode != 0)
			break;
		switch (c) {
		case 0:
		case -1:
			/* If this option set a flag, do nothing else now. */
			if (long_options[option_index].flag != 0) break;
			printf ("option %s", long_options[option_index].name);
			if (optarg)
				printf (" with arg %s", optarg);
			printf ("\n");
			break;
		case 'f':
			errno = 0;
			fanNumber = strtol(optarg, NULL, 10);
			if(fanNumber < 1 || fanNumber > 5){
				fprintf(stderr, "Fan number is invalid. Accepted values are 1, 2, 3 or 4.");
				returnCode = 1;
			}
			break;

		case 'm':
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

		case 'r':
			errno = 0;
			fanRPM = strtol(optarg, NULL, 10);
			if(fanRPM < 0){
				fprintf(stderr, "Fan RPM cannot be a negative value.\n");
				returnCode = 1;
			}
			break;

		case 'h':
			printHelp();
			exit(0);
			break;
		case 'l':
			ledNumber = strtol(optarg, NULL, 10);
			break;
		default:
			exit(1);
			returnCode = 0;
		}
	}
	return returnCode;
}

int main(int argc, char **argv) {
	fprintf(stdout, "Open Corsair Link\n");
	int fanNumber = 0, fanMode = 0, fanRPM = 0;
	int ledNumber = 0, ledMode = 0, ledRed = 0, ledGreen = 0, ledBlue = 0;
	if(parseArguments(argc, argv, fanNumber, fanMode, fanRPM, ledNumber, ledMode, ledRed, ledGreen, ledBlue))
	{
		return 1;
	}

	if(!cl->Initialize()) {
		fprintf(stdout, "Cannot initialize link.\n");
		delete cl;
		return 1;
	}

	if(fanNumber != 0) {
		if(fanMode != 0 || fanRPM != 0) {
			if(fanMode == FixedRPM && fanRPM <= 0) {
				fprintf(stderr, "Fan RMP missing for Fixed RPM fan mode.\n");
				return 1;
			}
			else {
				CorsairFan::CorsairFanInfo newFanInfo;
				if(fanMode != 0) {
					fprintf(stdout, "Setting fan to mode %s\n", CorsairFan::GetFanModeString(fanMode));
					newFanInfo.Mode = fanMode;
				}
				if(fanRPM != 0) {
					fprintf(stdout, "Setting fan RPM to %i\n", fanRPM);
					newFanInfo.RPM = fanRPM;
				}
				fans->SetFansInfo(fanNumber - 1, newFanInfo);
			}
		} else {
			fprintf(stdout, "No mode or fan RPM specified for the fan.\n");
			return 1;
		}
	}
	else if(ledNumber > 0) {
		fprintf(stdout, "Set LED Color\n");
		leds->SetMode(ledNumber - 1, 0);
		leds->SetColor(ledNumber - 1, 0, 0, 255);
	}
	else if(fanMode != 0 || fanRPM != 0) {
			fprintf(stderr, "Cannot set fan to a specific mode or fixed RPM without specifying the fan number\n");
			return 1;
	}
	else {
		int i = 0;

		fprintf(stdout, "Number of Controllable LEDs: %i\n", leds->GetLedCount());
		fprintf(stdout, "LED Mode: %02X\n", leds->GetMode(0));
		leds->GetColor(0, &leds->color);
		fprintf(stdout, "LED Color:\n\tRed: %i\n\tGreen: %i\n\tBlue: %i\n", leds->color.red, leds->color.green, leds->color.blue );
		fprintf(stdout, "Number of Temperature Sensors: %i\n", temp->GetTempSensors());
		fprintf(stdout, "Temperature: %.2f C\n", temp->GetTemp(0)/256);

		for(i = 0 ; i< 5; i++) {
			fans->ReadFanInfo(i, &fans->fanInfo[i]);
			fans->PrintInfo(fans->fanInfo[i]);
		}
	}

	if(cl != NULL) {
		delete cl;
	}
	return 0;
} 
