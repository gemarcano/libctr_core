/*******************************************************************************
 * Copyright (C) 2016, 2017 Gabriel Marcano
 *
 * Refer to the COPYING.txt file at the top of the project directory. If that is
 * missing, this file is licensed under the GPL version 2.0 or later.
 *
 ******************************************************************************/

/** @file */

#ifndef CTR_CORE_HID_H_
#define CTR_CORE_HID_H_

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

//Button type
typedef uint32_t ctr_core_hid_button_type;

//HID Register
#define CTR_CORE_HID_REG     ((volatile ctr_core_hid_button_type*)0x10146000)

//Button definitions
#define CTR_CORE_HID_NONE     0x000u
#define CTR_CORE_HID_A       0x001u
#define CTR_CORE_HID_B       0x002u
#define CTR_CORE_HID_SELECT  0x004u
#define CTR_CORE_HID_START   0x008u
#define CTR_CORE_HID_RIGHT   0x010u
#define CTR_CORE_HID_LEFT    0x020u
#define CTR_CORE_HID_UP      0x040u
#define CTR_CORE_HID_DOWN    0x080u
#define CTR_CORE_HID_RT      0x100u
#define CTR_CORE_HID_LT      0x200u
#define CTR_CORE_HID_X       0x400u
#define CTR_CORE_HID_Y       0x800u

/**	@brief Returns whether the given button combination is pressed or not.
 *
 *	@param[in] buttons Button combination to check if they are pressed.
 *
 *	@returns True if the buttons passed in are pressed, false otherwise.
 */
bool ctr_core_hid_button_status(ctr_core_hid_button_type buttons);

/**	@brief Returns all of the buttons pressed at the instant this is called.
 *
 *	@brief returns All of the buttons pressed at the time this function is
 *		called.
 */
ctr_core_hid_button_type ctr_core_hid_get_buttons(void);

void ctr_core_input_wait(void);

#ifdef __cplusplus
}
#endif

#endif//CTR_CORE_HID_H_

