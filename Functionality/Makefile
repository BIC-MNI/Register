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
          tags/matrix_basics.o \
          tags/objects.o \
          tags/procrustes.o \
          tags/save_and_load.o \
          tags/tag_points.o \
          tags/tag_transform.o \
          update/update_window.o

LIBRARY = lib$(LIBRARY_NAME).a
LINT_LIBRARY = llib-l$(LIBRARY_NAME).ln

PROTOTYPE_FILE = Include/$(LIBRARY_NAME)_prototypes.h

all:  $(PROTOTYPE_FILE) $(OBJECTS) $(OBJECTS:.o=.ln) Makefile.objects

Makefile.objects: Makefile
	@echo "$(LIBRARY_NAME)_OBJECTS = \\"   >  $@
	../make_object_list `pwd` $(OBJECTS)  >> $@

initialize/initialize.o: Include/global_variables.h

include $(C_DEV_DIRECTORY)/Make/Makefile.include
include $(GRAPHICS_DIRECTORY)/Makefile.include

OPT = -O

INCLUDE = -IInclude \
          -I.. \
          -I../UI_calls_F/Include \
          -I$(C_DEV_DIRECTORY)/Include \
          $(GRAPHICS_INCLUDE) \
          -I/usr/local/include

# ------------------------------

$(PROTOTYPE_FILE): $(OBJECTS:.o=.c) Include/*.h
	@echo "#ifndef  DEF_$(LIBRARY_NAME)_PROTOTYPES"              >  $@
	@echo "#define  DEF_$(LIBRARY_NAME)_PROTOTYPES"              >> $@
	@extract_functions -public $(OBJECTS:.o=.c)                  >> $@
	@echo "#endif"                                        >> $@
