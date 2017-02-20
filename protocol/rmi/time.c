#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <libusb.h>

#include "../../lowlevel/rmi.h"
#include "../../device.h"
#include "core.h"

int corsairlink_rmi_time_powered(struct corsair_device_info *dev, uint32_t *powered) {
	int r;
	uint8_t response[64];
	uint8_t commands[32];
	memset(response, 0, sizeof(response));
	memset(commands, 0, sizeof(commands));

	commands[0] = 0x03;
	commands[1] = 0xd1;
	commands[2] = 0x00;
	commands[3] = 0x00;
	commands[4] = 0x00;
	commands[5] = 0x00;

	r = dev->driver->write(dev->handle, dev->write_endpoint, commands, 6);
	r = dev->driver->read(dev->handle, dev->read_endpoint, response, 64);

	memcpy(powered, response+2, 4);

	return 0;
}

int corsairlink_rmi_time_uptime(struct corsair_device_info *dev, uint32_t *uptime) {
	int r;
	uint8_t response[64];
	uint8_t commands[32];
	memset(response, 0, sizeof(response));
	memset(commands, 0, sizeof(commands));

	commands[0] = 0x03;
	commands[1] = 0xd2;

	r = dev->driver->write(dev->handle, dev->write_endpoint, commands, 6);
	r = dev->driver->read(dev->handle, dev->read_endpoint, response, 64);

	memcpy(uptime, response+2, 4);

	return 0;
}
	
