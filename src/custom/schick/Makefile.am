AM_CPPFLAGS = -I$(top_srcdir)/include -I./rewrite_m302de

SUBDIRS = rewrite_m302de

noinst_LIBRARIES = libschick.a

EXTRA_DIST = schick.h
libschick_a_SOURCES =	 schick.cpp schick_status.cpp schick_gen105.cpp\
			 schick_m302de.cpp

libschick_a_LIBADD = rewrite_m302de/libschick_rewrite_m302de.a
