def: all

include ../Makefile.include
UIC_DIR =
include Makefile.objects

LIBRARY_NAME = UI_calls_F

OBJECTS = $(UI_calls_F_OBJECTS)
SRC = $(OBJECTS:.o=.c)

PROTOTYPE_FILE = Include/$(LIBRARY_NAME)_prototypes.h

all:  $(PROTOTYPE_FILE) $(OBJECTS)

$(SRC):
	@$(MAKE_SOURCE_LINKS)

objects-O3: $(PROTOTYPE_FILE) $(OBJECTS:.o=.u)

lint: $(SRC:.c=.ln)

OPT = $(OPT_O)

INCLUDE = -IInclude -I.. \
          -I../User_interface/Include \
          -I../Functionality/Include \
          $(GRAPHICS_INCLUDE)

# ------------------------------

$(PROTOTYPE_FILE): $(SRC) 
	@$(MAKE_PROTOTYPES) $@ $(SRC)
