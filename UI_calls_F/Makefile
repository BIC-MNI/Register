LIBRARY_NAME = UI_calls_F

OBJECTS = UI_calls_F.o

LIBRARY = lib$(LIBRARY_NAME).a
LINT_LIBRARY = llib-l$(LIBRARY_NAME).ln

PROTOTYPE_FILE = Include/$(LIBRARY_NAME)_prototypes.h

all:  $(PROTOTYPE_FILE) $(OBJECTS) $(OBJECTS:.o=.ln) Makefile.objects

Makefile.objects: Makefile
	@echo "$(LIBRARY_NAME)_OBJECTS = \\"   >  $@
	../make_object_list `pwd` $(OBJECTS)  >> $@

include $(C_DEV_DIRECTORY)/Make/Makefile.include
include $(GRAPHICS_DIRECTORY)/Makefile.include

OPT = -O

INCLUDE = -IInclude -I.. \
          -I../User_interface/Include \
          -I../Functionality/Include \
          -I$(C_DEV_DIRECTORY)/Include \
          $(GRAPHICS_INCLUDE) \
          -I/usr/local/include

# ------------------------------

$(PROTOTYPE_FILE): $(OBJECTS:.o=.c) 
	@echo "#ifndef  DEF_$(LIBRARY_NAME)_PROTOTYPES"              >  $@
	@echo "#define  DEF_$(LIBRARY_NAME)_PROTOTYPES"              >> $@
	@extract_functions -public $(OBJECTS:.o=.c)           >> $@
	@echo "#endif"                                        >> $@
