/*******************************************************************************
 * Copyright (C) 2016, 2017 Gabriel Marcano
 *
 * Refer to the COPYING.txt file at the top of the project directory. If that is
 * missing, this file is licensed under the GPL version 2.0 or later.
 *
 ******************************************************************************/

#include <cstring>

namespace ctr_core
{
	template<class Pixel, std::size_t Width, std::size_t Height>
	screen<Pixel, Width, Height>::screen(unsigned char *framebuffer)
	:framebuffer_(framebuffer)
	{}

	template<class Pixel, std::size_t Width, std::size_t Height>
	constexpr std::size_t screen<Pixel, Width, Height>::width() const
	{
		return Width;
	}

	template<class Pixel, std::size_t Width, std::size_t Height>
	constexpr std::size_t screen<Pixel, Width, Height>::height() const
	{
		return Height;
	}

	template<class Pixel, std::size_t Width, std::size_t Height>
	Pixel screen<Pixel, Width, Height>::operator()(std::size_t x, std::size_t y)
	{
		//x = y[a][b]
		//x = *((char *)y + a * NUMBER_OF_COLUMNS + b);
		//framebuffer_[Width][Height]
		//return framebuffer_[x][(Height-1) - y];
		return pixel_type(framebuffer_ + x * Height + (Height-1 -y));
	}

	template<class Pixel, std::size_t Width, std::size_t Height>
	const Pixel screen<Pixel, Width, Height>::operator()(std::size_t x, std::size_t y) const
	{
		return pixel_type(framebuffer_ + x * Height + (Height-1 -y));
	}

	template<class Pixel, std::size_t Width, std::size_t Height>
	screen<Pixel, Width, Height>& screen<Pixel, Width, Height>::get_screen()
	{
		return *this;
	}

	template<class Pixel, std::size_t Width, std::size_t Height>
	const screen<Pixel, Width, Height>& screen<Pixel, Width, Height>::get_screen() const
	{
		return *this;
	}

	template<class Pixel, std::size_t Width, std::size_t Height>
	void screen<Pixel, Width, Height>::clear(const pixel_type& pixel)
	{
		// Buffer is bottom to top, left to right
		//For my own sanity, I am defining that the framebuffer pixels are within is
		//one large array object. This allows for pointer arithmetic.
		constexpr std::size_t h = height();
		std::size_t pixel_size = operator()(0,0).size();
		unsigned char *data = framebuffer_;
		std::size_t offset_to_next_column = h * pixel_size;

		//Write into the first column
		for (std::size_t y = 0; y < h; ++y)
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

