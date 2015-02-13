# x11.m4
# Author: Aaron Caffrey
# TEST_X11() function in configure.ac
#
# Check for the presence of X11 headers and
# substitute the linker flags -lX11 to the
# the variable 'X_LIBS' if they are available.
# Otherwise test whether the user is running
# Linux or Solaris, and if so, fallback to 3rd party
# programs such as `lspci' and `xdpyinfo'
# Like the rest of screenfetch-c, this file is licensed under the MIT license.

AC_DEFUN([TEST_X11],
[
	X_LIBS=""
	xorg_here="no"

	AC_CHECK_HEADERS([X11/Xlib.h], [
		X_LIBS="-lX11"
		xorg_here="yes"
		AC_SUBST(X_LIBS)
	])


	if test "$xorg_here" = "no"; then
		AC_CHECK_PROG(UNAME, [uname], [yes], [no])
			if test "$UNAME" = "yes"; then
				uname_s=`uname -s`

				if test "$uname_s" = "Linux" || test "$uname_s" = "SunOS"; then
					AC_CHECK_PROG(LSPCI, [lspci], [yes], [no])
					AC_CHECK_PROG(XDPYINFO, [xdpyinfo], [yes], [no])

				if test "$LSPCI" = "no"; then
					AC_MSG_ERROR([Unable to find the lspci program. Install X headers and/or lspci in order to compile the program. Currently you have neither X headers nor lspci.])
				fi
			fi
		fi
	fi
])
