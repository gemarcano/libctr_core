#include <ctr_core/ctr_core_window.h>
#include <ctr_core/ctr_core_i2c.h>
#include <ctr_core/ctr_core_screen.h>

#include <climits>

ctr_core_window *ctr_core_window_initialize(ctr_core_screen *screen, std::size_t x, std::size_t y, std::size_t width, std::size_t height)
{
	auto &scr = *reinterpret_cast<ctr_core::generic_screen*>(screen);
	auto result = new ctr_core::generic_window(scr, width, height, x, y);
	return reinterpret_cast<ctr_core_window*>(result);
}

void ctr_core_window_destroy(ctr_core_window *window)
{
	delete reinterpret_cast<ctr_core::generic_window*>(window);
}

