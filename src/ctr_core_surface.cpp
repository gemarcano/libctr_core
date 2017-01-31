#include <ctr_core/ctr_core_surface.h>

#include <climits>
#include <cstring>

std::size_t ctr_core_surface_get_width(const void *surface)
{
	const ctr_core::generic_surface& surf = *reinterpret_cast<const ctr_core::generic_surface*>(surface);
	return surf.width();
}

std::size_t ctr_core_surface_get_height(const void *surface)
{
	const ctr_core::generic_surface& surf = *reinterpret_cast<const ctr_core::generic_surface*>(surface);
	return surf.height();
}

std::uint32_t ctr_core_surface_get_pixel(const void *surface, std::size_t x, std::size_t y)
{
	const ctr_core::generic_surface& surf = *reinterpret_cast<const ctr_core::generic_surface*>(surface);
	const ctr_core::generic_pixel pixel = surf(x, y);
	std::uint32_t result = 0;
	for (std::size_t i = 0; i < pixel.size(); ++i)
	{
		result |= static_cast<std::uint32_t>(pixel.data()[i]) << (CHAR_BIT*i);
	}
	return result;
}

void ctr_core_surface_set_pixel(void *surface, std::size_t x, std::size_t y, std::uint32_t pixel)
{
	ctr_core::generic_surface& surf = *reinterpret_cast<ctr_core::generic_surface*>(surface);
	surf(x, y) = reinterpret_cast<unsigned char*>(&pixel);
}

struct ctr_core_screen *ctr_core_surface_get_screen(void *surface)
{
	ctr_core::generic_surface& surf = *reinterpret_cast<ctr_core::generic_surface*>(surface);
	return reinterpret_cast<ctr_core_screen*>(&surf.get_screen());
}

void ctr_core_surface_clear(void *surface, std::uint32_t pixel)
{
	ctr_core::generic_surface& surf = *reinterpret_cast<ctr_core::generic_surface*>(surface);
	surf.clear(surf(0, 0) = reinterpret_cast<unsigned char*>(&pixel));
}

void ctr_core_surface_draw_bitmap(void *surface, std::size_t x, std::size_t y, std::uint32_t pixel, ctr_core_surface_bitmap *bitmap)
{
	auto& surf = *reinterpret_cast<ctr_core::generic_surface*>(surface);
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
					surf(x + i, y + j) = reinterpret_cast<unsigned char*>(&pixel);
				}
			}
		}
	}
}

namespace ctr_core
{
	template<class Pixel>
	void surface<Pixel>::clear(const pixel_type& pixel)
	{
		//Buffer is bottom to top, left to right
		//For my own sanity, I am defining that the framebuffer pixels are within is
		//one large array object. This allows for pointer arithmetic.
		size_t h = height();
		size_t pixel_size = pixel.size();
		unsigned char *data = operator()(0, h-1).data();
		std::size_t offset_to_next_column = h * pixel_size;

		//Write into the first column
		for (size_t y = 0; y < h; ++y)
		{
			operator()(0,y) = pixel;
		}
		data += offset_to_next_column;

		for (std::size_t x = 1; x < width(); ++x)
		{
			memmove(data, data - offset_to_next_column, offset_to_next_column);
			data += offset_to_next_column;
		}
	}

}

