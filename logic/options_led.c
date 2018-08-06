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
led_control_init( struct led_control* settings )
{
    settings->strip_count = 10;
    settings->speed = 3;
    INIT_RAINBOW_LED( settings->led_colors );

    // INIT_DEFAULT_LED(settings->led_colors[0]);
    // INIT_WARNING_LED(settings->led_colors[2]);

    settings->temperatures[0] = 35;
    settings->temperatures[1] = 45;
    settings->temperatures[2] = 55;
}

void
led_suboptions_parse( char* subopts, struct led_control* settings )
{
    int opt, returnCode = 0, option_index = 0;
    char *value, *token;
    uint8_t ii = 0;

    led_control_init( settings );

    while ( *subopts != '\0' )
        switch ( getsubopt( &subopts, led_options, &value ) )
        {
        case SUBOPTION_LED_CHANNEL:
            sscanf( value, "%hhu", &settings->channel );
            msg_debug( "LED Channel = %u\n", settings->channel );
            break;

        case SUBOPTION_LED_MODE:
            sscanf( value, "%u", &settings->mode );
            msg_debug( "LED Mode = %u\n", settings->mode );
            switch ( settings->mode )
            {
            case BLINK:
            case PULSE:
            case SHIFT:
                INIT_RAINBOW_LED( settings->led_colors );
                break;
            default:
                break;
            }
            break;

        case SUBOPTION_LED_STRIP_COUNT:
            sscanf( value, "%hhu", &settings->strip_count );
            msg_debug( "LED Strip Count = %u\n", settings->strip_count );
            break;

        case SUBOPTION_LED_STRIP_TYPE:
            sscanf( value, "%hhu", &settings->strip_type );
            msg_debug( "LED Strip Type = %u\n", settings->strip_type );
            break;

        case SUBOPTION_LED_SPEED:
            sscanf( value, "%hhu", &settings->speed );
            msg_debug( "LED Speed = %u\n", settings->speed );
            break;

        case SUBOPTION_LED_DIRECTION:
            sscanf( value, "%hhu", &settings->direction );
            msg_debug( "LED Direction = %u\n", settings->direction );
            break;

        case SUBOPTION_LED_CHANGE_STYLE:
            sscanf( value, "%hhu", &settings->color_change_style );
            msg_debug( "LED Change Style = %u\n", settings->color_change_style );
            break;

        case SUBOPTION_LED_COLORS: /* led color */
            ii = 0;
            token = strtok( value, ":" );
            while ( token != NULL )
            {
                if ( ii == 7 )
                    break;
                msg_debug( "Found Color %d: %s\n", ii, token );
                sscanf(
                    token, "%02hhX%02hhX%02hhX,", &settings->led_colors[ii].red,
                    &settings->led_colors[ii].green, &settings->led_colors[ii].blue );
                ++ii;
                token = strtok( NULL, ":" );
            }
            break;

        case SUBOPTION_LED_WARN: /* led warning color */
            sscanf(
                value, "%02hhX%02hhX%02hhX", &settings->led_colors[2].red,
                &settings->led_colors[2].green, &settings->led_colors[2].blue );
            break;

        case SUBOPTION_LED_TEMPERATURE: /* led warning temperatures */
            ii = 0;
            token = strtok( value, ":" );
            while ( token != NULL )
            {
                if ( ii == 3 )
                    break;
                msg_debug( "Found Color %d: %s\n", ii, token );
                if ( ii == 0 )
                    sscanf( token, "%hd", &settings->temperatures[0] );
                if ( ii == 1 )
                    sscanf( token, "%hd", &settings->temperatures[1] );
                if ( ii == 2 )
                    sscanf( token, "%hd", &settings->temperatures[2] );
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
