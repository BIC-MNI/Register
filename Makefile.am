AUTOMAKE_OPTIONS = check-news
ACLOCAL_AMFLAGS = -I m4

SUBDIRS = Graphics Functionality UI_calls_F User_interface \
	Register_resample Tagtoxfm

bin_PROGRAMS = register
man_MANS = Documentation/register.1

# This nonsense is to force automake to install "data" in a "lib"
# directory.  This is required because "lib_DATA = register_UI.globals"
# doesn't work (automake 1.7).
#
blurgdir = $(libdir)
blurg_DATA = register_UI.globals

EXTRA_DIST = INSTALL.register  \
	register_UI.globals    \
	Documentation/register.1 \
	epm-header.in \
	$(m4_files)

m4_files = \
	m4/mni_REQUIRE_LIB.m4 \
	m4/mni_REQUIRE_MNILIBS.m4 \
	m4/mni_REQUIRE_OPENINVENTOR.m4 \
	m4/mni_cxx_have_koenig_lookup.m4 \
	m4/smr_CGAL_MAKEFILE.m4 \
	m4/smr_OPTIONAL_LIB.m4 \
	m4/smr_REQUIRED_LIB.m4 \
	m4/smr_WITH_BUILD_PATH.m4 \
	m4/acx_pthread.m4 \
	m4/ax_check_gl.m4 \
	m4/ax_check_glu.m4 \
	m4/ax_check_glut.m4 \
	m4/ax_lang_compiler_ms.m4

# Adding a dummy source file to register is a work-around for a bug in 
# automake, which does not (in version 1.4) allow linking together
# only objects in subdirectories.

register_SOURCES = dummy.c common_include.h

dummy.c:
	touch $@

CLEANFILES = dummy.c


# All the required libs are in LIBS, which is automatically
# added to the link line.
register_LDFLAGS = $(X_LIBS)
register_LDADD = $(objects) \
	Graphics/libbicgl.a $(GL_LIBS) $(GLU_LIBS) $(GLUT_LIBS)\
	$(MNI_LIBS)


objects = \
Functionality/initialize/initialize.o \
Functionality/slices/colour_map.o \
Functionality/slices/cursor.o \
Functionality/slices/create_slice.o \
Functionality/slices/initialize_slice.o \
Functionality/slices/resample.o \
Functionality/slices/save_image.o \
Functionality/slices/set_volume.o \
Functionality/slices/slices.o \
Functionality/slices/updates.o \
Functionality/tags/objects.o \
Functionality/tags/save_and_load.o \
Functionality/tags/tag_points.o \
Functionality/tags/tag_transform.o \
Functionality/update/update_window.o \
UI_calls_F/UI_calls_F.o \
User_interface/main/main.o \
User_interface/main/initialize.o \
User_interface/colour_popup/colour_selection.o \
User_interface/delete_tags_popup/delete_tags.o \
User_interface/event_callbacks/save_image.o \
User_interface/event_callbacks/slice_events.o \
User_interface/event_callbacks/tag_events.o \
User_interface/event_callbacks/utilities.o \
User_interface/event_callbacks/window_events.o \
User_interface/event_handling/event_callbacks.o \
User_interface/event_handling/event_loop.o \
User_interface/event_handling/event_viewports.o \
User_interface/event_handling/global_events.o \
User_interface/event_handling/window_events.o \
User_interface/filter_popup/filter_selection.o \
User_interface/input/load.o \
User_interface/input/load_popup.o \
User_interface/layout/colours.o \
User_interface/layout/layout.o \
User_interface/print_popup/print.o \
User_interface/quit_popup/quit.o \
User_interface/resampling/resample.o \
User_interface/slices/slices.o \
User_interface/transform_popup/xform_selection.o \
User_interface/value_readout/update.o \
User_interface/widget_instances/colour_bar.o \
User_interface/widget_instances/initialize.o \
User_interface/widget_instances/main_menu_callbacks.o \
User_interface/widget_instances/merged_interface.o \
User_interface/widget_instances/meter.o \
User_interface/widget_instances/position_widgets.o \
User_interface/widget_instances/tag_points.o \
User_interface/widget_instances/volume_interface.o \
User_interface/widgets/buttons.o \
User_interface/widgets/sliders.o \
User_interface/widgets/text_entry.o \
User_interface/widgets/utilities.o \
User_interface/widgets/widgets.o \
User_interface/windows/lookup.o \
User_interface/windows/popup.o \
User_interface/windows/update.o \
User_interface/windows/xsect.o

