# --------------------------------------------------------------------
#
#     Generic Makefile
#
# if you are creating only one executable, you need only modify the
# following lines:

include $(SRC_DIRECTORY)/Volume_io/Makefile.include


PROG     = register_resample#                  # executable name
PROG_OBJ = register_resample.o ParseArgv.o#    # files to compile
OPT      = -O -prototypes#                     # compiler options
LDOPT    = $(VOLUME_IO_LIBS)
INCLUDES = -I. $(VOLUME_IO_INCLUDE)

# --------------------------------------------------------------------

.SUFFIXES: .ln

CFLAGS    = $(INCLUDES) $(OPT)#           # CFLAGS and LINTFLAGS should
LINTFLAGS = $(INCLUDES)#                  # be same, except for -g/-O

default : $(PROG)

all : $(PROG) lint_$(PROG)

.c.ln:#                                   # defines the rule for creating .ln
	lint $(LINTFLAGS) -c $< -o $@

.c.o:#                                    # defines the rule for creating .o
	$(CC) $(CFLAGS) -c $< -o $@

LINT_LIST = $(PROG_OBJ:.o=.ln)#           # list of lint files in program

$(PROG_OBJ) : Makefile

# How to create executable
$(PROG): $(PROG_OBJ)
	$(CC) $(PROG_OBJ) -o $@ $(LDOPT)

# how to lint the executable source
lint_$(PROG): $(LINT_LIST)
	lint -m -x -u $(LINTFLAGS) $(LINT_LIST)

clean:
	\rm -f $(PROG) $(PROG_OBJ)  $(LINT_LIST)
