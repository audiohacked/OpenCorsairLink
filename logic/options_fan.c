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
#include "print.h"

#include <errno.h>
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void
fan_control_init( struct fan_control* settings )
{
    settings->channel = 0;
    settings->mode = 0;
}

void
fan_suboptions_parse( char* subopts, struct fan_control* settings )
{
    int opt, returnCode = 0, option_index = 0;
    char *value, *token;
    uint8_t ii = 0;

    while ( *subopts != '\0' )
        switch ( getsubopt( &subopts, fan_options, &value ) )
        {
        case SUBOPTION_FAN_CHANNEL:
            sscanf( value, "%hhu", &settings->channel );
            msg_debug( "FAN Channel = %u\n", settings->channel );
            break;

        case SUBOPTION_FAN_MODE:
            sscanf( value, "%u", &settings->mode );
            msg_debug( "FAN Mode = %u\n", settings->mode );
            break;

        case SUBOPTION_FAN_PWM:
            sscanf( value, "%hhu", &settings->speed_pwm );
            msg_debug( "FAN PWM = %u\n", settings->speed_pwm );
            break;

        case SUBOPTION_FAN_RPM:
            sscanf( value, "%hu", &settings->speed_rpm );
            msg_debug( "FAN RPM = %u\n", settings->speed_rpm );
            break;

        case SUBOPTION_FAN_TEMPERATURES:
            ii = 0;
            token = strtok( value, ":" );
            while ( token != NULL )
            {
                if ( ii == 6 )
                    break;
                sscanf( token, "%hhu", &settings->table[ii].temperature );
                msg_debug( "FAN Temperature %u: %u\n", ii, settings->table[ii].temperature );
                ++ii;
                token = strtok( NULL, ":" );
            }
            break;

        case SUBOPTION_FAN_SPEEDS:
            ii = 0;
            token = strtok( value, ":" );
            while ( token != NULL )
            {
                if ( ii == 6 )
                    break;
                sscanf( token, "%hhu", &settings->table[ii].speed );
                msg_debug( "FAN Speed %d: %u\n", ii, settings->table[ii].speed );
                ++ii;
                token = strtok( NULL, ":" );
            }
            break;

        default:
            /* Unknown suboption. */
            msg_info( "Unknown suboption `%s'\n", value );
            break;
        }
}
