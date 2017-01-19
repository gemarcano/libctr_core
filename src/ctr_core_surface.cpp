#include <ctr_core/ctr_core_surface.h>

#include <climits>

size_t ctr_core_surface_get_width(const void *surface)
{
	const ctr_core::generic_surface& surf = *reinterpret_cast<const ctr_core::generic_surface*>(surface);
	return surf.width();
}

size_t ctr_core_surface_get_height(const void *surface)
{
	const ctr_core::generic_surface& surf = *reinterpret_cast<const ctr_core::generic_surface*>(surface);
	return surf.height();
}

uint32_t ctr_core_surface_get_pixel(const void *surface, size_t x, size_t y)
{
	const ctr_core::generic_surface& surf = *reinterpret_cast<const ctr_core::generic_surface*>(surface);
	const ctr_core::generic_pixel pixel = surf.get_pixel(x, y);
	uint32_t result = 0;
	for (size_t i = 0; i < surf.pixel_size(); ++i)
	{
		result |= static_cast<unsigned>(pixel[i]) << (CHAR_BIT*i);
	}
	return result;
}

void ctr_core_surface_set_pixel(void *surface, size_t x, size_t y, uint32_t pixel)
{
	ctr_core::generic_surface& surf = *reinterpret_cast<ctr_core::generic_surface*>(surface);
	surf.set_pixel(x, y, pixel);
}

struct ctr_core_screen *ctr_core_surface_get_screen(void *surface)
{
	ctr_core::generic_surface& surf = *reinterpret_cast<ctr_core::generic_surface*>(surface);
	return reinterpret_cast<ctr_core_screen*>(&surf.get_screen());
}

void ctr_core_surface_clear(void *surface, uint32_t pixel)
{
	ctr_core::generic_surface& surf = *reinterpret_cast<ctr_core::generic_surface*>(surface);
	surf.clear(ctr_core::generic_pixel(pixel));
}

