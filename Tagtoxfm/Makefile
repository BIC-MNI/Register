# --------------------------------------------------------------------
#
# MINC Makefile
#

# Executable names
PROGS    = tagtoxfm
EXTRA_OBJ = ParseArgv.o
HEADERS  = tagtoxfm.h ParseArgv.h

include $(SRC_DIRECTORY)/BIC_PL/Makefile.include

OPT      = $(OPT_O)
INCLUDES = -I. $(BIC_PL_INCLUDE) -I/usr/local/include
LINTFLAGS = -I. $(BIC_PL_INCLUDE) -I/usr/local/include
CDEFINES = #                        cpp defines
LDOPT    =  $(BIC_PL_LIBS) -lm
LINT_LIBS    =  $(BIC_PL_LINT_LIBS)

RM = rm
RMFLAGS = -f

# --------------------------------------------------------------------

CFLAGS    = $(CDEFINES) $(INCLUDES) $(OPT)# CFLAGS and LINTFLAGS should
LINTFLAGS = $(CDEFINES) $(INCLUDES)#        be same, except for -g/-O

PROG_OBJ  = $(PROGS:=.o)#                 list of objects
LINT_LIST = $(PROG_OBJ:.o=.ln)
LINT_EXTRA = $(EXTRA_OBJ:.o=.ln)
LINT_LIST_EXE = $(LINT_LIST:.ln=.)#       list of executable names to lint

# --------------------------------------------------------------------

.SUFFIXES: .ln

default: build

all: build lint

build: $(PROGS)

#Dependency on Makefile
$(PROG_OBJ) $(EXTRA_OBJ) $(LINT_LIST) $(LINT_EXTRA): Makefile

.c.ln:#                                   defines the rule for creating .ln
	lint $(LINTFLAGS) -c $< -o $@

.c.o:#                                    defines the rule for creating .o
	$(CC) $(CFLAGS) -c $< -o $@

#Dependency of .o and .ln on .h
$(PROG_OBJ) $(EXTRA_OBJ): $(HEADERS)

$(LINT_LIST) $(LINT_EXTRA): $(HEADERS)

# How to make executables
$(PROGS) : $$@.o $(EXTRA_OBJ) $(CC_PROG_LIB)
	$(CC) $@.o $(EXTRA_OBJ) -o $@ $(LDOPT)

#  how to lint the executable source
lint: $(LINT_LIST_EXE)

$(LINT_LIST_EXE) : $$@ln $(LINT_EXTRA)
	lint -u $(LINTFLAGS) $@ln $(LINT_EXTRA) $(LINT_LIBS)

# Remove all derived files in this directory
clean:
	$(RM) $(RM_FLAGS) $(LINT_LIST) $(PROGS) $(PROG_OBJ) \
	            $(EXTRA_OBJ) $(LINT_EXTRA)

