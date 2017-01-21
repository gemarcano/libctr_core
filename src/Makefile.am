include $(top_srcdir)/common.mk

lib_LTLIBRARIES = libctr_core.la
libctr_core_la_CFLAGS=$(AM_CFLAGS) $(C9FLAGS)
libctr_core_la_CCASFLAGS = -I$(top_srcdir)/data
libctr_core_la_SOURCES = ctr_core_pxi.c ctr_core_screen.cpp ctr_core_i2c.c ctr_core_system.cpp $(top_srcdir)/data/ctr_core_font_data.s ctr_core_freetype.c ctr_core_console.cpp ctr_core_hid.c ctr_core_circular_buffer.c ctr_core_headers.c ctr_core_firm.c ctr_core_window.cpp ctr_core_led.c ctr_core_surface.cpp ctr_core_pixel.cpp
libctr_core_la_LDFLAGS = $(AM_LDFLAGS) -static

libctr_coredir=$(pkgdatadir)
libctr_core_DATA=linker.ld

EXTRA_DIST=screen_init.ld

includedirectory = $(top_srcdir)/include

ctrincludedir = $(includedir)/ctr_core

ctrinclude_HEADERS = $(includedirectory)/ctr_core/ctr_core_pxi.h \
$(includedirectory)/ctr_core/ctr_core_screen.h \
$(includedirectory)/ctr_core/ctr_core_i2c.h \
$(includedirectory)/ctr_core/ctr_core_system.h \
$(includedirectory)/ctr_core/ctr_core_freetype.h \
$(includedirectory)/ctr_core/ctr_core_console.h \
$(includedirectory)/ctr_core/ctr_core_hid.h \
$(includedirectory)/ctr_core/ctr_core_circular_buffer.h \
$(includedirectory)/ctr_core/ctr_core_headers.h \
$(includedirectory)/ctr_core/ctr_core_firm.h \
$(includedirectory)/ctr_core/ctr_core_window.h \
$(includedirectory)/ctr_core/ctr_core_led.h \
$(includedirectory)/ctr_core/ctr_core_surface.h \
$(includedirectory)/ctr_core/ctr_core_pixel.hpp \
$(includedirectory)/ctr_core/ctr_core_screen.ipp


