default_target: all

FUNC_DIR = Functionality/
UIC_DIR = UI_calls_F/
UI_DIR = User_interface/

include $(FUNC_DIR)/Makefile.objects
include $(UI_DIR)/Makefile.objects
include $(UIC_DIR)/Makefile.objects

OBJECTS = \
          $(Functionality_OBJECTS) \
          $(User_interface_OBJECTS) \
          $(UI_calls_F_OBJECTS)

OPT = $(OPT_O)

make_objects:
	cd Functionality;  make "OPT=$(OPT)" "CC=$(CC)"
	cd UI_calls_F;     make "OPT=$(OPT)" "CC=$(CC)"
	cd User_interface; make "OPT=$(OPT)" "CC=$(CC)"

make_objects-O3:
	cd Functionality;  make objects-O3
	cd UI_calls_F;     make objects-O3
	cd User_interface; make objects-O3

all:  make_objects   register.irisgl  Tagtoxfm/tagtoxfm

$(OBJECTS):

include Makefile.include

Tagtoxfm/tagtoxfm Tagtoxfm/tagtoxfm-O3:
	cd Tagtoxfm;  make

register.irisgl: make_objects $(OBJECTS)
	$(CC) $(LFLAGS) $(OBJECTS) $(IRISGL_GRAPHICS_LIBS) -o $@

register-O3.irisgl: make_objects-O3 $(OBJECTS:.o=.u)
	$(CC) $(LFLAGS) -O3 $(OBJECTS:.o=.u) $(IRISGL_GRAPHICS_LIBS-O3) -o $@

register.opengl: make_objects $(OBJECTS)
	$(CC) $(LFLAGS) $(OBJECTS) $(OPENGL_GRAPHICS_LIBS) -o $@

register-O3.opengl: make_objects-O3 $(OBJECTS:.o=.u)
	$(CC) $(LFLAGS) -O3 $(OBJECTS:.o=.u) $(OPENGL_GRAPHICS_LIBS-O3) -o $@

register.mesa: make_objects $(OBJECTS)
	$(CC) $(LFLAGS) $(OBJECTS) $(MESA_GRAPHICS_LIBS) -o $@

register-O3.mesa: make_objects-O3 $(OBJECTS:.o=.u)
	$(CC) $(LFLAGS) -O3 $(OBJECTS:.o=.u) $(MESA_GRAPHICS_LIBS-O3) -o $@

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
	\rm -f register-O3.irisgl register-O3.opengl register-O3.mesa

test.o: test.c test.include
	$(CC) $(OPT) test.c -c -o $@ \
              -I. -I$(SRC_DIRECTORY)/GLUT/include

test: test.o
	$(CC) $(LFLAGS) test.o $(MESA_GRAPHICS_LIBS) -o $@

