def: all

include ../Makefile.include
UI_DIR =
include Makefile.objects

LIBRARY_NAME = User_interface

OBJECTS = $(User_interface_OBJECTS)
SRC = $(OBJECTS:.o=.c)

PROTOTYPE_FILE = Include/$(LIBRARY_NAME)_prototypes.h

all:  $(PROTOTYPE_FILE) $(OBJECTS)

$(SRC):
	@$(MAKE_SOURCE_LINKS)

objects-O3: $(PROTOTYPE_FILE) $(OBJECTS:.o=.u)

lint: $(SRC:.c=.ln)

$(UI_DIR)main/$(ARCH_DIR)/main.o:  Include/global_variables.h

$(UI_DIR)main/$(ARCH_DIR)/main.u:  Include/global_variables.h

OPT = $(OPT_O)

INCLUDE = -IInclude -I.. \
          -I../UI_calls_F/Include \
          $(GRAPHICS_INCLUDE)

# ------------------------------

$(PROTOTYPE_FILE): $(SRC)
	@$(MAKE_PROTOTYPES) $@ $(SRC)
