#include <ctr_core/ctr_core_surface.h>

size_t ctr_core_surface_get_width(const void *surface)
{
	const ctr_core_surface *surf = surface;
	return surf->get_width(surface);
}

size_t ctr_core_surface_get_height(const void *surface)
{
	const ctr_core_surface *surf = surface;
	return surf->get_height(surface);
}

uint32_t ctr_core_surface_get_pixel(const void *surface, size_t x, size_t y)
{
	const ctr_core_surface *surf = surface;
	return surf->get_pixel(surface, x, y);
}

void ctr_core_surface_set_pixel(void *surface, size_t x, size_t y, uint32_t pixel)
{
	ctr_core_surface *surf = surface;
	surf->set_pixel(surface, x, y, pixel);
}

struct ctr_core_screen *ctr_core_surface_get_screen(void *surface)
{
	ctr_core_surface *surf = surface;
	return surf->get_screen(surface);
}

void ctr_core_surface_clear(void *surface, uint32_t pixel)
{
	ctr_core_surface *surf = surface;
	surf->clear(surface, pixel);
}

