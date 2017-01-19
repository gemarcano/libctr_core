#include <cstdint>
#include <cstddef>

namespace ctr_core
{
	namespace detail
	{
		template<pixel_format>
		std::uint8_t pixel_get(const uint8_t *data, std::size_t index);
		
		template<>
		std::uint8_t pixel_get<pixel_format::RGBA8>(const uint8_t *data, std::size_t index)
		{
			return data[index];
		}

		template<>
		std::uint8_t pixel_get<pixel_format::RGB8>(const uint8_t *data, std::size_t index)
		{
			return data[index];
		}

		template<>
		std::uint8_t pixel_get<pixel_format::RGB565>(const uint8_t *data, std::size_t index)
		{
			std::uint8_t result;
			switch (index)
			{
				case 0:
					return data[0] & 0x1F;
				case 1:
					return data[0] >> 0x1F | ((data[1] & 0x7) << 3);
				case 2:
					return data[1] >> 3;
				default:
					return 255; //undefined behavior, index should be within bounds, so just return garbage
			}
		}

		template<>
		std::uint8_t pixel_get<pixel_format::A1_RGB5>(const uint8_t *data, std::size_t index)
		{
			switch (index)
			{
				case 0: return data[0] & 0x1;
				case 1: return (data[0] >> 1) & 0x1F;
				case 2: return (data[1] & 0x3)<< 2 | data[0] >> 6;
				case 3: return data[1] >> 3;
				default: return 255;
			}
		}

		template<>
		std::uint8_t pixel_get<pixel_format::RGBA4>(const uint8_t *data, std::size_t index)
		{
			if (index & 0x1) return data[index/2] >> 4;
			return data[index/2] & 0xF;
		}

	}

	std::uint8_t pixel<pixel_format::RGBA8>::operator[](std::size_t index) const
	{
		return detail::pixel_get<pixel_format::RGBA8>(&data[0], index);
	}

	std::uint8_t pixel<pixel_format::RGB8>::operator[](std::size_t index) const
	{
		return detail::pixel_get<pixel_format::RGB8>(&data[0], index);
	}

	std::uint8_t pixel<pixel_format::RGB565>::operator[](std::size_t index) const
	{
		return detail::pixel_get<pixel_format::RGB565>(&data[0], index);
	}

	std::uint8_t pixel<pixel_format::A1_RGB5>::operator[](std::size_t index) const
	{
		return detail::pixel_get<pixel_format::A1_RGB5>(&data[0], index);
	}

	std::uint8_t pixel<pixel_format::RGBA4>::operator[](std::size_t index) const
	{
		return detail::pixel_get<pixel_format::RGBA4>(&data[0], index);
	}

	generic_pixel::generic_pixel(unsigned char *buffer, pixel_format format)
	:buffer_(buffer), format_(format)
	{}

	std::uint8_t generic_pixel::operator[](std::size_t index) const
	{
		switch(format_)
		{
			case pixel_format::RGBA8:
				return detail::pixel_get<pixel_format::RGBA8>(buffer_, index);
			case pixel_format::RGB8:
				return detail::pixel_get<pixel_format::RGB8>(buffer_, index);
			case pixel_format::RGB565:
				return detail::pixel_get<pixel_format::RGB565>(buffer_, index);
			case pixel_format::A1_RGB5:
				return detail::pixel_get<pixel_format::A1_RGB5>(buffer_, index);
			case pixel_format::RGBA4:
				return detail::pixel_get<pixel_format::RGBA4>(buffer_, index);
			default: return 0;
		}
	}
}

