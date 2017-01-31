#include <ctr_core/ctr_core_window.h>

ctr_core_window *ctr_core_window_initialize(ctr_core_screen *screen, std::size_t x, std::size_t y, std::size_t width, std::size_t height)
{
	auto &scr = *reinterpret_cast<ctr_core::generic_screen*>(screen);
	void *data = malloc(sizeof(ctr_core::generic_window));
	auto result = new(data) ctr_core::generic_window(scr, width, height, x, y);
	return reinterpret_cast<ctr_core_window*>(result);
}

void ctr_core_window_destroy(ctr_core_window *window)
{
	reinterpret_cast<ctr_core::generic_window*>(window)->~generic_window();
	free(window);
}

namespace ctr_core
{
	generic_window::generic_window(generic_surface& parent, size_t width, size_t height, size_t x, size_t y)
	:parent_(parent), width_(width), height_(height), x_(x), y_(y)
	{}

	size_t generic_window::width() const
	{
		return width_;
	}

	size_t generic_window::height() const
	{
		return height_;
	}

	generic_window::pixel_type generic_window::operator()(size_t x, size_t y)
	{
		return parent_.operator()(x_ + x, y_ + y);
	}

	const generic_window::pixel_type generic_window::operator()(size_t x, size_t y) const
	{
		return parent_.operator()(x_ + x, y_ + y);
	}

	generic_surface& generic_window::get_screen()
	{
		return parent_.get_screen();
	}

	const generic_surface& generic_window::get_screen() const
	{
		return parent_.get_screen();
	}

	void generic_window::clear(const pixel_type& pixel)
	{

	}
}

