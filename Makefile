# Generated automatically from Makefile.in by configure.
# Makefile.in generated automatically by automake 1.4 from Makefile.am

# Copyright (C) 1994, 1995-8, 1999 Free Software Foundation, Inc.
# This Makefile.in is free software; the Free Software Foundation
# gives unlimited permission to copy and/or distribute it,
# with or without modifications, as long as this notice is preserved.

# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY, to the extent permitted by law; without
# even the implied warranty of MERCHANTABILITY or FITNESS FOR A
# PARTICULAR PURPOSE.





SHELL = /bin/sh

srcdir = .
top_srcdir = .
prefix = /usr/local
exec_prefix = ${prefix}

bindir = ${exec_prefix}/bin
sbindir = ${exec_prefix}/sbin
libexecdir = ${exec_prefix}/libexec
datadir = ${prefix}/share
sysconfdir = ${prefix}/etc
sharedstatedir = ${prefix}/com
localstatedir = ${prefix}/var
libdir = ${exec_prefix}/lib
infodir = ${prefix}/info
mandir = ${prefix}/man
includedir = ${prefix}/include
oldincludedir = /usr/include

DESTDIR =

pkgdatadir = $(datadir)/register
pkglibdir = $(libdir)/register
pkgincludedir = $(includedir)/register

top_builddir = .

ACLOCAL = aclocal
AUTOCONF = autoconf
AUTOMAKE = automake
AUTOHEADER = autoheader

INSTALL = /usr/bin/install -c
INSTALL_PROGRAM = ${INSTALL} $(AM_INSTALL_PROGRAM_FLAGS)
INSTALL_DATA = ${INSTALL} -m 644
INSTALL_SCRIPT = ${INSTALL_PROGRAM}
transform = s,x,x,

NORMAL_INSTALL = :
PRE_INSTALL = :
POST_INSTALL = :
NORMAL_UNINSTALL = :
PRE_UNINSTALL = :
POST_UNINSTALL = :
CC = gcc
LN_S = ln -s
MAKEINFO = makeinfo
MNI_INCLUDES =   -I/usr/local/mni/include 
MNI_LIBS = -lbicpl -lvolume_io -L/usr/local/mni/lib -lminc -lnetcdf
PACKAGE = register
RANLIB = ranlib
VERSION = 1.3.1

SUBDIRS = Graphics Functionality UI_calls_F User_interface Register_resample Tagtoxfm 

FUNC_DIR = Functionality/
UIC_DIR = UI_calls_F/
UI_DIR = User_interface/

Functionality_OBJECTS =  $(FUNC_DIR)initialize/$(ARCH_DIR)/initialize.o $(FUNC_DIR)slices/$(ARCH_DIR)/colour_map.o $(FUNC_DIR)slices/$(ARCH_DIR)/cursor.o $(FUNC_DIR)slices/$(ARCH_DIR)/create_slice.o $(FUNC_DIR)slices/$(ARCH_DIR)/initialize_slice.o $(FUNC_DIR)slices/$(ARCH_DIR)/resample.o $(FUNC_DIR)slices/$(ARCH_DIR)/save_image.o $(FUNC_DIR)slices/$(ARCH_DIR)/set_volume.o $(FUNC_DIR)slices/$(ARCH_DIR)/slices.o $(FUNC_DIR)slices/$(ARCH_DIR)/updates.o $(FUNC_DIR)tags/$(ARCH_DIR)/objects.o $(FUNC_DIR)tags/$(ARCH_DIR)/save_and_load.o $(FUNC_DIR)tags/$(ARCH_DIR)/tag_points.o $(FUNC_DIR)tags/$(ARCH_DIR)/tag_transform.o $(FUNC_DIR)update/$(ARCH_DIR)/update_window.o


User_interface_OBJECTS =  $(UI_DIR)main/$(ARCH_DIR)/main.o $(UI_DIR)main/$(ARCH_DIR)/initialize.o $(UI_DIR)colour_popup/$(ARCH_DIR)/colour_selection.o $(UI_DIR)delete_tags_popup/$(ARCH_DIR)/delete_tags.o $(UI_DIR)event_callbacks/$(ARCH_DIR)/save_image.o $(UI_DIR)event_callbacks/$(ARCH_DIR)/slice_events.o $(UI_DIR)event_callbacks/$(ARCH_DIR)/tag_events.o $(UI_DIR)event_callbacks/$(ARCH_DIR)/utilities.o $(UI_DIR)event_callbacks/$(ARCH_DIR)/window_events.o $(UI_DIR)event_handling/$(ARCH_DIR)/event_callbacks.o $(UI_DIR)event_handling/$(ARCH_DIR)/event_loop.o $(UI_DIR)event_handling/$(ARCH_DIR)/event_viewports.o $(UI_DIR)event_handling/$(ARCH_DIR)/global_events.o $(UI_DIR)event_handling/$(ARCH_DIR)/window_events.o $(UI_DIR)filter_popup/$(ARCH_DIR)/filter_selection.o $(UI_DIR)input/$(ARCH_DIR)/load.o $(UI_DIR)input/$(ARCH_DIR)/load_popup.o $(UI_DIR)layout/$(ARCH_DIR)/colours.o $(UI_DIR)layout/$(ARCH_DIR)/layout.o $(UI_DIR)print_popup/$(ARCH_DIR)/print.o $(UI_DIR)quit_popup/$(ARCH_DIR)/quit.o $(UI_DIR)resampling/$(ARCH_DIR)/resample.o $(UI_DIR)slices/$(ARCH_DIR)/slices.o $(UI_DIR)transform_popup/$(ARCH_DIR)/xform_selection.o $(UI_DIR)value_readout/$(ARCH_DIR)/update.o $(UI_DIR)widget_instances/$(ARCH_DIR)/colour_bar.o $(UI_DIR)widget_instances/$(ARCH_DIR)/initialize.o $(UI_DIR)widget_instances/$(ARCH_DIR)/main_menu_callbacks.o $(UI_DIR)widget_instances/$(ARCH_DIR)/merged_interface.o $(UI_DIR)widget_instances/$(ARCH_DIR)/meter.o $(UI_DIR)widget_instances/$(ARCH_DIR)/position_widgets.o $(UI_DIR)widget_instances/$(ARCH_DIR)/tag_points.o $(UI_DIR)widget_instances/$(ARCH_DIR)/volume_interface.o $(UI_DIR)widgets/$(ARCH_DIR)/buttons.o $(UI_DIR)widgets/$(ARCH_DIR)/sliders.o $(UI_DIR)widgets/$(ARCH_DIR)/text_entry.o $(UI_DIR)widgets/$(ARCH_DIR)/utilities.o $(UI_DIR)widgets/$(ARCH_DIR)/widgets.o $(UI_DIR)windows/$(ARCH_DIR)/lookup.o $(UI_DIR)windows/$(ARCH_DIR)/popup.o $(UI_DIR)windows/$(ARCH_DIR)/update.o


UI_calls_F_OBJECTS = $(UIC_DIR)UI_calls_F.o

bin_PROGRAMS = register

# Adding a dummy source file to register is a work-around for a bug in 
# automake, which does not (in version 1.4) allow linking together
# only objects in subdirectories.

register_SOURCES = dummy.c

CLEANFILES = dummy.c

register_LDADD = $(Functionality_OBJECTS) $(User_interface_OBJECTS) $(UI_calls_F_OBJECTS) 	-LGraphics -lopengl_x_graphics 	-lglut -lGL -lGLU 	$(X_LIBS) $(X_PRE_LIBS) -lX11 $(X_EXTRA_LIBS) 	$(MNI_LIBS) -lm

ACLOCAL_M4 = $(top_srcdir)/aclocal.m4
mkinstalldirs = $(SHELL) $(top_srcdir)/mkinstalldirs
CONFIG_HEADER = config.h
CONFIG_CLEAN_FILES =  Makefile.include
PROGRAMS =  $(bin_PROGRAMS)


DEFS = -DHAVE_CONFIG_H -I. -I$(srcdir) -I.
CPPFLAGS = 
LDFLAGS = 
LIBS = 
X_CFLAGS =  -I/usr/X11R6/include
X_LIBS =  -L/usr/X11R6/lib
X_EXTRA_LIBS = 
X_PRE_LIBS =  -lSM -lICE
register_OBJECTS =  dummy.o
register_DEPENDENCIES =  $(FUNC_DIR)initialize/$(ARCH_DIR)/initialize.o \
$(FUNC_DIR)slices/$(ARCH_DIR)/colour_map.o \
$(FUNC_DIR)slices/$(ARCH_DIR)/cursor.o \
$(FUNC_DIR)slices/$(ARCH_DIR)/create_slice.o \
$(FUNC_DIR)slices/$(ARCH_DIR)/initialize_slice.o \
$(FUNC_DIR)slices/$(ARCH_DIR)/resample.o \
$(FUNC_DIR)slices/$(ARCH_DIR)/save_image.o \
$(FUNC_DIR)slices/$(ARCH_DIR)/set_volume.o \
$(FUNC_DIR)slices/$(ARCH_DIR)/slices.o \
$(FUNC_DIR)slices/$(ARCH_DIR)/updates.o \
$(FUNC_DIR)tags/$(ARCH_DIR)/objects.o \
$(FUNC_DIR)tags/$(ARCH_DIR)/save_and_load.o \
$(FUNC_DIR)tags/$(ARCH_DIR)/tag_points.o \
$(FUNC_DIR)tags/$(ARCH_DIR)/tag_transform.o \
$(FUNC_DIR)update/$(ARCH_DIR)/update_window.o \
$(UI_DIR)main/$(ARCH_DIR)/main.o $(UI_DIR)main/$(ARCH_DIR)/initialize.o \
$(UI_DIR)colour_popup/$(ARCH_DIR)/colour_selection.o \
$(UI_DIR)delete_tags_popup/$(ARCH_DIR)/delete_tags.o \
$(UI_DIR)event_callbacks/$(ARCH_DIR)/save_image.o \
$(UI_DIR)event_callbacks/$(ARCH_DIR)/slice_events.o \
$(UI_DIR)event_callbacks/$(ARCH_DIR)/tag_events.o \
$(UI_DIR)event_callbacks/$(ARCH_DIR)/utilities.o \
$(UI_DIR)event_callbacks/$(ARCH_DIR)/window_events.o \
$(UI_DIR)event_handling/$(ARCH_DIR)/event_callbacks.o \
$(UI_DIR)event_handling/$(ARCH_DIR)/event_loop.o \
$(UI_DIR)event_handling/$(ARCH_DIR)/event_viewports.o \
$(UI_DIR)event_handling/$(ARCH_DIR)/global_events.o \
$(UI_DIR)event_handling/$(ARCH_DIR)/window_events.o \
$(UI_DIR)filter_popup/$(ARCH_DIR)/filter_selection.o \
$(UI_DIR)input/$(ARCH_DIR)/load.o \
$(UI_DIR)input/$(ARCH_DIR)/load_popup.o \
$(UI_DIR)layout/$(ARCH_DIR)/colours.o \
$(UI_DIR)layout/$(ARCH_DIR)/layout.o \
$(UI_DIR)print_popup/$(ARCH_DIR)/print.o \
$(UI_DIR)quit_popup/$(ARCH_DIR)/quit.o \
$(UI_DIR)resampling/$(ARCH_DIR)/resample.o \
$(UI_DIR)slices/$(ARCH_DIR)/slices.o \
$(UI_DIR)transform_popup/$(ARCH_DIR)/xform_selection.o \
$(UI_DIR)value_readout/$(ARCH_DIR)/update.o \
$(UI_DIR)widget_instances/$(ARCH_DIR)/colour_bar.o \
$(UI_DIR)widget_instances/$(ARCH_DIR)/initialize.o \
$(UI_DIR)widget_instances/$(ARCH_DIR)/main_menu_callbacks.o \
$(UI_DIR)widget_instances/$(ARCH_DIR)/merged_interface.o \
$(UI_DIR)widget_instances/$(ARCH_DIR)/meter.o \
$(UI_DIR)widget_instances/$(ARCH_DIR)/position_widgets.o \
$(UI_DIR)widget_instances/$(ARCH_DIR)/tag_points.o \
$(UI_DIR)widget_instances/$(ARCH_DIR)/volume_interface.o \
$(UI_DIR)widgets/$(ARCH_DIR)/buttons.o \
$(UI_DIR)widgets/$(ARCH_DIR)/sliders.o \
$(UI_DIR)widgets/$(ARCH_DIR)/text_entry.o \
$(UI_DIR)widgets/$(ARCH_DIR)/utilities.o \
$(UI_DIR)widgets/$(ARCH_DIR)/widgets.o \
$(UI_DIR)windows/$(ARCH_DIR)/lookup.o \
$(UI_DIR)windows/$(ARCH_DIR)/popup.o \
$(UI_DIR)windows/$(ARCH_DIR)/update.o $(UIC_DIR)UI_calls_F.o
register_LDFLAGS = 
CFLAGS = -g -O2
COMPILE = $(CC) $(DEFS) $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS) $(AM_CFLAGS) $(CFLAGS)
CCLD = $(CC)
LINK = $(CCLD) $(AM_CFLAGS) $(CFLAGS) $(LDFLAGS) -o $@
DIST_COMMON =  README ./stamp-h.in AUTHORS COPYING ChangeLog INSTALL \
Makefile.am Makefile.in Makefile.include.in NEWS aclocal.m4 config.h.in \
configure configure.in install-sh missing mkinstalldirs


DISTFILES = $(DIST_COMMON) $(SOURCES) $(HEADERS) $(TEXINFOS) $(EXTRA_DIST)

TAR = tar
GZIP_ENV = --best
DEP_FILES =  .deps/dummy.P
SOURCES = $(register_SOURCES)
OBJECTS = $(register_OBJECTS)

all: all-redirect
.SUFFIXES:
.SUFFIXES: .S .c .o .s
$(srcdir)/Makefile.in: Makefile.am $(top_srcdir)/configure.in $(ACLOCAL_M4) $(srcdir)/Functionality/Makefile.objects $(srcdir)/User_interface/Makefile.objects $(srcdir)/UI_calls_F/Makefile.objects
	cd $(top_srcdir) && $(AUTOMAKE) --gnu Makefile

Makefile: $(srcdir)/Makefile.in  $(top_builddir)/config.status $(BUILT_SOURCES)
	cd $(top_builddir) \
	  && CONFIG_FILES=$@ CONFIG_HEADERS= $(SHELL) ./config.status

$(ACLOCAL_M4):  configure.in 
	cd $(srcdir) && $(ACLOCAL)

config.status: $(srcdir)/configure $(CONFIG_STATUS_DEPENDENCIES)
	$(SHELL) ./config.status --recheck
$(srcdir)/configure: $(srcdir)/configure.in $(ACLOCAL_M4) $(CONFIGURE_DEPENDENCIES)
	cd $(srcdir) && $(AUTOCONF)

config.h: stamp-h
	@if test ! -f $@; then \
		rm -f stamp-h; \
		$(MAKE) stamp-h; \
	else :; fi
stamp-h: $(srcdir)/config.h.in $(top_builddir)/config.status
	cd $(top_builddir) \
	  && CONFIG_FILES= CONFIG_HEADERS=config.h \
	     $(SHELL) ./config.status
	@echo timestamp > stamp-h 2> /dev/null
$(srcdir)/config.h.in: $(srcdir)/stamp-h.in
	@if test ! -f $@; then \
		rm -f $(srcdir)/stamp-h.in; \
		$(MAKE) $(srcdir)/stamp-h.in; \
	else :; fi
$(srcdir)/stamp-h.in: $(top_srcdir)/configure.in $(ACLOCAL_M4) 
	cd $(top_srcdir) && $(AUTOHEADER)
	@echo timestamp > $(srcdir)/stamp-h.in 2> /dev/null

mostlyclean-hdr:

clean-hdr:

distclean-hdr:
	-rm -f config.h

maintainer-clean-hdr:
Makefile.include: $(top_builddir)/config.status Makefile.include.in
	cd $(top_builddir) && CONFIG_FILES=$@ CONFIG_HEADERS= $(SHELL) ./config.status

mostlyclean-binPROGRAMS:

clean-binPROGRAMS:
	-test -z "$(bin_PROGRAMS)" || rm -f $(bin_PROGRAMS)

distclean-binPROGRAMS:

maintainer-clean-binPROGRAMS:

install-binPROGRAMS: $(bin_PROGRAMS)
	@$(NORMAL_INSTALL)
	$(mkinstalldirs) $(DESTDIR)$(bindir)
	@list='$(bin_PROGRAMS)'; for p in $$list; do \
	  if test -f $$p; then \
	    echo "  $(INSTALL_PROGRAM) $$p $(DESTDIR)$(bindir)/`echo $$p|sed 's/$(EXEEXT)$$//'|sed '$(transform)'|sed 's/$$/$(EXEEXT)/'`"; \
	     $(INSTALL_PROGRAM) $$p $(DESTDIR)$(bindir)/`echo $$p|sed 's/$(EXEEXT)$$//'|sed '$(transform)'|sed 's/$$/$(EXEEXT)/'`; \
	  else :; fi; \
	done

uninstall-binPROGRAMS:
	@$(NORMAL_UNINSTALL)
	list='$(bin_PROGRAMS)'; for p in $$list; do \
	  rm -f $(DESTDIR)$(bindir)/`echo $$p|sed 's/$(EXEEXT)$$//'|sed '$(transform)'|sed 's/$$/$(EXEEXT)/'`; \
	done

.s.o:
	$(COMPILE) -c $<

.S.o:
	$(COMPILE) -c $<

mostlyclean-compile:
	-rm -f *.o core *.core

clean-compile:

distclean-compile:
	-rm -f *.tab.c

maintainer-clean-compile:

register: $(register_OBJECTS) $(register_DEPENDENCIES)
	@rm -f register
	$(LINK) $(register_LDFLAGS) $(register_OBJECTS) $(register_LDADD) $(LIBS)

# This directory's subdirectories are mostly independent; you can cd
# into them and run `make' without going through this Makefile.
# To change the values of `make' variables: instead of editing Makefiles,
# (1) if the variable is set in `config.status', edit `config.status'
#     (which will cause the Makefiles to be regenerated when you run `make');
# (2) otherwise, pass the desired values on the `make' command line.



all-recursive install-data-recursive install-exec-recursive \
installdirs-recursive install-recursive uninstall-recursive  \
check-recursive installcheck-recursive info-recursive dvi-recursive:
	@set fnord $(MAKEFLAGS); amf=$$2; \
	dot_seen=no; \
	target=`echo $@ | sed s/-recursive//`; \
	list='$(SUBDIRS)'; for subdir in $$list; do \
	  echo "Making $$target in $$subdir"; \
	  if test "$$subdir" = "."; then \
	    dot_seen=yes; \
	    local_target="$$target-am"; \
	  else \
	    local_target="$$target"; \
	  fi; \
	  (cd $$subdir && $(MAKE) $(AM_MAKEFLAGS) $$local_target) \
	   || case "$$amf" in *=*) exit 1;; *k*) fail=yes;; *) exit 1;; esac; \
	done; \
	if test "$$dot_seen" = "no"; then \
	  $(MAKE) $(AM_MAKEFLAGS) "$$target-am" || exit 1; \
	fi; test -z "$$fail"

mostlyclean-recursive clean-recursive distclean-recursive \
maintainer-clean-recursive:
	@set fnord $(MAKEFLAGS); amf=$$2; \
	dot_seen=no; \
	rev=''; list='$(SUBDIRS)'; for subdir in $$list; do \
	  rev="$$subdir $$rev"; \
	  test "$$subdir" = "." && dot_seen=yes; \
	done; \
	test "$$dot_seen" = "no" && rev=". $$rev"; \
	target=`echo $@ | sed s/-recursive//`; \
	for subdir in $$rev; do \
	  echo "Making $$target in $$subdir"; \
	  if test "$$subdir" = "."; then \
	    local_target="$$target-am"; \
	  else \
	    local_target="$$target"; \
	  fi; \
	  (cd $$subdir && $(MAKE) $(AM_MAKEFLAGS) $$local_target) \
	   || case "$$amf" in *=*) exit 1;; *k*) fail=yes;; *) exit 1;; esac; \
	done && test -z "$$fail"
tags-recursive:
	list='$(SUBDIRS)'; for subdir in $$list; do \
	  test "$$subdir" = . || (cd $$subdir && $(MAKE) $(AM_MAKEFLAGS) tags); \
	done

tags: TAGS

ID: $(HEADERS) $(SOURCES) $(LISP)
	list='$(SOURCES) $(HEADERS)'; \
	unique=`for i in $$list; do echo $$i; done | \
	  awk '    { files[$$0] = 1; } \
	       END { for (i in files) print i; }'`; \
	here=`pwd` && cd $(srcdir) \
	  && mkid -f$$here/ID $$unique $(LISP)

TAGS: tags-recursive $(HEADERS) $(SOURCES) config.h.in $(TAGS_DEPENDENCIES) $(LISP)
	tags=; \
	here=`pwd`; \
	list='$(SUBDIRS)'; for subdir in $$list; do \
   if test "$$subdir" = .; then :; else \
	    test -f $$subdir/TAGS && tags="$$tags -i $$here/$$subdir/TAGS"; \
   fi; \
	done; \
	list='$(SOURCES) $(HEADERS)'; \
	unique=`for i in $$list; do echo $$i; done | \
	  awk '    { files[$$0] = 1; } \
	       END { for (i in files) print i; }'`; \
	test -z "$(ETAGS_ARGS)config.h.in$$unique$(LISP)$$tags" \
	  || (cd $(srcdir) && etags $(ETAGS_ARGS) $$tags config.h.in $$unique $(LISP) -o $$here/TAGS)

mostlyclean-tags:

clean-tags:

distclean-tags:
	-rm -f TAGS ID

maintainer-clean-tags:

distdir = $(PACKAGE)-$(VERSION)
top_distdir = $(distdir)

# This target untars the dist file and tries a VPATH configuration.  Then
# it guarantees that the distribution is self-contained by making another
# tarfile.
distcheck: dist
	-rm -rf $(distdir)
	GZIP=$(GZIP_ENV) $(TAR) zxf $(distdir).tar.gz
	mkdir $(distdir)/=build
	mkdir $(distdir)/=inst
	dc_install_base=`cd $(distdir)/=inst && pwd`; \
	cd $(distdir)/=build \
	  && ../configure --srcdir=.. --prefix=$$dc_install_base \
	  && $(MAKE) $(AM_MAKEFLAGS) \
	  && $(MAKE) $(AM_MAKEFLAGS) dvi \
	  && $(MAKE) $(AM_MAKEFLAGS) check \
	  && $(MAKE) $(AM_MAKEFLAGS) install \
	  && $(MAKE) $(AM_MAKEFLAGS) installcheck \
	  && $(MAKE) $(AM_MAKEFLAGS) dist
	-rm -rf $(distdir)
	@banner="$(distdir).tar.gz is ready for distribution"; \
	dashes=`echo "$$banner" | sed s/./=/g`; \
	echo "$$dashes"; \
	echo "$$banner"; \
	echo "$$dashes"
dist: distdir
	-chmod -R a+r $(distdir)
	GZIP=$(GZIP_ENV) $(TAR) chozf $(distdir).tar.gz $(distdir)
	-rm -rf $(distdir)
dist-all: distdir
	-chmod -R a+r $(distdir)
	GZIP=$(GZIP_ENV) $(TAR) chozf $(distdir).tar.gz $(distdir)
	-rm -rf $(distdir)
distdir: $(DISTFILES)
	-rm -rf $(distdir)
	mkdir $(distdir)
	-chmod 777 $(distdir)
	here=`cd $(top_builddir) && pwd`; \
	top_distdir=`cd $(distdir) && pwd`; \
	distdir=`cd $(distdir) && pwd`; \
	cd $(top_srcdir) \
	  && $(AUTOMAKE) --include-deps --build-dir=$$here --srcdir-name=$(top_srcdir) --output-dir=$$top_distdir --gnu Makefile
	@for file in $(DISTFILES); do \
	  d=$(srcdir); \
	  if test -d $$d/$$file; then \
	    cp -pr $$d/$$file $(distdir)/$$file; \
	  else \
	    test -f $(distdir)/$$file \
	    || ln $$d/$$file $(distdir)/$$file 2> /dev/null \
	    || cp -p $$d/$$file $(distdir)/$$file || :; \
	  fi; \
	done
	for subdir in $(SUBDIRS); do \
	  if test "$$subdir" = .; then :; else \
	    test -d $(distdir)/$$subdir \
	    || mkdir $(distdir)/$$subdir \
	    || exit 1; \
	    chmod 777 $(distdir)/$$subdir; \
	    (cd $$subdir && $(MAKE) $(AM_MAKEFLAGS) top_distdir=../$(distdir) distdir=../$(distdir)/$$subdir distdir) \
	      || exit 1; \
	  fi; \
	done

DEPS_MAGIC := $(shell mkdir .deps > /dev/null 2>&1 || :)

-include $(DEP_FILES)

mostlyclean-depend:

clean-depend:

distclean-depend:
	-rm -rf .deps

maintainer-clean-depend:

%.o: %.c
	@echo '$(COMPILE) -c $<'; \
	$(COMPILE) -Wp,-MD,.deps/$(*F).pp -c $<
	@-cp .deps/$(*F).pp .deps/$(*F).P; \
	tr ' ' '\012' < .deps/$(*F).pp \
	  | sed -e 's/^\\$$//' -e '/^$$/ d' -e '/:$$/ d' -e 's/$$/ :/' \
	    >> .deps/$(*F).P; \
	rm .deps/$(*F).pp

%.lo: %.c
	@echo '$(LTCOMPILE) -c $<'; \
	$(LTCOMPILE) -Wp,-MD,.deps/$(*F).pp -c $<
	@-sed -e 's/^\([^:]*\)\.o[ 	]*:/\1.lo \1.o :/' \
	  < .deps/$(*F).pp > .deps/$(*F).P; \
	tr ' ' '\012' < .deps/$(*F).pp \
	  | sed -e 's/^\\$$//' -e '/^$$/ d' -e '/:$$/ d' -e 's/$$/ :/' \
	    >> .deps/$(*F).P; \
	rm -f .deps/$(*F).pp
info-am:
info: info-recursive
dvi-am:
dvi: dvi-recursive
check-am: all-am
check: check-recursive
installcheck-am:
installcheck: installcheck-recursive
all-recursive-am: config.h
	$(MAKE) $(AM_MAKEFLAGS) all-recursive

install-exec-am: install-binPROGRAMS
install-exec: install-exec-recursive

install-data-am:
install-data: install-data-recursive

install-am: all-am
	@$(MAKE) $(AM_MAKEFLAGS) install-exec-am install-data-am
install: install-recursive
uninstall-am: uninstall-binPROGRAMS
uninstall: uninstall-recursive
all-am: Makefile $(PROGRAMS) config.h
all-redirect: all-recursive-am
install-strip:
	$(MAKE) $(AM_MAKEFLAGS) AM_INSTALL_PROGRAM_FLAGS=-s install
installdirs: installdirs-recursive
installdirs-am:
	$(mkinstalldirs)  $(DESTDIR)$(bindir)


mostlyclean-generic:

clean-generic:
	-test -z "$(CLEANFILES)" || rm -f $(CLEANFILES)

distclean-generic:
	-rm -f Makefile $(CONFIG_CLEAN_FILES)
	-rm -f config.cache config.log stamp-h stamp-h[0-9]*

maintainer-clean-generic:
mostlyclean-am:  mostlyclean-hdr mostlyclean-binPROGRAMS \
		mostlyclean-compile mostlyclean-tags mostlyclean-depend \
		mostlyclean-generic

mostlyclean: mostlyclean-recursive

clean-am:  clean-hdr clean-binPROGRAMS clean-compile clean-tags \
		clean-depend clean-generic mostlyclean-am

clean: clean-recursive

distclean-am:  distclean-hdr distclean-binPROGRAMS distclean-compile \
		distclean-tags distclean-depend distclean-generic \
		clean-am

distclean: distclean-recursive
	-rm -f config.status

maintainer-clean-am:  maintainer-clean-hdr maintainer-clean-binPROGRAMS \
		maintainer-clean-compile maintainer-clean-tags \
		maintainer-clean-depend maintainer-clean-generic \
		distclean-am
	@echo "This command is intended for maintainers to use;"
	@echo "it deletes files that may require special tools to rebuild."

maintainer-clean: maintainer-clean-recursive
	-rm -f config.status

.PHONY: mostlyclean-hdr distclean-hdr clean-hdr maintainer-clean-hdr \
mostlyclean-binPROGRAMS distclean-binPROGRAMS clean-binPROGRAMS \
maintainer-clean-binPROGRAMS uninstall-binPROGRAMS install-binPROGRAMS \
mostlyclean-compile distclean-compile clean-compile \
maintainer-clean-compile install-data-recursive \
uninstall-data-recursive install-exec-recursive \
uninstall-exec-recursive installdirs-recursive uninstalldirs-recursive \
all-recursive check-recursive installcheck-recursive info-recursive \
dvi-recursive mostlyclean-recursive distclean-recursive clean-recursive \
maintainer-clean-recursive tags tags-recursive mostlyclean-tags \
distclean-tags clean-tags maintainer-clean-tags distdir \
mostlyclean-depend distclean-depend clean-depend \
maintainer-clean-depend info-am info dvi-am dvi check check-am \
installcheck-am installcheck all-recursive-am install-exec-am \
install-exec install-data-am install-data install-am install \
uninstall-am uninstall all-redirect all-am all installdirs-am \
installdirs mostlyclean-generic distclean-generic clean-generic \
maintainer-clean-generic clean mostlyclean distclean maintainer-clean


dummy.c:
	touch $@

# Tell versions [3.59,3.63) of GNU make to not export all variables.
# Otherwise a system limit (for SysV at least) may be exceeded.
.NOEXPORT:
