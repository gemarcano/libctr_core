INCPATHS=-I$(top_srcdir)/include/ -I$(prefix)/include/freetype2

#SIZE_OPTIMIZATION = -flto
SIZE_OPTIMIZATION = -Wl,--gc-sections -ffunction-sections
AM_CPPFLAGS=$(INCPATHS)

AM_CFLAGS= -std=gnu11 -O2 -g -fomit-frame-pointer -ffast-math \
	$(THUMBFLAGS) $(SIZE_OPTIMIZATION) $(WARN_CFLAGS) $(MACHINE_CFLAGS) $(C11FLAGS)
AM_LDFLAGS=-Wl,--use-blx,--pic-veneer,-q $(WARN_LDFLAGS)
OCFLAGS=--set-section-flags .bss=alloc,load,contents

