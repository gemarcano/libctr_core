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
typedef struct ctr_core_window ctr_core_window;

/**	@brief Creates a new window object.
 *
 *	@param[in] parent Parent screen, to hold the window.
 *	@param[in] x X position in parent of the window.
 *	@param[in] y Y position in parent of the window.
 *	@param[in] width Width of the window in pixels.
 *	@param[in] height Height of the window in pixels.
 *
 *	@returns A new window object.
 */
ctr_core_window *ctr_core_window_initialize(ctr_core_screen *parent, size_t x, size_t y, size_t width, size_t height);

/**	@brief Destroys the given window object.
 *
 *	@param[in] window Window object to destroy.
 *
 *	@post Resources held by window object are freed. Any further use will lead
 *		to undefined behavior.
 */
void ctr_core_window_destroy(ctr_core_window *window);

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

#ifdef __cplusplus
}

#include <ctr_core/ctr_core_surface.h>
#include <memory>

namespace ctr_core
{
	template<class Parent, class Width, class Height>
	class window
	{
	public:
		typedef typename Parent::pixel_type pixel_type;

		window(Parent& parent, size_t x, size_t y);
		constexpr size_t width() const;
		constexpr size_t height() const;
		pixel_type& operator()(size_t x, size_t y);
		const pixel_type& operator()(size_t x, size_t y) const;
		Parent& get_parent();
		const Parent& get_parent() const;
		void clear(const pixel_type& pixel);
	};

	class generic_window : public generic_surface
	{
	public:
		generic_window(generic_surface& parent, size_t width, size_t height, size_t x, size_t y);
		virtual size_t width() const override;
		virtual size_t height() const override;
		virtual pixel_type get_pixel(size_t x, size_t y) override;
		virtual const pixel_type get_pixel(size_t x, size_t y) const override;
		virtual void set_pixel(size_t x, size_t y, const pixel_type& pixel) override;
		virtual surface& get_screen() override;
		virtual const surface& get_screen() const override;
		virtual void clear(const pixel_type& pixel) override;
		pixel_format get_pixel_format() const;
		size_t pixel_size() const;

	public:
		size_t width_;
		size_t height_;
		size_t x_;
		size_t y_;
		generic_surface &parent;
		std::unique_ptr<generic_surface> window_impl;
	};
}
#endif

#endif//CTR_CORE_WINDOW_H_

