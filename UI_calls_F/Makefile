def: all

include ../Makefile.include

LIBRARY_NAME = UI_calls_F

SRC = $(ARCH_DIR)/UI_calls_F.c

OBJECTS = $(SRC:.c=.o)

PROTOTYPE_FILE = Include/$(LIBRARY_NAME)_prototypes.h

all:  $(PROTOTYPE_FILE) $(OBJECTS) Makefile.objects

$(SRC):
	@$(MAKE_SOURCE_LINKS)

objects-O3: $(PROTOTYPE_FILE) $(OBJECTS:.o=.u)

lint: $(SRC:.c=.ln)

Makefile.objects: Makefile
	@echo "$(LIBRARY_NAME)_OBJECTS = \\"   >  $@
	../make_object_list `pwd` $(OBJECTS) | sed -e 's/$(ARCH_DIR)/$$(ARCH_DIR
)/' >> $@

OPT = $(OPT_O)

INCLUDE = -IInclude -I.. \
          -I../User_interface/Include \
          -I../Functionality/Include \
          $(GRAPHICS_INCLUDE)

# ------------------------------

$(PROTOTYPE_FILE): $(SRC) 
	@$(MAKE_PROTOTYPES) $@ $(SRC)
