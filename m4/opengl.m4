# opengl.m4
# Author: Aaron Caffrey
# TEST_OPENGL() function in configure.ac
#
# Check for the presence of OpenGL headers and
# substitute the linker flags -lGL to the
# the variable 'GL_LIBS' if they are available
# Like the rest of screenfetch-c, this file is licensed under the MIT license.

AC_DEFUN([TEST_OPENGL],
[
	GL_LIBS=""

	AC_CHECK_HEADERS([GL/glx.h], [
		GL_LIBS="-lGL"
		AC_SUBST(GL_LIBS)
	])
])
