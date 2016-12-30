/*******************************************************************************
 * Copyright (C) 2016 Gabriel Marcano
 *
 * Refer to the COPYING.txt file at the top of the project directory. If that is
 * missing, this file is licensed under the GPL version 2.0 or later.
 *
 ******************************************************************************/

#include <ctr_core/ctr_core_hid.h>
#include <stdbool.h>

bool ctr_core_hid_button_status(ctr_core_hid_button_type buttons)
{
	return (~(*CTR_CORE_HID_REG) & 0xFFF) == buttons;
}

ctr_core_hid_button_type ctr_core_hid_get_buttons(void)
{
	return ~(*CTR_CORE_HID_REG) & 0xFFF;
}

//This was taken and adapted from b1l1s's hid.c
void ctr_core_input_wait(void)
{
	uint32_t prev = *CTR_CORE_HID_REG;
	uint32_t key;
	do
	{
		// Wait for state change
		while((key = *CTR_CORE_HID_REG) == prev);

		// Ignore key releases
		if(key > prev)
		{
			prev = key;
			continue;
		}

		// Simple debounce
		uint32_t deb = 0x7FFF;
		while(--deb)
		{
			if(key != *CTR_CORE_HID_REG)
			{
				// State changed, redo
				key = prev;
				break;
			}
		}
	} while(key == prev);
}

