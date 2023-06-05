// SPDX-License-Identifier: GPL-2.0-or-later
// Copyright: Gabriel Marcano, 2023

#ifndef CTR_CORE_I2C_H_
#define CTR_CORE_I2C_H_

#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

#define CTR_I2C1_BASE (0x10161000u)
#define CTR_I2C2_BASE (0x10144000u)
#define CTR_I2C3_BASE (0x10148000u)

#define CTR_I2C_REG(BASE, OFFSET) (((volatile uint8_t*)(BASE))[OFFSET])
#define CTR_I2C_DATA_OFFSET (0)
#define CTR_I2C_CNT_OFFSET (1)
#define CTR_I2C_CNTEX_OFFSET (2)
#define CTR_I2C_SCL_OFFSET (4)

typedef enum
{
	CTR_I2C1,
	CTR_I2C2,
	CTR_I2C3
} ctr_core_i2c_bus;

int ctr_core_i2c_write(ctr_core_i2c_bus bus, uint8_t device, uint8_t register_, const uint8_t *data, size_t amount);
int ctr_core_i2c_write_one(ctr_core_i2c_bus bus, uint8_t device, uint8_t register_, uint8_t data);
int ctr_core_i2c_read(ctr_core_i2c_bus bus, uint8_t device, uint8_t register_, uint8_t *data, size_t amount);
int ctr_core_i2c_read_one(ctr_core_i2c_bus bus, uint8_t device, uint8_t register_, uint8_t *data);

#ifdef __cplusplus
}
#endif

#endif//CTR_CORE_I2C_H_
