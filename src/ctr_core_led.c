#include <ctr_core/ctr_core_led.h>
#include <ctr_core/ctr_core_i2c.h>

#include <stdbool.h>

void ctr_core_led_wifi_set(bool enable)
{
	ctr_core_i2cWriteRegister(3, 0x2A, enable);
}

void ctr_core_led_3d_set(bool enable)
{
	ctr_core_i2cWriteRegister(3, 0x2C, enable);
}

