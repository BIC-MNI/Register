LIBRARY_NAME = Functionality

OBJECTS = initialize/initialize.o \
          slices/colour_map.o \
          slices/cursor.o \
          slices/create_slice.o \
          slices/initialize_slice.o \
          slices/resample.o \
          slices/save_image.o \
          slices/set_volume.o \
          slices/slices.o \
          slices/updates.o \
          tags/objects.o \
          tags/save_and_load.o \
          tags/tag_points.o \
          tags/tag_transform.o \
          update/update_window.o

PROTOTYPE_FILE = Include/$(LIBRARY_NAME)_prototypes.h

all:  $(PROTOTYPE_FILE) $(OBJECTS)  Makefile.objects

$(OBJECTS) $(OBJECTS:.o=.ln): $(PROTOTYPE_FILE)

lint: $(OBJECTS:.o=.ln)

Makefile.objects: Makefile
	@echo "$(LIBRARY_NAME)_OBJECTS = \\"   >  $@
	../make_object_list `pwd` $(OBJECTS)  >> $@

initialize/initialize.o: Include/global_variables.h

include ../Makefile.include

OPT = -O

INCLUDE = -IInclude \
          -I.. \
          -I../UI_calls_F/Include \
          $(GRAPHICS_INCLUDE)

# ------------------------------

$(PROTOTYPE_FILE): $(OBJECTS:.o=.c) Include/*.h
	@$(MAKE_DIRECTORY)/create_prototypes.csh $@ $(OBJECTS:.o=.c)
