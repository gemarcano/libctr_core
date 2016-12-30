/*******************************************************************************
 * Copyright (C) 2016 Gabriel Marcano
 *
 * Refer to the COPYING.txt file at the top of the project directory. If that is
 * missing, this file is licensed under the GPL version 2.0 or later.
 *
 ******************************************************************************/

/** @file */

#include <ctr_core/ctr_core_system.h>
#include <ctr_core/ctr_core_i2c.h>

#include <stdint.h>
#include <stdbool.h>

#define PDN_MPCORE_CFG ((volatile uint8_t*)0x10140FFC)
#define PDN_SPI_CNT ((volatile uint8_t*)0x101401C0)

ctr_core_system_type ctr_core_get_system_type(void)
{
	//This is seemingly not confirmed on 3dbrew, but it seems PDN_MPCORE_CFG's
	//second and third bit are only set on the N3DS, while the first bit is
	//set for all systems. Use that to detect the type of system.
	return 0x07 == *PDN_MPCORE_CFG ? SYSTEM_N3DS : SYSTEM_O3DS;
}

bool ctr_core_detect_a9lh_entry(void)
{
	//Aurora determined that this register isn't yet set when a9lh launches.
	return *PDN_SPI_CNT == 0;
}

void ctr_core_system_poweroff(void)
{
	ctr_core_i2cWriteRegister(I2C_DEV_MCU, 0x20, 1);
	while (true);
}

void ctr_core_system_reset(void)
{
	ctr_core_i2cWriteRegister(I2C_DEV_MCU, 0x20, 1 << 2);
	while (true);
}

