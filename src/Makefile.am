AM_CPPFLAGS = -I$(top_srcdir)/include

SUBDIRS = cpu debug dos fpu gui hardware libs ints misc shell platform custom

bin_PROGRAMS = dosbox

if HAVE_WINDRES
ico_stuff = winres.rc
endif

.rc.o:
	$(WINDRES) -o $@ $<

dosbox_SOURCES = dosbox.cpp $(ico_stuff)
dosbox_LDADD = custom/libcustom.a cpu/libcpu.a debug/libdebug.a dos/libdos.a fpu/libfpu.a  hardware/libhardware.a gui/libgui.a \
               ints/libints.a misc/libmisc.a shell/libshell.a hardware/serialport/libserial.a libs/gui_tk/libgui_tk.a custom/schick/libschick.a custom/schick/rewrite_m302de/libschick_rewrite_m302de.a custom/schick/rewrite_g105de/libschick_rewrite_g105de.a custom/schweif/libschweif.a custom/schweif/rewrite_c102de/libschweif_rewrite_c102de.a

EXTRA_DIST = winres.rc dosbox.ico


