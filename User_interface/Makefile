LIBRARY_NAME = User_interface

OBJECTS = \
          main/main.o \
          main/initialize.o \
          colour_popup/colour_selection.o \
          delete_tags_popup/delete_tags.o \
          event_callbacks/save_image.o \
          event_callbacks/slice_events.o \
          event_callbacks/tag_events.o \
          event_callbacks/utilities.o \
          event_callbacks/window_events.o \
          event_handling/event_callbacks.o \
          event_handling/event_loop.o \
          event_handling/event_viewports.o \
          event_handling/global_events.o \
          filter_popup/filter_selection.o \
          input/load.o \
          input/load_popup.o \
          layout/colours.o \
          layout/layout.o \
          print_popup/print.o \
          quit_popup/quit.o \
          resampling/resample.o \
          slices/slices.o \
          value_readout/update.o \
          widget_instances/colour_bar.o \
          widget_instances/initialize.o \
          widget_instances/main_menu_callbacks.o \
          widget_instances/merged_interface.o \
          widget_instances/meter.o \
          widget_instances/position_widgets.o \
          widget_instances/tag_points.o \
          widget_instances/volume_interface.o \
          widgets/buttons.o \
          widgets/sliders.o \
          widgets/text_entry.o \
          widgets/utilities.o \
          widgets/widgets.o \
          windows/lookup.o \
          windows/popup.o \
          windows/update.o

LIBRARY = lib$(LIBRARY_NAME).a
LINT_LIBRARY = llib-l$(LIBRARY_NAME).ln

PROTOTYPE_FILE = Include/$(LIBRARY_NAME)_prototypes.h

all:  $(PROTOTYPE_FILE) $(OBJECTS) $(OBJECTS:.o=.ln) Makefile.objects

Makefile.objects: Makefile
	@echo "$(LIBRARY_NAME)_OBJECTS = \\"  >  $@
	../make_object_list `pwd` $(OBJECTS) >> $@

main/main.o:  Include/global_variables.h

include $(C_DEV_DIRECTORY)/Make/Makefile.include
include $(GRAPHICS_DIRECTORY)/Makefile.include

OPT = -O

INCLUDE = -IInclude -I.. \
          -I../UI_calls_F/Include \
          -I$(C_DEV_DIRECTORY)/Include \
          $(GRAPHICS_INCLUDE) \
          -I/usr/local/include

# ------------------------------

$(PROTOTYPE_FILE): $(OBJECTS:.o=.c) Include/*.h
	@echo "#ifndef  DEF_$(LIBRARY_NAME)_PROTOTYPES"              >  $@
	@echo "#define  DEF_$(LIBRARY_NAME)_PROTOTYPES"              >> $@
	@extract_functions -public $(OBJECTS:.o=.c)           >> $@
	@echo "#endif"                                        >> $@
