/*******************************************************************************
 * Copyright (C) 2016, 2017 Gabriel Marcano
 *
 * Refer to the COPYING.txt file at the top of the project directory. If that is
 * missing, this file is licensed under the GPL version 2.0 or later.
 *
 ******************************************************************************/

/** @file */

#ifndef CTR_CORE_LED_H_
#define CTR_CORE_LED_H_

#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/**	@brief Sets the WiFi LED state.
 *
 */
void ctr_core_led_wifi_set(bool enable);

/** @brief Sets the 3D LED state.
 *
 */
void ctr_core_led_3d_set(bool enable);

#ifdef __cplusplus
}
#endif

#endif//CTR_CORE_LED_H_

