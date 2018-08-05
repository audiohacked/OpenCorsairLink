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

#include "logic/options.h"

#include "common.h"
#include "print.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int
options_parse(
    int argc,
    char** argv,
    struct option_flags* flags,
    int8_t* device_number,
    struct option_parse_return* settings )
{
    int opt, returnCode = 0, option_index = 0;
    char *subopts, *value;

    memset( settings, 0, sizeof( struct option_parse_return ) );
    memset( flags, 0, sizeof( struct option_flags ) );

    // fan_control_init( &settings->fan_ctrl );
    // led_control_init( &settings->led_ctrl );
    // pump_control_init( &settings->pump_ctrl );

    while ( ( opt = getopt_long( argc, argv, "", long_options, &option_index ) ) != EOF )
    {
        switch ( opt )
        {
        case OPTION_VERSION: /* program version */
            msg_info( "OpenCorsairLink Version: %s", VERSION );
            break;

        case OPTION_DEBUG:
            verbose++;

        case OPTION_DUMP:
            verbose++;
            break;

        case OPTION_MACHINE:
            verbose = MSG_MACHINE;
            break;

        case OPTION_DEVICE:
            sscanf( optarg, "%hhd", device_number );
            break;

        case OPTION_FAN:
            flags->set_fan = 1;
            fan_suboptions_parse( optarg, &settings->fan_ctrl );
            break;

        case OPTION_LED:
            flags->set_led = 1;
            led_suboptions_parse( optarg, &settings->led_ctrl );
            break;

        case OPTION_PUMP:
            flags->set_pump = 1;
            pump_suboptions_parse( optarg, &settings->pump_ctrl );
            break;

        case OPTION_HELP:
        default:
            options_print();
            exit( 1 );
            returnCode = 0;
        }
    }
    return returnCode;
}

void
options_print( void )
{
    msg_info( "OpenCorsairLink [options]\n" );

    msg_info( "Options:\n" );
    msg_info( "\t--help\t\t\t\t:Prints this Message\n" );
    msg_info( "\t--version\t\t\t:Displays version.\n" );
    msg_info( "\t--debug\t\t\t\t:Displays enhanced Debug Messages.\n" );
    msg_info( "\t--dump\t\t\t\t:Implies --debug. Dump the raw data recieved from the device.\n" );
    msg_info( "\t--machine\t\t\t:Prints statuses in Machine Readable Format.\n" );
    msg_info( "\t--device <Device Number>\t:Select device.\n" );

    msg_info( "\n\tLED:\n" );
    msg_info( "\t--led " );
    msg_info( "channel=N," );
    msg_info( "mode=N," );
    msg_info( "colors=HHHHHH:HHHHHH:HHHHHH," );
    msg_info( "temp=TEMP:TEMP:TEMP" );
    msg_info( "\n" );
    msg_info( "\t\tChannel: <led number> :Selects a led channel to setup. "
              "Accepted values are 1 or 2.\n" );
    msg_info( "\t\tMode:\n" );
    msg_info( "\t\t\t 0 - Static\n" );
    msg_info( "\t\t\t 1 - Blink (Only Commander Pro and Asetek Pro)\n" );
    msg_info( "\t\t\t 2 - Color Pulse (Only Commander Pro and Asetek Pro)\n" );
    msg_info( "\t\t\t 3 - Color Shift (Only Commander Pro and Asetek Pro)\n" );
    msg_info( "\t\t\t 4 - Rainbow (Only Commander Pro and Asetek Pro)\n" );
    msg_info( "\t\t\t 5 - Temperature (Only Commander Pro, Asetek, and Asetek Pro)\n" );
    msg_info( "\t\tColors: <HTML Color Code>\t\t\t:Define Color for LED.\n" );
    msg_info( "\t\tWarn: <HTML Color Code>\t\t:Define Color for Warning Temp.\n" );
    msg_info( "\t\tTemp: <Temperature in Celsius>\t:Define Warning Temperature.\n" );

    msg_info( "\n\tFan:\n" );
    msg_info( "\t--fan " );
    msg_info( "channel=N," );
    msg_info( "mode=N," );
    msg_info( "pwm=PWM," );
    msg_info( "rpm=RPM," );
    msg_info( "temps=TEMP:TEMP:TEMP," );
    msg_info( "speeds=SPEED:SPEED:SPEED" );
    msg_info( "\n" );
    msg_info( "\t\tChannel: <fan number> :Selects a fan to setup. Accepted "
              "values are 1, 2, 3 or 4.\n" );
    msg_info( "\t\tModes:\n" );
    msg_info( "\t\t\t 0 - Fixed PWM (requires to specify the PWM)\n" );
    msg_info( "\t\t\t 1 - Fixed RPM (requires to specify the RPM)\n" );
    msg_info( "\t\t\t 2 - Default\n" );
    msg_info( "\t\t\t 3 - Quiet\n" );
    msg_info( "\t\t\t 4 - Balanced\n" );
    msg_info( "\t\t\t 5 - Performance\n" );
    msg_info( "\t\t\t 6 - Custom Curve\n" );
    msg_info( "\t\tPWM <PWM Percent> \t:The desired PWM for the selected fan. "
              "NOTE: it only works when fan mode is set to Fixed PWM\n" );
    msg_info( "\t\tRPM <fan RPM> \t:The desired RPM for the selected fan. "
              "NOTE: it works only when fan mode is set to Fixed RPM\n" );
    msg_info( "\t\tFor Custom Curves:\n" );
    msg_info( "\t\t\tTemps <C>\t:Define Celsius Temperatures for Fan.\n" );
    msg_info( "\t\t\tSpeeds <Percentage>\t:Define Values of RPM Percentage for Fan.\n" );

    msg_info( "\n\tPump:\n" );
    msg_info( "\t--pump " );
    msg_info( "mode=<mode>" );
    msg_info( "\n" );
    msg_info( "\t\tModes:\n" );
    msg_info( "\t\t\t 3 - Quiet\n" );
    msg_info( "\t\t\t 5 - Performance\n" );

    msg_info( "\n Without options, OpenCorsairLink will show the status of any "
              "detected Corsair Link device.\n" );
}
