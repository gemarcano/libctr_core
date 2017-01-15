#include <ctr_core/ctr_core_window.h>
#include <ctr_core/ctr_core_i2c.h>
#include <ctr_core/ctr_core_screen.h>

static const ctr_core_surface window_base = {
	ctr_core_window_get_width,
	ctr_core_window_get_height,
	ctr_core_window_get_pixel,
	ctr_core_window_set_pixel,
	ctr_core_window_get_screen,
	ctr_core_window_clear
};

void ctr_core_window_initialize(ctr_core_window *window, ctr_core_screen *screen, size_t x, size_t y, size_t width, size_t height)
{
	window->base = window_base;
	window->parent = screen;
	window->x = x;
	window->y = y;
	window->width = width;
	window->height = height;
}

void ctr_core_window_clear(void *window, uint32_t pixel)
{
	ctr_core_window *wnd = window;
	for (size_t x = 0; x < wnd->width; ++x)
	{
		for (size_t y = wnd->height; y > 0; --y)
		{
			ctr_core_window_set_pixel(window, x, y, pixel);
		}
	}
}

size_t ctr_core_window_get_width(const void *window)
{
	const ctr_core_window *wnd = window;
	return wnd->width;
}

size_t ctr_core_window_get_height(const void *window)
{
	const ctr_core_window *wnd = window;
	return wnd->height;
}

uint32_t ctr_core_window_get_pixel(const void *window, size_t x, size_t y)
{
	const ctr_core_window *wnd = window;
	return ctr_core_screen_get_pixel(wnd->parent, wnd->x + x, wnd->y + y);
}

void ctr_core_window_set_pixel(void *window, size_t x, size_t y, uint32_t pixel)
{
	ctr_core_window *wnd = window;
	ctr_core_screen_set_pixel(wnd->parent, wnd->x + x, wnd->y + y, pixel);
}

ctr_core_screen *ctr_core_window_get_screen(void *window)
{
	ctr_core_window *wnd = window;
	return wnd->parent;
}

void ctr_core_window_draw_bitmap(ctr_core_window *window, size_t x, size_t y, uint32_t pixel, ctr_core_screen_bitmap *bitmap)
{
	ctr_core_screen_draw_bitmap(window->parent, window->x + x, window->y + y, pixel, bitmap);
}

