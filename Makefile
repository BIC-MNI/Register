include Functionality/Makefile.objects
include User_interface/Makefile.objects
include UI_calls_F/Makefile.objects

OBJECTS = \
          $(Functionality_OBJECTS) \
          $(User_interface_OBJECTS) \
          $(UI_calls_F_OBJECTS)

all:  register  lint_register

$(OBJECTS):

include Makefile.include

OPT = -g

register: $(OBJECTS)
	$(CC) $(LFLAGS) $(OBJECTS) $(GRAPHICS_LIBS_2D) -o $@

lint_register: $(OBJECTS:.o=.ln)
	$(LINT) -u $(OBJECTS:.o=.ln) $(GRAPHICS_LINT_LIBS)

register.pixie:  $(OBJECTS)
	if( -e register ) rm register
	if( -e register.Addrs ) rm register.Addrs
	if( -e register.Counts ) rm register.Counts
	make register GRAPHICS_LIBS="-L$HHOME/david/new_C_dev -lmni -lfm -lgl -lX11 -L$$HHOME/david/new_C_dev/Libraries -lminc -lnetcdf  -lm -lsun -lmalloc"
	@\rm -f register.Counts
	@pixie register -o $@

prof:
	prof -pixie register -proc >&! pixie.procedures
	prof -pixie register -heavy >&! pixie.heavy


