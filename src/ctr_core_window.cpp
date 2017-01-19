#include <ctr_core/ctr_core_window.h>
#include <ctr_core/ctr_core_i2c.h>
#include <ctr_core/ctr_core_screen.h>

#include <climits>

void ctr_core_window_initialize(ctr_core_window *window, ctr_core_screen *screen, std::size_t x, std::size_t y, std::size_t width, std::size_t height)
{
}

void ctr_core_window_clear(void *window, std::uint32_t pixel)
{
	ctr_core::generic_window& wnd = *reinterpret_cast<ctr_core::generic_window*>(window);
	for (std::size_t x = 0; x < wnd.width(); ++x)
	{
		for (std::size_t y = wnd.height(); y > 0; --y)
		{
			ctr_core_window_set_pixel(window, x, y, pixel);
		}
	}
}

std::size_t ctr_core_window_get_width(const void *window)
{
	const ctr_core::generic_window& wnd = *reinterpret_cast<const ctr_core::generic_window*>(window);
	return wnd.width();
}

std::size_t ctr_core_window_get_height(const void *window)
{
	const ctr_core::generic_window& wnd = *reinterpret_cast<const ctr_core::generic_window*>(window);
	return wnd.height();
}

std::uint32_t ctr_core_window_get_pixel(const void *window, std::size_t x, std::size_t y)
{
	const ctr_core::generic_window& wnd = *reinterpret_cast<const ctr_core::generic_window*>(window);
	const ctr_core::generic_pixel& pixel = wnd.get_pixel(x, y);
	std::uint32_t result = 0;
	for (std::size_t i = 0; i < wnd.pixel_size(); ++i)
	{
		result |= static_cast<unsigned>(pixel[i]) << (CHAR_BIT*i);
	}
	return result;
}

void ctr_core_window_set_pixel(void *window, std::size_t x, std::size_t y, std::uint32_t pixel)
{
	ctr_core::generic_window& wnd = *reinterpret_cast<ctr_core::generic_window*>(window);
	wnd.set_pixel(wnd.x_ + x, wnd.y_ + y, pixel);
}

ctr_core_screen *ctr_core_window_get_screen(void *window)
{
	ctr_core::generic_window& wnd = *reinterpret_cast<ctr_core::generic_window*>(window);
	return reinterpret_cast<ctr_core_screen*>(&wnd.get_screen());
}

void ctr_core_window_draw_bitmap(ctr_core_window *window, std::size_t x, std::size_t y, std::uint32_t pixel, ctr_core_screen_bitmap *bitmap)
{
	//ctr_core_screen_draw_bitmap(window->parent, window->x + x, window->y + y, pixel, bitmap);
}

