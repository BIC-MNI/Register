def: all

include ../Makefile.include

LIBRARY_NAME = Functionality

SRC = initialize/$(ARCH_DIR)/initialize.c \
      slices/$(ARCH_DIR)/colour_map.c \
      slices/$(ARCH_DIR)/cursor.c \
      slices/$(ARCH_DIR)/create_slice.c \
      slices/$(ARCH_DIR)/initialize_slice.c \
      slices/$(ARCH_DIR)/resample.c \
      slices/$(ARCH_DIR)/save_image.c \
      slices/$(ARCH_DIR)/set_volume.c \
      slices/$(ARCH_DIR)/slices.c \
      slices/$(ARCH_DIR)/updates.c \
      tags/$(ARCH_DIR)/objects.c \
      tags/$(ARCH_DIR)/save_and_load.c \
      tags/$(ARCH_DIR)/tag_points.c \
      tags/$(ARCH_DIR)/tag_transform.c \
      update/$(ARCH_DIR)/update_window.c

OBJECTS = $(SRC:.c=.o)

PROTOTYPE_FILE = Include/$(LIBRARY_NAME)_prototypes.h

all:  $(PROTOTYPE_FILE) $(OBJECTS)  Makefile.objects

$(SRC):
	@$(MAKE_SOURCE_LINKS)

objects-O3: $(PROTOTYPE_FILE) $(OBJECTS:.o=.u)

lint: $(SRC:.c=.ln)

Makefile.objects: Makefile
	@echo "$(LIBRARY_NAME)_OBJECTS = \\"   >  $@
	../make_object_list `pwd` $(OBJECTS) | sed -e 's/$(ARCH_DIR)/$$(ARCH_DIR)/' >> $@

initialize/$(ARCH_DIR)/initialize.o: Include/global_variables.h
initialize/$(ARCH_DIR)/initialize.u: Include/global_variables.h

OPT = $(OPT_O)

INCLUDE = -IInclude \
          -I.. \
          -I../UI_calls_F/Include \
          $(GRAPHICS_INCLUDE)

# ------------------------------

$(PROTOTYPE_FILE): $(SRC)
	@$(MAKE_PROTOTYPES) $@ $(SRC)
