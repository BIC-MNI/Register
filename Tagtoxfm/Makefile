def: tagtoxfm

OPT = -O

include $(SRC_DIRECTORY)/BIC_PL/Makefile.include

INCLUDE = $(BIC_PL_INCLUDE)
LIBS =  $(BIC_PL_LIBS)
LIBS-O3 =  $(BIC_PL_LIBS-O3)
LINT_LIBS = $(BIC_PL_LINT_LIBS)

TAG_SRC = tagtoxfm.c

TAG_OBJ = $(TAG_SRC:.c=.o)

###########################################################################

tagtoxfm: $(TAG_OBJ)
	$(CC) $(LFLAGS) $(TAG_OBJ) $(LIBS) -o $@

tagtoxfm-O3: $(TAG_OBJ:.o=.u)
	$(CC) $(LFLAGS) -O3 $(TAG_OBJ:.o=.u) $(LIBS-O3) -o $@

lint_tagtoxfm: $(TAG_SRC:.c=.ln)
	$(LINT) -x -u $(LINTFLAGS) $(TAG_SRC:.c=.ln) $(LINT_LIBS)

###########################################################################
