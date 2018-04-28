/*
 * This file is part of OpenCorsairLink.
 * Copyright (C) 2017,2018  Sean Nelson <audiohacked@gmail.com>

 * OpenCorsairLink is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * any later version.

 * OpenCorsairLink is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with OpenCorsairLink.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <string.h>
#include "options.h"
#include "../print.h"

static struct option long_options[] = {
    {"help",              no_argument,          0,  0},
    {"version",           no_argument,          0,  1},
    {"debug",             no_argument,          0,  2},
    {"dump",              no_argument,          0,  3},
    {"machine",           no_argument,          0,  19},

    {"device",            required_argument,    0,  4},
    {"firmware",          no_argument,          0,  5},

    {"print-temperature", no_argument,          0,  6},
    {"print-fan-speed",   no_argument,          0,  7},

    {"led-mode",          required_argument,    0,  8},
    {"led-colors",        required_argument,    0,  9},
    {"led-warn",          required_argument, NULL,  10},
    {"led-temp",          required_argument, NULL,  11},

    {"fan",               required_argument, NULL,  12},
    {"fan-mode",          required_argument, NULL,  13},
    {"fan-pwm",           required_argument, NULL,  14},
    {"fan-rpm",           required_argument, NULL,  15},
    {"fan-temps",         required_argument, NULL,  16},
    {"fan-speeds",        required_argument, NULL,  17},

    {"pump-mode",         required_argument, NULL,  18},

    {0, 0, 0, 0}
};

void options_print(void);

#define INIT_WARNING_LED(xx) \
    xx.red = 0xFF; \
    xx.green = 0x00; \
    xx.blue = 0x00;

#define INIT_DEFAULT_LED(xx) \
    xx.red = 0xFF; \
    xx.green = 0xFF; \
    xx.blue = 0x00;

int options_parse(int argc, char **argv,
    struct option_flags *flags,
    int8_t *device_number,
    struct option_parse_return *settings)
{
    int opt, returnCode = 0, option_index = 0;

    memset(settings, 0, sizeof(struct option_parse_return));
    INIT_DEFAULT_LED(settings->led_color[0]);
    INIT_WARNING_LED(settings->warning_led);
    settings->warning_led_temp = 60;
    settings->pump_mode = DEFAULT;

    while ((opt = getopt_long(argc, argv, "", long_options, &option_index)) != EOF) {
        int option_index = 0;

        //cc = getopt_long (argc, argv, "", long_options, &option_index);

        //if (cc == -1 || returnCode != 0)
        //    break;

        switch (opt) {
        case 0:
            options_print();
            break;

        case 1: /* program version */
            msg_info("OpenCorsairLink Version: %s", VERSION);
            break;

        case 2:
            verbose++;
        case 3:
            verbose++;
            break;

        case 19:
            verbose = MSG_MACHINE;
            //machine = MSG_MACHINE;
            break;

        case 4:
            sscanf(optarg, "%hhd", device_number);
            break;

        case 5:
            flags->device_firmware = 1;
            break;

        case 6:
            flags->read_temperature = 1;
            break;

        case 7:
            flags->read_fan_speed = 1;
            break;

        case 8:
            flags->set_led = 1;
            sscanf(optarg, "%2s", &settings->led_mode);
            break;

        case 9: /* led color */
        {
            uint8_t ii = 0;
            char* token = strtok(optarg, ",");
            while( token != NULL )
            {
                msg_debug("Found Color %d: %s\n", ii, token);
                sscanf(token, "%02hhX%02hhX%02hhX,", &settings->led_color[ii].red,
                        &settings->led_color[ii].green, &settings->led_color[ii].blue);
                ++ii;
                token = strtok(NULL, ",");
            }
            break;
        }

        case 10: /* led warning color */
            sscanf(optarg, "%02hhX%02hhX%02hhX", &settings->warning_led.red, &settings->warning_led.green, &settings->warning_led.blue);
            break;

        case 11: /* led warning temperature */
            sscanf(optarg, "%hhd", &settings->warning_led_temp);
            break;

        case 12:
            sscanf(optarg, "%hhd", &settings->fan);
            break;

        case 13:
            sscanf(optarg, "%hhd", &settings->fan_mode);
            break;

        case 14:
            sscanf(optarg, "%hu", &settings->fan_data);
            break;

        case 15:
            sscanf(optarg, "%hu", &settings->fan_data);
            break;

        case 16:
            sscanf(optarg, "%hhd,%hhd,%hhd,%hhd,%hhd,%hhd",
                &settings->fan1.t1,
                &settings->fan1.t2,
                &settings->fan1.t3,
                &settings->fan1.t4,
                &settings->fan1.t5,
                &settings->fan1.t6);
            break;

        case 17:
            sscanf(optarg, "%hhd,%hhd,%hhd,%hhd,%hhd,%hhd",
                &settings->fan1.s1,
                &settings->fan1.s2,
                &settings->fan1.s3,
                &settings->fan1.s4,
                &settings->fan1.s5,
                &settings->fan1.s6);
            break;

        case 18:
            sscanf(optarg, "%hhu", &settings->pump_mode);
            break;

        default:
            options_print();
            exit(1);
            returnCode = 0;
        }
    }
    return returnCode;
}

void options_print() {
    msg_info("OpenCorsairLink [options]\n");

    msg_info("Options:\n");
    msg_info("\t--help\t\t\t\t:Prints this Message\n");
    msg_info("\t--version\t\t\t:Displays version.\n");
    msg_info("\t--debug\t\t\t\t:Displays enhanced Debug Messages.\n");
    msg_info("\t--dump\t\t\t\t:Implies --debug. Dump the raw data recieved from the device.\n");
    msg_info("\t--device <Device Number>\t:Select device.\n");

    msg_info("\n\tLED:\n");
    msg_info("\t--led-mode");
    msg_info("\t\tModes:\n");
    msg_info("\t\t 0 - Static\n");
    msg_info("\t\t 1 - Blink (Only Commander Pro and Asetek Pro)\n");
    msg_info("\t\t 2 - Color Pulse (Only Commander Pro and Asetek Pro)\n");
    msg_info("\t\t 3 - Color Shift (Only Commander Pro and Asetek Pro)\n");
    msg_info("\t\t 4 - Rainbow (Only Commander Pro and Asetek Pro)\n");
    msg_info("\t\t 5 - Temperature (Only Commander Pro and Asetek Pro)\n");
    msg_info("\t--led-colors <HTML Color Code>\t\t\t:Define Color for LED.\n");
    msg_info("\t--led-warn <HTML Color Code>\t\t:Define Color for Warning Temp.\n");
    msg_info("\t--led-temp <Temperature in Celsius>\t:Define Warning Temperature.\n");

    msg_info("\n\tFan:\n");
    msg_info("\t--fan <fan number> :Selects a fan to setup. Accepted values are 1, 2, 3 or 4.\n");
    msg_info("\t--fan-mode <fan mode> :Sets the mode for the selected fan\n");
    msg_info("\t\tModes:\n");
    msg_info("\t\t 0 - Fixed PWM (requires to specify the PWM)\n");
    msg_info("\t\t 1 - Fixed RPM (requires to specify the RPM)\n");
    msg_info("\t\t 2 - Default\n");
    msg_info("\t\t 3 - Quiet\n");
    msg_info("\t\t 4 - Balanced\n");
    msg_info("\t\t 5 - Performance\n");
    msg_info("\t\t 6 - Custom Curve\n");
    msg_info("\t--fan-pwm <fan PWM> :The desired PWM speed for the selected fan. NOTE: it only works when fan mode is set to Fixed PWM\n");
    msg_info("\t--fan-rpm <fan RPM> :The desired RPM for the selected fan. NOTE: it works only when fan mode is set to Fixed RPM\n");
    msg_info("\t--fan-temps <CSV of Temperatures>\t:Define Comma Separated Values of Temperatures for Fan.\n");
    msg_info("\t--fan-speeds <CSV of Speed Percentage>\t:Define Comma Separated Values of RPM Percentage for Fan.\n");

    msg_info("\n\tPump mode:\n");
    msg_info("\t--pump-mode <mode>\t:set to 3 for quiet, and 5 for performance\n");

    msg_info("\n Without options, OpenCorsairLink will show the status of any detected Corsair Link device.\n");
}

