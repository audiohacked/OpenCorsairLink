#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>

#include "CorsairLinkProto.h"
#include "CorsairLink.h"
#include "CorsairFan.h"
#include "CorsairLed.h"
#include "CorsairTemp.h"

extern CorsairLed *l;

static struct option long_options[] = {
	{"help",  no_argument, 0, 'h'},
	{"fan", required_argument, 0, 'f'},
	{"mode", required_argument, 0, 'm'},
	{"rpm",  required_argument, 0, 'r'},
	{"led", required_argument, 0, 'l'},

	{"led_mode", required_argument, NULL, 0},
	{"rgb1", required_argument, NULL, 1},
	{"rgb2", required_argument, NULL, 2},
	{"rgb3", required_argument, NULL, 3},
	{"rgb4", required_argument, NULL, 4},

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

int parseArguments(int argc, char **argv, 
	int &fanNumber, int &fanMode, int &fanRPM,
	int &ledNumber, int &ledMode, CorsairLed::CorsairLedColor *leds)
{
	int c, returnCode = 0;
	while (1) {
		int option_index = 0;

		c = getopt_long (argc, argv, "f:m:r:l:h", long_options, &option_index);
		if (c == -1 || returnCode != 0)
			break;
		switch (c) {
		case 'l':
			ledNumber = strtol(optarg, NULL, 10);
			if(ledNumber < 1) {
				fprintf(stderr, "Led number is invalid.");
				returnCode = 1;
			}
			break;
		case 0:
			ledMode = strtol(optarg, NULL, 10);
			break;
		case 1:
			sscanf(optarg, "%2x%2x%2x", &leds[0].red, &leds[0].green, &leds[0].blue);
			l->color_set_by_opts++;
			break;
		case 2:
			sscanf(optarg, "%2x%2x%2x", &leds[1].red, &leds[1].green, &leds[1].blue);
			l->color_set_by_opts++;
			break;
		case 3:
			sscanf(optarg, "%2x%2x%2x", &leds[2].red, &leds[2].green, &leds[2].blue);
			l->color_set_by_opts++;
			break;
		case 4:
			sscanf(optarg, "%2x%2x%2x", &leds[3].red, &leds[3].green, &leds[3].blue);
			l->color_set_by_opts++;
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
		default:
			exit(1);
			returnCode = 0;
		}
	}
	return returnCode;
}
