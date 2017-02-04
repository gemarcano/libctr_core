
#include <cstring>

namespace ctr_core
{
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

		template<pixel_format>
		std::uint8_t pixel_get(const std::uint8_t *data, std::size_t index);

		template<>
		std::uint8_t pixel_get<pixel_format::RGBA8>(const std::uint8_t *data, std::size_t index);

		template<>
		std::uint8_t pixel_get<pixel_format::RGB8>(const std::uint8_t *data, std::size_t index);

		template<>
		std::uint8_t pixel_get<pixel_format::RGB565>(const std::uint8_t *data, std::size_t index);

		template<>
		std::uint8_t pixel_get<pixel_format::A1_RGB5>(const std::uint8_t *data, std::size_t index);

		template<>
		std::uint8_t pixel_get<pixel_format::RGBA4>(const std::uint8_t *data, std::size_t index);
	}

	template<pixel_format Format>
	pixel<Format>::pixel(unsigned char *data)
	:data_(data)
	{}

	template<pixel_format Format>
	unsigned char *pixel<Format>::data()
	{
		return data_;
	}

	template<pixel_format Format>
	const unsigned char *pixel<Format>::data() const
	{
		return data_;
	}

	template<pixel_format Format>
	std::uint8_t pixel<Format>::operator[](std::size_t index) const
	{
		return detail::pixel_get<Format>(data_, index);
	}

	template<pixel_format Format>
	constexpr std::size_t pixel<Format>::size()
	{
		return detail::pixel_format_size(Format);
	}

	template<pixel_format Format>
	pixel<Format>& pixel<Format>::operator=(const unsigned char *data)
	{
		memcpy(data_, data, size());
		return *this;
	}

	template<pixel_format Format>
	pixel<Format>& pixel<Format>::operator=(const pixel& pix)
	{
		return operator=(pix.data_);
	}
}

