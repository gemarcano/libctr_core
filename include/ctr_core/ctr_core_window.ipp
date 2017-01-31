
namespace ctr_core
{
	template<class Parent, std::size_t Width, std::size_t Height>
	window::window(Parent& parent, size_t x, size_t y)
	:parent_(parent), x_(x), y_(y)
	{}

	template<class Parent, std::size_t Width, std::size_t Height>
	constexpr size_t window::width() const
	{
		return Width;
	}

	template<class Parent, std::size_t Width, std::size_t Height>
	constexpr size_t window::height() const
	{
		return Height;
	}

	template<class Parent, std::size_t Width, std::size_t Height>
	pixel_type& window::operator()(size_t x, size_t y)
	{
		return parent_(x + x_, y + y_);
	}

	template<class Parent, std::size_t Width, std::size_t Height>
	const pixel_type& window::operator()(size_t x, size_t y) const
	{
		return parent_(x + x_, y + y_);
	}

	template<class Parent, std::size_t Width, std::size_t Height>
	Parent& window::get_parent()
	{
		return parent_;
	}

	template<class Parent, std::size_t Width, std::size_t Height>
	const Parent& window::get_parent() const
	{
		return parent_;
	}

	template<class Parent, std::size_t Width, std::size_t Height>
	void window::clear(const pixel_type& pixel)
	{

	}
}

