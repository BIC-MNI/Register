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
          transform_popup/xform_selection.o \
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

PROTOTYPE_FILE = Include/$(LIBRARY_NAME)_prototypes.h

all:  $(PROTOTYPE_FILE) $(OBJECTS)  Makefile.objects

$(OBJECTS) $(OBJECTS:.o=.ln): $(PROTOTYPE_FILE)

lint: $(OBJECTS:.o=.ln)

Makefile.objects: Makefile
	@echo "$(LIBRARY_NAME)_OBJECTS = \\"   >  $@
	../make_object_list `pwd` $(OBJECTS)  >> $@

main/main.o:  Include/global_variables.h

include ../Makefile.include

OPT = -O

INCLUDE = -IInclude -I.. \
          -I../UI_calls_F/Include \
          $(GRAPHICS_INCLUDE)

# ------------------------------

$(PROTOTYPE_FILE): $(OBJECTS:.o=.c) Include/*.h
	@$(MAKE_DIRECTORY)/create_prototypes.csh $@ $(OBJECTS:.o=.c)
