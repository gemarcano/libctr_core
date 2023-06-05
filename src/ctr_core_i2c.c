// SPDX-License-Identifier: GPL-2.0-or-later
// Copyright: Gabriel Marcano, 2023

#include <ctr_core/ctr_core_i2c.h>
#include <stdint.h>
#include <stdbool.h>

static uint32_t get_base(ctr_core_i2c_bus bus)
{
	switch(bus)
	{
		default:
		case CTR_I2C1:
			return CTR_I2C1_BASE;
		case CTR_I2C2:
			return CTR_I2C2_BASE;
		case CTR_I2C3:
			return CTR_I2C3_BASE;
	}
}

// I2C primitives

static void wait(ctr_core_i2c_bus bus)
{
	// The busy bit will remain high while the hardware is busy
	uint32_t base = get_base(bus);
	while (CTR_I2C_REG(base, CTR_I2C_CNT_OFFSET) & 0x80u);
}

static bool ack(ctr_core_i2c_bus bus)
{
	const uint32_t base = get_base(bus);
	return CTR_I2C_REG(base, CTR_I2C_CNT_OFFSET) & 0x10;
}

static void error(ctr_core_i2c_bus bus)
{
	const uint32_t base = get_base(bus);
	//Send stop, and set pause? bit
	CTR_I2C_REG(base, CTR_I2C_CNT_OFFSET) = 0xC5;
	wait(bus);
}

static bool start(ctr_core_i2c_bus bus, uint8_t device)
{
	const uint32_t base = get_base(bus);
	CTR_I2C_REG(base, CTR_I2C_DATA_OFFSET) = device;
	CTR_I2C_REG(base, CTR_I2C_CNT_OFFSET) = 0xC2;
	wait(bus);
	if (!ack(bus))
	{
		error(bus);
		return false;
	}
	return true;
}

static bool send(ctr_core_i2c_bus bus, uint8_t data, bool last)
{
	const uint32_t base = get_base(bus);
	CTR_I2C_REG(base, CTR_I2C_DATA_OFFSET) = data;
	CTR_I2C_REG(base, CTR_I2C_CNT_OFFSET) = 0xC0 | (last ? 1 : 0);
	wait(bus);
	if (!ack(bus))
	{
		error(bus);
		return false;
	}
	return true;
}

static uint8_t read(ctr_core_i2c_bus bus, bool last)
{
	const uint32_t base = get_base(bus);
	CTR_I2C_REG(base, CTR_I2C_CNT_OFFSET) = 0xE0 | (last ? 1 : 0x10);
	wait(bus);
	return CTR_I2C_REG(base, CTR_I2C_DATA_OFFSET);
}

// FIXME clean up error handling logic
static int i2c_write(ctr_core_i2c_bus bus, uint8_t device, uint8_t register_, const uint8_t *data, size_t amount)
{
	if (!start(bus, device) || !send(bus, register_, false))
		return -1;
	for (size_t j = 0; j < (amount - 1); ++j)
	{
		if (!send(bus, data[j], false))
			return -1;
	}

	if (!send(bus, data[amount - 1], true))
		return -1;
	return 0;
}


int ctr_core_i2c_write(ctr_core_i2c_bus bus, uint8_t device, uint8_t register_, const uint8_t *data, size_t amount)
{
	size_t i;
	for (i = 0; i < 8; ++i)
	{
		if (i2c_write(bus, device, register_, data, amount) == 0)
			break;
	}

	if (i == 8)
		return -1;
	return 0;
}

int ctr_core_i2c_write_one(ctr_core_i2c_bus bus, uint8_t device, uint8_t register_, uint8_t data)
{
	return ctr_core_i2c_write(bus, device, register_, &data, 1);
}

int ctr_core_i2c_read(ctr_core_i2c_bus bus, uint8_t device, uint8_t register_, uint8_t *data, size_t amount)
{
	if (amount == 0)
		return 0;

	size_t i;
	for (i = 0; i < 8; ++i)
	{
		if (start(bus, device) && send(bus, register_, false) && start(bus, device | 1))
			break;
	}

	if (i == 8)
		return -1;

	for (i = 0; i < (amount - 1); ++i)
	{
		data[i] = read(bus, false);
	}
	data[amount-1] = read(bus, true);
	return 0;
}

int ctr_core_i2c_read_one(ctr_core_i2c_bus bus, uint8_t device, uint8_t register_, uint8_t *data)
{
	return ctr_core_i2c_read(bus, device, register_, data, 1);
}
