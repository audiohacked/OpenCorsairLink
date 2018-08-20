/* test_protocol_asetek.c */
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>

#include <stdio.h>
#include <string.h>

#include "cmocka.h"
#include "protocol/asetek.h"

/* redefinitons/wrapping */
int __wrap_corsairlink_asetek_write(
    struct libusb_device_handle* dev_handle, uint8_t endpoint, uint8_t* data, int length)
{
    check_expected(endpoint);
    check_expected(length);
    check_expected(data);
    return mock();
}

int __wrap_corsairlink_asetek_read(
    struct libusb_device_handle* dev_handle, uint8_t endpoint, uint8_t* data, int length)
{
    check_expected(endpoint);
    check_expected(data);
    check_expected(length);
    return mock();
}

/* tests */
void test_corsairlink_asetek_firmware_id_failure(void **state)
{
    (void) state; /* unused */
    int ret;

    uint8_t commands[32] = {};
    commands[0] = 0x20;

    uint8_t response[32] = {};
    response[0x17] = 2;
    response[0x18] = 8;
    response[0x19] = 0;
    response[0x1A] = 0;

    expect_value(__wrap_corsairlink_asetek_write, endpoint, 0x02)
    expect_value(__wrap_corsairlink_asetek_write, data, commands);
    expect_value(__wrap_corsairlink_asetek_write, length, 32);
    will_return(__wrap_corsairlink_asetek_write, -1);

    expect_value(__wrap_corsairlink_asetek_read, endpoint, 0x82)
    expect_value(__wrap_corsairlink_asetek_read, data, response);
    expect_value(__wrap_corsairlink_asetek_read, length, 32);
    will_return(__wrap_corsairlink_asetek_read, -1);

    ret = corsairlink_asetek_firmware_id(NULL, NULL, firmware_str, firmware_len);
    assert_int_equal(-1, ret);
}

void test_corsairlink_asetek_firmware_id_success(void **state)
{
    (void) state; /* unused */
    int ret;

    uint8_t commands[32] = {};
    commands[0] = 0x20;

    uint8_t response[32] = {};
    response[0x17] = 2;
    response[0x18] = 8;
    response[0x19] = 0;
    response[0x1A] = 0;

    expect_value(__wrap_corsairlink_asetek_write, endpoint, 0x02)
    expect_value(__wrap_corsairlink_asetek_write, data, commands);
    expect_value(__wrap_corsairlink_asetek_write, length, 32);
    will_return(__wrap_corsairlink_asetek_write, 0);

    expect_value(__wrap_corsairlink_asetek_read, endpoint, 0x82)
    expect_value(__wrap_corsairlink_asetek_read, data, response);
    expect_value(__wrap_corsairlink_asetek_read, length, 32);
    will_return(__wrap_corsairlink_asetek_read, 0);

    ret = corsairlink_asetek_firmware_id(NULL, NULL, firmware_str, firmware_len);
    assert_int_equal(0, ret);
}


const struct CMUnitTest protocol_asetek_tests[] = {
    cmocka_unit_test(test_corsairlink_asetek_read_failure),
    cmocka_unit_test(test_corsairlink_asetek_read_success),
};


int main(void)
{
    return cmocka_run_group_tests(protocol_asetek_tests, NULL, NULL);
}
