#include <ctr_core/ctr_core_pixel.hpp>
#include <climits>
#include <new>

namespace ctr_core
{
	class generic_pixel_impl
	{
	public:
		~generic_pixel_impl() = default;
		virtual std::uint8_t at(unsigned char *data, std::size_t index) const = 0;
		virtual std::size_t size() const = 0;
		virtual generic_pixel_impl& set(unsigned char *data, const unsigned char *src) = 0;
	};

	template<class Pixel>
	class generic_pixel_impl_ : public generic_pixel_impl
	{
	public:
		typedef Pixel pixel_type;

		virtual std::uint8_t at(unsigned char *data, std::size_t index) const override
		{
			return Pixel(data)[index];
		}

		virtual std::size_t size() const override
		{
			return Pixel(nullptr).size();
		}

		virtual generic_pixel_impl& set(unsigned char *data, const unsigned char *src) override
		{
			Pixel pix(data);
			pix = src;
			return *this;
		}
	};

	namespace detail
	{
		static generic_pixel_impl_<pixel<pixel_format::RGBA8>> pix_rgba8;
		static generic_pixel_impl_<pixel<pixel_format::RGB8>> pix_rgb8;
		static generic_pixel_impl_<pixel<pixel_format::RGB565>> pix_rgb565;
		static generic_pixel_impl_<pixel<pixel_format::A1_RGB5>> pix_a1_rgb5;
		static generic_pixel_impl_<pixel<pixel_format::RGBA4>> pix_rgba4;
	}

	generic_pixel::generic_pixel(unsigned char *data, pixel_format format)
	:data_(data), format_(format)
	{
		switch(format_)
		{
			case pixel_format::RGBA8:
				impl_ = &detail::pix_rgba8;
				break;
			case pixel_format::RGB8:
				impl_ = &detail::pix_rgb8;
				break;
			case pixel_format::RGB565:
				impl_ = &detail::pix_rgb565;
				break;
			case pixel_format::A1_RGB5:
				impl_ = &detail::pix_a1_rgb5;
				break;
			case pixel_format::RGBA4:
				impl_ = &detail::pix_rgba4;
				break;
			default:
				impl_ = nullptr;
		}
	}

	unsigned char *generic_pixel::data()
	{
		return data_;
	}

	const unsigned char *generic_pixel::data() const
	{
		return data_;
	}

	std::uint8_t generic_pixel::operator[](std::size_t index) const
	{
		return impl_->at(data_, index);
	}

	std::size_t generic_pixel::size() const
	{
		return impl_->size();
	}

	generic_pixel& generic_pixel::operator=(const unsigned char *data)
	{
		impl_->set(data_, data);
		return *this;
	}

	namespace detail
	{
		template<>
		std::uint8_t pixel_get<pixel_format::RGBA8>(const std::uint8_t *data, std::size_t index)
		{
			return data[index];
		}

		template<>
		std::uint8_t pixel_get<pixel_format::RGB8>(const std::uint8_t *data, std::size_t index)
		{
			return data[index];
		}

		template<>
		std::uint8_t pixel_get<pixel_format::RGB565>(const std::uint8_t *data, std::size_t index)
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
		std::uint8_t pixel_get<pixel_format::A1_RGB5>(const std::uint8_t *data, std::size_t index)
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
		std::uint8_t pixel_get<pixel_format::RGBA4>(const std::uint8_t *data, std::size_t index)
		{
			if (index & 0x1) return data[index/2] >> 4;
			return data[index/2] & 0xF;
		}
	}
}

