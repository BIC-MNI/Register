default_target: all

include Functionality/Makefile.objects
include User_interface/Makefile.objects
include UI_calls_F/Makefile.objects

OBJECTS = \
          $(Functionality_OBJECTS) \
          $(User_interface_OBJECTS) \
          $(UI_calls_F_OBJECTS)

make_objects:
	cd Functionality;  make
	cd UI_calls_F;     make
	cd User_interface; make

all:  make_objects   register.irisgl

$(OBJECTS):

include Makefile.include

RECIPES_LIB =

register.irisgl: make_objects $(OBJECTS)
	$(CC) $(LFLAGS) $(OBJECTS) $(IRISGL_GRAPHICS_LIBS) -o $@ $(RECIPES_LIB)

register.opengl: make_objects $(OBJECTS)
	$(CC) $(LFLAGS) $(OBJECTS) $(OPENGL_GRAPHICS_LIBS) -o $@ $(RECIPES_LIB)

register.mesa: make_objects $(OBJECTS)
	$(CC) $(LFLAGS) $(OBJECTS) $(MESA_GRAPHICS_LIBS) -o $@ $(RECIPES_LIB)

lint_register:
	cd Functionality;  make lint
	cd UI_calls_F;     make lint
	cd User_interface; make lint
	$(LINT) -x -u $(OBJECTS:.o=.ln) $(GRAPHICS_LINT_LIBS)

clean_all:
	cd Functionality;  make clean
	cd UI_calls_F;     make clean
	cd User_interface; make clean
	\rm -f register.irisgl register.opengl register.mesa
