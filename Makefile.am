SUBDIRS = Graphics Functionality UI_calls_F User_interface Register_resample Tagtoxfm 

FUNC_DIR = Functionality/
UIC_DIR = UI_calls_F/
UI_DIR = User_interface/

include Functionality/Makefile.objects
include User_interface/Makefile.objects
include UI_calls_F/Makefile.objects


bin_PROGRAMS = register


# Adding a dummy source file to register is a work-around for a bug in 
# automake, which does not (in version 1.4) allow linking together
# only objects in subdirectories.

register_SOURCES = dummy.c

dummy.c:
	touch $@

CLEANFILES = dummy.c


register_LDADD = $(Functionality_OBJECTS) $(User_interface_OBJECTS) $(UI_calls_F_OBJECTS) \
	-LGraphics -lopengl_x_graphics \
	-lglut -lGL -lGLU \
	$(X_LIBS) $(X_PRE_LIBS) -lX11 $(X_EXTRA_LIBS) \
	$(MNI_LIBS) -lm

