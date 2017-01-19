/*******************************************************************************
 * Copyright (C) 2016 Gabriel Marcano
 *
 * Refer to the COPYING.txt file at the top of the project directory. If that is
 * missing, this file is licensed under the GPL version 2.0 or later.
 *
 ******************************************************************************/

/** @file */

#ifndef CTR_CORE_PIXEL_HPP_
#define CTR_CORE_PIXEL_HPP_

#include <array>

namespace ctr_core
{
	enum class pixel_format
	{
		RGBA8,
		RGB8,
		RGB565,
		A1_RGB5,
		RGBA4
	};

	namespace detail
	{
		constexpr std::size_t pixel_format_size(pixel_format format)
		{
			switch (format)
			{
				case pixel_format::RGBA8: return 4;
				case pixel_format::RGB8: return 3;
				case pixel_format::RGB565:
				case pixel_format::A1_RGB5:
				case pixel_format::RGBA4: return 2;
				default: return 255;
			}
		}
	}

	template<pixel_format format>
	class pixel;

	template<>
	class pixel<pixel_format::RGBA8>
	{
	public:
		std::uint8_t operator[](std::size_t index) const;

	private:
		std::array<unsigned char, detail::pixel_format_size(pixel_format::RGBA8)> data;
	};

	template<>
	class pixel<pixel_format::RGB8>
	{
	public:
		std::uint8_t operator[](std::size_t index) const;

	private:
		std::array<unsigned char, detail::pixel_format_size(pixel_format::RGB8)> data;
	};

	template<>
	class pixel<pixel_format::RGB565>
	{
	public:
		std::uint8_t operator[](std::size_t index) const;

	private:
		std::array<unsigned char, detail::pixel_format_size(pixel_format::RGB565)> data;
	};

	template<>
	class pixel<pixel_format::A1_RGB5>
	{
	public:
		std::uint8_t operator[](std::size_t index) const;

	private:
		std::array<unsigned char, detail::pixel_format_size(pixel_format::A1_RGB5)> data;
	};

	template<>
	class pixel<pixel_format::RGBA4>
	{
	public:
		std::uint8_t operator[](std::size_t index) const;

	private:
		std::array<unsigned char, detail::pixel_format_size(pixel_format::RGBA4)> data;
	};

	class generic_pixel
	{
	public:
		generic_pixel(unsigned char *buffer, pixel_format format);
		generic_pixel(const generic_pixel& pixel);
		generic_pixel(std::uint32_t pixel);
		std::uint8_t operator[](std::size_t index) const;
		generic_pixel& operator=(std::uint32_t pixel);
		generic_pixel& operator=(const generic_pixel& pixel);
		const unsigned char *get_buffer() const;
		unsigned char *get_buffer();
		std::uint32_t get_value() const;

	private:
		unsigned char *buffer_;
		pixel_format format_;
	};

}

#include "ctr_core_pixel.ipp"

#endif//CTR_CORE_PIXEL_HPP_

