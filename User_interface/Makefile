def: all

include ../Makefile.include

LIBRARY_NAME = User_interface

SRC = \
          main/$(ARCH_DIR)/main.c \
          main/$(ARCH_DIR)/initialize.c \
          colour_popup/$(ARCH_DIR)/colour_selection.c \
          delete_tags_popup/$(ARCH_DIR)/delete_tags.c \
          event_callbacks/$(ARCH_DIR)/save_image.c \
          event_callbacks/$(ARCH_DIR)/slice_events.c \
          event_callbacks/$(ARCH_DIR)/tag_events.c \
          event_callbacks/$(ARCH_DIR)/utilities.c \
          event_callbacks/$(ARCH_DIR)/window_events.c \
          event_handling/$(ARCH_DIR)/event_callbacks.c \
          event_handling/$(ARCH_DIR)/event_loop.c \
          event_handling/$(ARCH_DIR)/event_viewports.c \
          event_handling/$(ARCH_DIR)/global_events.c \
          filter_popup/$(ARCH_DIR)/filter_selection.c \
          input/$(ARCH_DIR)/load.c \
          input/$(ARCH_DIR)/load_popup.c \
          layout/$(ARCH_DIR)/colours.c \
          layout/$(ARCH_DIR)/layout.c \
          print_popup/$(ARCH_DIR)/print.c \
          quit_popup/$(ARCH_DIR)/quit.c \
          resampling/$(ARCH_DIR)/resample.c \
          slices/$(ARCH_DIR)/slices.c \
          transform_popup/$(ARCH_DIR)/xform_selection.c \
          value_readout/$(ARCH_DIR)/update.c \
          widget_instances/$(ARCH_DIR)/colour_bar.c \
          widget_instances/$(ARCH_DIR)/initialize.c \
          widget_instances/$(ARCH_DIR)/main_menu_callbacks.c \
          widget_instances/$(ARCH_DIR)/merged_interface.c \
          widget_instances/$(ARCH_DIR)/meter.c \
          widget_instances/$(ARCH_DIR)/position_widgets.c \
          widget_instances/$(ARCH_DIR)/tag_points.c \
          widget_instances/$(ARCH_DIR)/volume_interface.c \
          widgets/$(ARCH_DIR)/buttons.c \
          widgets/$(ARCH_DIR)/sliders.c \
          widgets/$(ARCH_DIR)/text_entry.c \
          widgets/$(ARCH_DIR)/utilities.c \
          widgets/$(ARCH_DIR)/widgets.c \
          windows/$(ARCH_DIR)/lookup.c \
          windows/$(ARCH_DIR)/popup.c \
          windows/$(ARCH_DIR)/update.c

OBJECTS = $(SRC:.c=.o)

PROTOTYPE_FILE = Include/$(LIBRARY_NAME)_prototypes.h

all:  $(PROTOTYPE_FILE) $(OBJECTS)  Makefile.objects

$(SRC):
	@$(MAKE_SOURCE_LINKS)

objects-O3: $(PROTOTYPE_FILE) $(OBJECTS:.o=.u)

lint: $(SRC:.c=.ln)

Makefile.objects: Makefile
	@echo "$(LIBRARY_NAME)_OBJECTS = \\"   >  $@
	../make_object_list `pwd` $(OBJECTS) | sed -e 's/$(ARCH_DIR)/$$(ARCH_DIR
)/' >> $@

main/main.o:  Include/global_variables.h
main/main.u:  Include/global_variables.h

OPT = $(OPT_O)

INCLUDE = -IInclude -I.. \
          -I../UI_calls_F/Include \
          $(GRAPHICS_INCLUDE)

# ------------------------------

$(PROTOTYPE_FILE): $(SRC)
	@$(MAKE_PROTOTYPES) $@ $(SRC)
