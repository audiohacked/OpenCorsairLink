int corsairlink_rmi_fan_rpm(struct corsair_device_info *dev, uint16_t rpm) {
	int r;
	uint8_t response[64];
	uint8_t commands[32];
	memset(response, 0, sizeof(response));
	memset(commands, 0, sizeof(commands));

	commands[0] = 0x03;
	commands[1] = 0x90;
	commands[2] = 0x00;
	commands[3] = 0x00;

	r = dev->driver->write(dev->handle, dev->write_endpoint, commands, 4);
	r = dev->driver->read(dev->handle, dev->read_endpoint, response, 64);

	rpm = 
	return 0;	
}
