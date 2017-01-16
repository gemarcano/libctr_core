/*******************************************************************************
 * Copyright (C) 2016 Gabriel Marcano
 *
 * Refer to the COPYING.txt file at the top of the project directory. If that is
 * missing, this file is licensed under the GPL version 2.0 or later.
 *
 ******************************************************************************/

/** @file */

#ifndef CTR_CORE_SYSTEM_H_
#define CTR_CORE_SYSTEM_H_

#include <stdbool.h>

#ifdef CTR_NORETURN
#undef CTR_NORETURN
#endif

#ifdef __cplusplus
extern "C" {
#define CTR_NORETURN [[noreturn]]
#else
#define CTR_NORETURN _Noreturn
#endif

/** @brief Enumeration describing the running system.
 */
typedef enum
{
    SYSTEM_O3DS,
    SYSTEM_N3DS,
} ctr_core_system_type;

/**	@brief Returns whether the arm9 entry was a9lh or not.
 *
 *	@returns True if the arm9 entrypoint was a9lh, false otherwise.
 */
bool ctr_core_detect_a9lh_entry(void);

/**	@brief Powers off the 3DS.
 *
 *	This function does not return. It powers off the 3DS via an i2c call to the
 *	MPU.
 *
 *	@post 3DS has powered off.
 */
CTR_NORETURN void ctr_core_system_poweroff(void);

/**	@brief Resets the 3DS.
 *
 *	This function does not return. It resets the 3DS via an i2c call to the MPU.
 *
 *	@post 3DS has restarted.
 */
CTR_NORETURN void ctr_core_system_reset(void);

/**	@brief Returns the enumeration corresponding to the running system.
 *
 *	@returns The type of system this is called in.
 */
ctr_core_system_type ctr_core_get_system_type(void);

#ifdef __cplusplus
}

namespace ctr_core
{
	/**	@brief Class representing the general system.
	 */
	class system
	{
	public:
		/**	@brief @See ctr_core_detect_a9lh_entry for details.
		 */
		static ctr_core_system_type get_type();

		/**	@brief @See ctr_core_system_reset for details.
		 */
		[[ noreturn ]] static void reset();

		/**	@brief @See ctr_core_system_poweroff for details.
		 */
		[[ noreturn ]] static void poweroff();
	};
}

#endif

#endif//CTR_CORE_SYSTEM_H_

