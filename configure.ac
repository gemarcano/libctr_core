AC_INIT([libctr_core], [0.0.1], [], [])
AM_INIT_AUTOMAKE([foreign no-dist-gzip dist-xz subdir-objects])

#Find AR and RANLIB for lto support before initializing libtool
AC_CHECK_TOOL([AR],gcc-ar)
AC_CHECK_TOOL([RANLIB],gcc-ranlib)
LT_INIT

AC_PROG_CC
AC_PROG_CPP
AC_PROG_CC_STDC
AC_PROG_INSTALL
AC_CHECK_TOOL([OBJCOPY],objcopy)
AM_PROG_AS

AC_CHECK_HEADERS([stddef.h stdint.h stdlib.h string.h])

AC_CHECK_HEADER_STDBOOL
AC_C_INLINE
AC_TYPE_SIZE_T
AC_TYPE_SSIZE_T
AC_TYPE_UINT16_T
AC_TYPE_UINT32_T
AC_TYPE_UINT8_T

AC_CHECK_FUNCS([malloc memmove memset strchr strtol])

AX_CFLAGS_WARN_ALL
AX_COMPILER_FLAGS([-Wpedantic -Wall -Wextra -Wcast-align -Wcast-qual\
 -Wdisabled-optimization -Wformat=2 -Winit-self -Wlogical-op\
 -Wmissing-declarations -Wmissing-include-dirs -Wredundant-decls\
 -Wshadow -Wsign-conversion -Wstrict-overflow=5 -Wswitch-default\
 -Wundef -Wno-unused])
AX_CHECK_COMPILE_FLAG(-Wno-declaration-after-statement,
	[WARN_CFLAGS="$WARN_CFLAGS -Wno-declaration-after-statement -Wno-error=declaration-after-statement"], [], [], [])

AX_GCC_ARCHFLAG([no],
	[
		MACHINE_CFLAGS="$ax_cv_gcc_archflag"
		AC_SUBST(MACHINE_CFLAGS)
	] , [AC_MSG_ERROR([configure was unable to identify the\
 architecture being targeted. Please specify the architecture via the\
 --with-gcc-arch option.])])

AC_ARG_ENABLE(3ds-arm11,
	AS_HELP_STRING([--enable-3ds-arm11],
	[Enable flags used for compatibility with devKitARM ARM11 libc]))

AC_ARG_ENABLE(thumb,
	AS_HELP_STRING([--enable-thumb],
	[Enable flags commonly used to compile with Thumb support]))

AS_IF([test "x$enable_3ds_arm11" = "xyes"], [
	AS_IF([test "x$enable_thumb" = "xyes"], [
		AC_MSG_ERROR([Cannot mix thumb with hard float support])
	])
	C11FLAGS="-mfloat-abi=hard -mtp=soft"
	AC_SUBST(C11FLAGS)
])

AS_IF([test "x$enable_thumb" = "xyes"], [
	THUMBFLAGS="-mthumb -mthumb-interwork"
	AC_SUBST(THUMBFLAGS)
])

AC_CONFIG_FILES([Makefile src/Makefile libctr_core.pc])
AC_OUTPUT

