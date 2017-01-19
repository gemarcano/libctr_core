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
	auto result = new ctr_core::generic_screen(framebuffer, width, height, static_cast<ctr_core::pixel_format>(format));
	return reinterpret_cast<ctr_core_screen*>(result);
}

void ctr_core_screen_destroy(ctr_core_screen *screen)
{
	delete reinterpret_cast<ctr_core::generic_screen*>(screen);
}

void ctr_core_screen_clear(void *screen, std::uint32_t pixel)
{
	ctr_core::generic_screen *scr = reinterpret_cast<ctr_core::generic_screen*>(screen);
	ctr_core::generic_pixel px(pixel);
	scr->clear(px);
}

std::size_t ctr_core_screen_get_width(const void *screen)
{
	const ctr_core::generic_screen *scr = reinterpret_cast<const ctr_core::generic_screen*>(screen);
	return scr->width();
}

std::size_t ctr_core_screen_get_height(const void *screen)
{
	const ctr_core::generic_screen *scr = reinterpret_cast<const ctr_core::generic_screen*>(screen);
	return scr->height();
}

std::uint32_t ctr_core_screen_get_pixel(const void *screen, std::size_t x, std::size_t y)
{
	const ctr_core::generic_screen& scr = *reinterpret_cast<const ctr_core::generic_screen*>(screen);
	ctr_core::generic_pixel pixel = scr.get_pixel(x, y);
	std::uint32_t result = 0;
	for (std::size_t i = 0; i < scr.pixel_size(); ++i)
	{
		result |= static_cast<std::uint32_t>(pixel[i]) << (CHAR_BIT*i);
	}
	return result; 
}

void ctr_core_screen_set_pixel(void *screen, std::size_t x, std::size_t y, std::uint32_t pixel)
{
	ctr_core::generic_screen *scr = reinterpret_cast<ctr_core::generic_screen*>(screen);
}

ctr_core_screen *ctr_core_screen_get_screen(void *screen)
{
	return reinterpret_cast<ctr_core_screen*>(screen);
}

void ctr_core_screen_draw_bitmap(ctr_core_screen *screen, std::size_t x, std::size_t y, std::uint32_t pixel, ctr_core_screen_bitmap *bitmap)
{
	if (bitmap->width && bitmap->height)
	{
		std::uint8_t *data = reinterpret_cast<unsigned char*>(bitmap->data);
		std::size_t width_bytes = bitmap->width / 8u;
		if (bitmap->width % 8) width_bytes++;

		for (std::size_t i = 0; i < bitmap->width; ++i)
		{
			std::size_t byte = i/8;
			std::size_t bit = 7 - i%8;
			for (std::size_t j = bitmap->height-1; j < bitmap->height; --j)
			{
				if (data[byte + width_bytes * j] & (1u << bit))
				{
					ctr_core_screen_set_pixel(screen, x + i, y + j, pixel);
				}
			}
		}
	}
}

