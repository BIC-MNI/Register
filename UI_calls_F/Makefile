LIBRARY_NAME = UI_calls_F

OBJECTS = UI_calls_F.o

PROTOTYPE_FILE = Include/$(LIBRARY_NAME)_prototypes.h

all:  $(PROTOTYPE_FILE) $(OBJECTS) Makefile.objects

$(OBJECTS) $(OBJECTS:.o=.ln): $(PROTOTYPE_FILE)

lint: $(OBJECTS:.o=.ln)

Makefile.objects: Makefile
	@echo "$(LIBRARY_NAME)_OBJECTS = \\"   >  $@
	../make_object_list `pwd` $(OBJECTS)  >> $@

include ../Makefile.include

OPT = -O

INCLUDE = -IInclude -I.. \
          -I../User_interface/Include \
          -I../Functionality/Include \
          $(GRAPHICS_INCLUDE)

# ------------------------------

$(PROTOTYPE_FILE): $(OBJECTS:.o=.c) 
	@$(MAKE_DIRECTORY)/create_prototypes.csh $@ $(OBJECTS:.o=.c)
