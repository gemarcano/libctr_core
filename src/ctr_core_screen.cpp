#include <ctr_core/ctr_core_screen.h>
#include <ctr_core/ctr_core_i2c.h>
#include <ctr_core/ctr_core_pixel.hpp>

#include <climits>

ctr_core_screen *ctr_screen_top;
ctr_core_screen *ctr_screen_bottom;

void ctr_core_screen_enable_backlight(ctr_core_screen_enum aScreens)
{
	int data = 0;
	if (aScreens & CTR_SCREEN_TOP)
	{
		data |= 1 << 5;
	}

	if (aScreens & CTR_SCREEN_BOTTOM)
	{
		data |= 1 << 3;
	}

	ctr_core_i2cWriteRegister(I2C_DEV_MCU, 0x22, data);
}

void ctr_core_screen_disable_backlight(ctr_core_screen_enum aScreens)
{
	int data = 0;
	if (aScreens & CTR_SCREEN_TOP)
	{
		data |= 1 << 4;
	}

	if (aScreens & CTR_SCREEN_BOTTOM)
	{
		data |= 1 << 2;
	}
	ctr_core_i2cWriteRegister(I2C_DEV_MCU, 0x22, data);
}

ctr_core_screen *ctr_core_screen_initialize(std::uint8_t *framebuffer, std::size_t width, std::size_t height, ctr_core_screen_pixel format)
{
	auto screen = new(std::nothrow) ctr_core::generic_screen(framebuffer, width, height, static_cast<ctr_core::pixel_format>(format));
	return reinterpret_cast<ctr_core_screen*>(screen);
}

void ctr_core_screen_destroy(ctr_core_screen *screen)
{
	delete reinterpret_cast<ctr_core::generic_screen*>(screen);
}

namespace ctr_core
{
	generic_screen::generic_screen(std::uint8_t *framebuffer, std::size_t width, std::size_t height, pixel_format format)
	:framebuffer_(framebuffer), width_(width), height_(height), format_(format)
	{}

	std::size_t generic_screen::width() const
	{
		return width_;
	}

	std::size_t generic_screen::height() const
	{
		return height_;
	}

	generic_pixel generic_screen::operator()(std::size_t x, std::size_t y)
	{
		return static_cast<const generic_screen*>(this)->operator()(x, y);
	}

	const generic_pixel generic_screen::operator()(std::size_t x, std::size_t y) const
	{
		std::size_t pixel_size = detail::pixel_format_size(format_);
		auto ptr = framebuffer_ + ((x * height_) + ((height_ - 1 - y))) * pixel_size;
		return generic_pixel(ptr, format_);
	}

	generic_surface& generic_screen::get_screen()
	{
		return *this;
	}

	const generic_surface& generic_screen::get_screen() const
	{
		return *this;
	}

	void generic_screen::clear(const generic_pixel& pixel)
	{
		for (std::size_t x = 0; x < width_; ++x)
		{
			for(std::size_t y = 0; y < height_; ++y)
			{
				operator()(x, y) = pixel;
			}
		}
	}
}

