/*******************************************************************************
 * Copyright (C) 2016 Gabriel Marcano
 *
 * Refer to the COPYING.txt file at the top of the project directory. If that is
 * missing, this file is licensed under the GPL version 2.0 or later.
 *
 ******************************************************************************/

/** @file */

#ifndef CTR_CORE_WINDOW_H_
#define CTR_CORE_WINDOW_H_

#include <ctr_core/ctr_core_screen.h>
#include <ctr_core/ctr_core_surface.h>

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**	@brief Represents a single window within the screen.
 */
typedef struct
{
	ctr_core_surface base;
	ctr_core_screen *parent;
	size_t x;
	size_t y;
	size_t width;
	size_t height;
} ctr_core_window;

/**	@brief Initializes the given window.
 *
 *	@param[out] window Window structure to initialize.
 *	@param[in] parent Parent screen, to hold the window.
 *	@param[in] x X position in parent of the window.
 *	@param[in] y Y position in parent of the window.
 *	@param[in] width Width of the window in pixels.
 *	@param[in] height Height of the window in pixels.
 *
 *	@post The window object has been initialized and is ready for use.
 */
void ctr_core_window_initialize(ctr_core_window *window, ctr_core_screen *parent, size_t x, size_t y, size_t width, size_t height);

//FIXME these functions are bound to change. I can't say I'm happy about the API

/**	@brief Returns the window's width.
 *
 *	@param[in] screen Surface to poll for its width.
 *
 *	@return The width of the given window.
 */
size_t ctr_core_window_get_width(const void *window);

/**	@brief Returns the window's height.
 *
 *	@param[in] screen Surface to poll for its height.
 *
 *	@return The height of the given window.
 */
size_t ctr_core_window_get_height(const void *window);

/**	@brief Retrieves the pixel specified from the given window.
 *
 *	Note that only the actual size of the pixel, according to the current pixel,
 *	format is guaranteed to be correct. Any bits beyond the actual pixel size
 *	are not guaranteed to be anything useful. E.g. when in one of the 16 bit
 *	modes, the upper 16 bits of the return can be garbage and not zero.
 *	Accessing a pixel outside of the framebuffer invokes undefined behavior.
 *
 *	@param[in] window Window to access. Must be initialized.
 *	@param[in] x X location of pixel.
 *	@param[in] y Y location of pixel.
 *
 *	@returns The pixel value at the specified location. The current window
 *	format governs how many of the returned bits mean anything.
 */
uint32_t ctr_core_window_get_pixel(const void *window, size_t x, size_t y);

/**	@brief Sets the pixel to the value specified in the given window.
 *
 *	Note that only the actual size of the pixel, according to the current pixel,
 *	format is used for setting the pixel value. Any bits beyond the actual pixel
 *	are ignored. Accessing a pixel outside of the framebuffer invokes undefined
 *	behavior.
 *
 *	@param[in] window Window to use for operation. Must be initialized.
 *	@param[in] x X location of pixel.
 *	@param[in] y Y location of pixel.
 *	@param[in] pixel Pixel value to set.
 *
 *	@post The pixel value specified has been set at the given position.
 */
void ctr_core_window_set_pixel(void *window, size_t x, size_t y, uint32_t pixel);

/**	@brief Draws the given bitmap at the given location in the given window.
 *
 *	The coordinates in the framebuffer given dictate where the upper left corner
 *	of the bitmap will be drawn. If the bitmap is placed in the framebuffer such
 *	that it will overflow outside of the framebuffer, this invokes undefined
 *	behavior.
 *
 *	@param[in] window Window to use for operation. Must be initialized.
 *	@param[in] x X location of pixel.
 *	@param[in] y Y location of pixel.
 *	@param[in] pixel Pixel to use as color when drawing bitmap.
 *	@param[in] bitmap Bitmap to draw.
 *
 *	@post The bitmap has been drawn at the given location.
 */
void ctr_core_window_draw_bitmap(ctr_core_window *window, size_t x, size_t y, uint32_t pixel, ctr_core_screen_bitmap *bitmap);

/**	@brief Returns the screen in which this window is located.
 *
 *	@param[in] window Window to query.
 *
 *	@returns Screen in which the given window is located.
 */
ctr_core_screen *ctr_core_window_get_screen(void *window);

/**	@brief Clears the window with the given pixel.
 *
 *	@paran[in] window Window to clear.
 *	@param[in] pixel Pixel to clear window with.
 *
 *	@post Window is cleared with the given pixel.
 */
void ctr_core_window_clear(void *window, uint32_t pixel);

#ifdef __cplusplus
}
#endif

#endif//CTR_CORE_WINDOW_H_

