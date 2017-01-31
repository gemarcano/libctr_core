/*******************************************************************************
 * Copyright (C) 2016, 2017 Gabriel Marcano
 *
 * Refer to the COPYING.txt file at the top of the project directory. If that is
 * missing, this file is licensed under the GPL version 2.0 or later.
 *
 ******************************************************************************/

/** @file */

#ifndef CTR_CORE_SURFACE_H_
#define CTR_CORE_SURFACE_H_

#include <stddef.h>
#include <stdint.h>

struct ctr_core_screen;

#ifdef __cplusplus
extern "C" {
#endif

/**	@brief Represents a single drawable surface.
 */
typedef struct ctr_core_surface ctr_core_surface;

//FIXME these functions are bound to change. I can't say I'm happy about the API

/**	@brief Returns the surface's width.
 *
 *	@param[in] screen Surface to poll for its width.
 *
 *	@return The width of the given surface.
 */
size_t ctr_core_surface_get_width(const void *surface);

/**	@brief Returns the surface's height.
 *
 *	@param[in] screen Surface to poll for its height.
 *
 *	@return The height of the given surface.
 */
size_t ctr_core_surface_get_height(const void *surface);

/**	@brief Retrieves the pixel specified from the given surface.
 *
 *	Note that only the actual size of the pixel, according to the current pixel,
 *	format is guaranteed to be correct. Any bits beyond the actual pixel size
 *	are not guaranteed to be anything useful. E.g. when in one of the 16 bit
 *	modes, the upper 16 bits of the return can be garbage and not zero.
 *	Accessing a pixel outside of the framebuffer invokes undefined behavior.
 *
 *	@param[in] surface Screen to access. Must be initialized.
 *	@param[in] x X location of pixel.
 *	@param[in] y Y location of pixel.
 *
 *	@returns The pixel value at the specified location. The current surface
 *	format governs how many of the returned bits mean anything.
 */
uint32_t ctr_core_surface_get_pixel(const void *surface, size_t x, size_t y);

/**	@brief Sets the pixel to the value specified in the given surface.
 *
 *	Note that only the actual size of the pixel, according to the current pixel,
 *	format is used for setting the pixel value. Any bits beyond the actual pixel
 *	are ignored. Accessing a pixel outside of the framebuffer invokes undefined
 *	behavior.
 *
 *	@param[in] surface Screen to use for operation. Must be initialized.
 *	@param[in] x X location of pixel.
 *	@param[in] y Y location of pixel.
 *	@param[in] pixel Pixel value to set.
 *
 *	@post The pixel value specified has been set at the given position.
 */
void ctr_core_surface_set_pixel(void *surface, size_t x, size_t y, uint32_t pixel);

/**	@brief Returns the screen holding this surface.
 *
 *	@param[in] surface Surface to query.
 *
 *	@returns The screen holding the given surface.
 */
struct ctr_core_screen *ctr_core_surface_get_screen(void *surface);

/**	@brief Clears the surface to the given pixel value.
 *
 *	@param[in] surface Surface to clear.
 *	@param[in] pixel Pixel to set all pixels in the surface to.
 *
 *	@post Surface has been cleared with given pixel.
 */
void ctr_core_surface_clear(void *surface, uint32_t pixel);

//FIXME currently bitmaps must start at the beginning of a byte
/**	@brief Represents a single bitmap entity.
 */
typedef struct
{
	size_t width, height;
	void *data;
} ctr_core_surface_bitmap;

/**	@brief Draws the given bitmap at the given location in the given surface.
 *
 *	The coordinates in the framebuffer given dictate where the upper left corner
 *	of the bitmap will be drawn. If the bitmap is placed in the framebuffer such
 *	that it will overflow outside of the framebuffer, this invokes undefined
 *	behavior.
 *
 *	@param[in] surface Surface to use for operation. Must be initialized.
 *	@param[in] x X location of pixel.
 *	@param[in] y Y location of pixel.
 *	@param[in] pixel Pixel to use as color when drawing bitmap.
 *	@param[in] bitmap Bitmap to draw.
 *
 *	@post The bitmap has been drawn at the given location.
 */
void ctr_core_surface_draw_bitmap(void *surface, size_t x, size_t y, uint32_t pixel, ctr_core_surface_bitmap *bitmap);

#ifdef __cplusplus
}

#include <ctr_core/ctr_core_pixel.hpp>

namespace ctr_core
{
	template<class Pixel>
	class surface
	{
	public:
		typedef Pixel pixel_type;

		virtual ~surface() = default;

		virtual std::size_t width() const = 0;
		virtual std::size_t height() const = 0;
		virtual pixel_type operator()(std::size_t x, std::size_t y) = 0;
		virtual const pixel_type operator()(std::size_t x, std::size_t y) const = 0;
		virtual surface& get_screen() = 0;
		virtual const surface& get_screen() const = 0;

		virtual void clear(const pixel_type& pixel) = 0;
	};

	typedef surface<generic_pixel> generic_surface;
}
#endif

#endif//CTR_CORE_SURFACE_H_

