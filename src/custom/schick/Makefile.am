AM_CPPFLAGS = -I$(top_srcdir)/include -I$(srcdir)/rewrite_m302de -Wall -pedantic -Wno-variadic-macros

SUBDIRS = rewrite_m302de rewrite_g105de

noinst_LIBRARIES = libschick.a

EXTRA_DIST = schick.h
libschick_a_SOURCES =	 schick.cpp schick_status.cpp\
			 schick_g105de.cpp\
			 schick_m302de.cpp
