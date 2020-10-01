AM_CPPFLAGS = \
	-I$(top_srcdir)/groebner/include \
	-I$(top_builddir)/libbrial/include \
	-I$(top_srcdir)/libbrial/include

AM_CXXFLAGS = -ftemplate-depth-100

AM_DEFAULT_SOURCE_EXT = .cc
