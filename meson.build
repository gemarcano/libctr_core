project('libctr_core', 'c',
  default_options: [
    'b_lto=true',
    'b_staticpic=false',
    'c_std=c17',
    'warning_level=3',
  ],
  license: ['GPL-2.0-or-later'],
  version: '0.0.1')

sources = files([
  'src/ctr_core_pxi.c',
  'src/ctr_core_screen.c',
  'src/ctr_core_i2c.c',
  'src/ctr_core_system.c',
  'src/ctr_core_freetype.c',
  'src/ctr_core_console.c',
  'src/ctr_core_hid.c',
  'src/ctr_core_circular_buffer.c',
  'src/ctr_core_headers.c',
  'src/ctr_core_firm.c',
  'data/ctr_core_font_data.s',
])

header_files = files([
  'include/ctr_core/ctr_core_pxi.h',
  'include/ctr_core/ctr_core_screen.h',
  'include/ctr_core/ctr_core_i2c.h',
  'include/ctr_core/ctr_core_system.h',
  'include/ctr_core/ctr_core_freetype.h',
  'include/ctr_core/ctr_core_console.h',
  'include/ctr_core/ctr_core_hid.h',
  'include/ctr_core/ctr_core_circular_buffer.h',
  'include/ctr_core/ctr_core_headers.h',
  'include/ctr_core/ctr_core_firm.h',
])

install_subdir('include', install_dir: 'include', strip_directory: true)

freetype = dependency('freetype2')

lib = library(
  'ctr_core', sources,
  dependencies: freetype,
  include_directories: 'include',
  install: true,
)

pkg = import('pkgconfig')

pkg.generate(lib,
  subdirs: [ 'ctr_core', '.' ],
  description: 'Baremetal ARM library for the Nintendo 3DS, used by libctr9 and libctr11',
  url: 'https://github.com/gemarcano/' + meson.project_name(),
)
