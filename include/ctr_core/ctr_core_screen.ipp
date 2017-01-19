/*******************************************************************************
 * Copyright (C) 2016 Gabriel Marcano
 *
 * Refer to the COPYING.txt file at the top of the project directory. If that is
 * missing, this file is licensed under the GPL version 2.0 or later.
 *
 ******************************************************************************/

namespace ctr_core
{
	template<class Pixel, size_t Width, size_t Height>
	screen<Pixel, Width, Height>::screen(Pixel (*fb)[Width])
	:framebuffer(fb)
	{}

	template<class Pixel, size_t Width, size_t Height>
	constexpr size_t screen<Pixel, Width, Height>::width() const
	{
		return Width;
	}

	template<class Pixel, size_t Width, size_t Height>
	constexpr size_t screen<Pixel, Width, Height>::height() const
	{
		return Height;
	}

	template<class Pixel, size_t Width, size_t Height>
	Pixel& screen<Pixel, Width, Height>::operator()(size_t x, size_t y)
	{
		return framebuffer[x][(Height-1) - y];
	}

	template<class Pixel, size_t Width, size_t Height>
	const Pixel& screen<Pixel, Width, Height>::operator()(size_t x, size_t y) const
	{
		return framebuffer[x][(Height-1) - y];
	}

	template<class Pixel, size_t Width, size_t Height>
	screen<Pixel, Width, Height>& screen<Pixel, Width, Height>::get_screen()
	{
		return *this;
	}

	template<class Pixel, size_t Width, size_t Height>
	const screen<Pixel, Width, Height>& screen<Pixel, Width, Height>::get_screen() const
	{
		return *this;
	}

	template<class Pixel, size_t Width, size_t Height>
	void screen<Pixel, Width, Height>::clear(const pixel_type& pixel)
	{

	}
}

