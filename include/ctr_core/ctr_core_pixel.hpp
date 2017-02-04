/*******************************************************************************
 * Copyright (C) 2016, 2017 Gabriel Marcano
 *
 * Refer to the COPYING.txt file at the top of the project directory. If that is
 * missing, this file is licensed under the GPL version 2.0 or later.
 *
 ******************************************************************************/

/** @file */

#ifndef CTR_CORE_PIXEL_HPP_
#define CTR_CORE_PIXEL_HPP_

#include <cstdint>

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

	template<pixel_format format>
	class pixel
	{
	public:
		explicit pixel(unsigned char *data);
		unsigned char *data();
		const unsigned char *data() const;
		std::uint8_t operator[](std::size_t index) const;
		pixel& operator=(const unsigned char *data);
		pixel& operator=(const pixel& pixel);
		static constexpr std::size_t size();

	private:
		unsigned char *data_;
	};

	class generic_pixel_impl;

	class generic_pixel
	{
	public:
		generic_pixel(unsigned char *data, pixel_format format);
		unsigned char *data();
		const unsigned char *data() const;
		std::uint8_t operator[](std::size_t index) const;
		std::size_t size() const;
		generic_pixel& operator=(const unsigned char *data);
	private:
		unsigned char *data_;
		pixel_format format_;
		generic_pixel_impl *impl_;
	};

}

#include "ctr_core_pixel.ipp"

#endif//CTR_CORE_PIXEL_HPP_

