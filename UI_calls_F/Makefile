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


OPT = $(OPT_O)

INCLUDE = -IInclude -I.. \
          -I../User_interface/Include \
          -I../Functionality/Include \
	  -I../Graphics/Include $(BIC_PL_INCLUDE) $(X11_INCLUDES)


# ------------------------------

$(PROTOTYPE_FILE): $(SRC) 
	@$(MAKE_PROTOTYPES) $@ $(SRC)
