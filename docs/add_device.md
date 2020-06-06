# Adding A new Device

```
    {
        .vendor_id = 0x1b1c,
        .product_id = 0x0c04,
        .device_id = 0x3b,
        .name = "H80i",
        .read_endpoint = 0x01 | LIBUSB_ENDPOINT_IN,
        .write_endpoint = 0x00 | LIBUSB_ENDPOINT_OUT,
        .driver = &corsairlink_driver_coolit,
        .lowlevel = &corsairlink_lowlevel_coolit,
        .led_control_count = 1,
        .fan_control_count = 4,
        .pump_index = 5,
    },
```

- `vendor_id` is used for the Corsair USB Vendor ID (0x1B1C).
- `product_id` is used for the USB Product ID.
- `device_id` is used for early CorsairLink CoolIT AIOs; Should use 0xFF if not supported by the device protocol.
- `name` is the String for OpenCorsairLink, can be copied from USB device string.
- `read_endpoint` the device endpoint for reading data from the device.
- `write_endpoint` the device endpoint for commanding device.
- `driver` the driver/protocol that the device uses.
- `lowlevel` the low level protocol for the device.
- `led_control_count` is the number of led channels.
- `fan_control_count` is the number of fan channels.
- `pump_index` is the array offset for pump channel, typically the pump channel will come after fan channels as they usually share the same command.
