AM_CPPFLAGS = -I$(top_srcdir)/include -I./rewrite_o100de -I./rewrite_c102de

SUBDIRS = rewrite_o100de rewrite_c102de

noinst_LIBRARIES = libschweif.a

EXTRA_DIST = schweif.h
libschweif_a_SOURCES =	 schweif.cpp\
			 schweif_o100de.cpp\
			schweif_c102de.cpp

libschweif_a_LIBADD = rewrite_o100de/libschweif_rewrite_o100de.a\
			rewrite_c102de/libschweif_rewrite_c102de.a
