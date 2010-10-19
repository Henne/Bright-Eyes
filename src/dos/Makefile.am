AM_CPPFLAGS = -I$(top_srcdir)/include

noinst_LIBRARIES = libdos.a
EXTRA_DIST = scsidefs.h wnaspi32.h dos_codepages.h dos_keyboard_layout_data.h
libdos_a_SOURCES = dos.cpp dos_devices.cpp dos_execute.cpp dos_files.cpp dos_ioctl.cpp dos_memory.cpp \
                   dos_misc.cpp dos_classes.cpp dos_programs.cpp dos_tables.cpp \
		   drives.cpp drives.h drive_virtual.cpp drive_local.cpp drive_cache.cpp drive_fat.cpp \
		   drive_iso.cpp dev_con.h dos_mscdex.cpp dos_keyboard_layout.cpp \
		   cdrom.h cdrom.cpp cdrom_ioctl_win32.cpp cdrom_aspi_win32.cpp cdrom_ioctl_linux.cpp cdrom_image.cpp \
		   cdrom_ioctl_os2.cpp schick.cpp schick.h schick_gen105.cpp\
		schick_status.cpp\
		schick_rewrite/seg002.cpp schick_rewrite/seg002.h\
		schick_rewrite/seg003.cpp schick_rewrite/seg003.h\
		schick_rewrite/seg007.cpp schick_rewrite/seg007.h\
		schick_rewrite/seg098.cpp schick_rewrite/seg098.h
