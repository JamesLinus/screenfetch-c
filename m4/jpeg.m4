# jpeg.m4
# Author: Aaron Caffrey
# TEST_JPEG() function in configure.ac
#
# Check for the presence of jpeglib headers and
# substitute the linker flags -ljpeg to the
# the variable 'JPEG_LIBS' if they are available
# Like the rest of screenfetch-c, this file is licensed under the MIT license.

AC_DEFUN([TEST_JPEG],
[
	JPEG_LIBS=""
	AC_CHECK_HEADERS([jpeglib.h], [
		JPEG_LIBS="-ljpeg"
		AC_SUBST(JPEG_LIBS)
	])
])
