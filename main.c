#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <libusb.h>
#include "options.h"
#include "lowlevel/asetek4.h"
#include "lowlevel/hid.h"
#include "device.h"
#include "protocol/hid/core.h"

extern struct corsair_device_info corsairlink_devices[2];

int corsairlink_find_device(struct corsair_device_info *dev)
{
	int r;
	r = libusb_init(&dev->context);
	if (r < 0) {
		fprintf(stdout, "Init Error %d\n", r);
		return 1;
	}
	libusb_set_debug(dev->context, 3);

	dev->handle = libusb_open_device_with_vid_pid(dev->context, dev->vendor_id, dev->product_id);
	if(dev->handle == NULL) {
		return -1;
	}

	r = libusb_detach_kernel_driver(dev->handle, 0);
	r = libusb_claim_interface(dev->handle, 0);

	return 0;
}

void corsairlink_close(struct corsair_device_info *dev)
{
	int r;
	r = libusb_release_interface(dev->handle, 0);
	libusb_close(dev->handle);
	libusb_exit(dev->context);
}

int main(int argc, char *argv[])
{
	struct corsair_device_info *dev;
	int r; // result from libusb functions
	int i; // for loops

	struct option_flags flags;
	struct color led_color;
	struct color warning_led;
	struct fan_table fan1;
	struct fan_table pump;
	int8_t warning_led_temp;
	uint8_t pump_mode;
	
	options_parse(argc, argv, &flags,
		&led_color, &warning_led, &warning_led_temp,
		&fan1, &pump, &pump_mode);
	
	for (i=0; i<2; i++) {
		dev = &corsairlink_devices[i];
		r = corsairlink_find_device(dev);
		if (r >= 0) {
			fprintf(stdout, "CorsairLink Device Found: %s!\n", dev->name);
			break;
		}
	}

	r = dev->driver->init(dev->handle, dev->write_endpoint);

	float v, a, w;

	for (i=0; i<5; i++) {
		r = dev->driver->power->select(dev, i);
		fprintf(stdout, "Output %d\n", i);
		r = dev->driver->power->voltage(dev, &v);
		fprintf(stdout, "\tVoltage %.2f\n", v);
		r = dev->driver->power->amperage(dev, &a);
		fprintf(stdout, "\tAmps %.2f\n", a);
		r = dev->driver->power->wattage(dev, &w);
		fprintf(stdout, "\tWatts %.2f\n", w);
	}

	r = dev->driver->deinit(dev->handle, dev->write_endpoint);

exit:
	corsairlink_close(dev);
	return 0;
}
