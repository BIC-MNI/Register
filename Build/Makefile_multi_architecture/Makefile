default: cshrc.csh Makefile.system_dependent

cshrc.csh: FORCE
	@bin/insert_directory.csh $@.in $@
	@echo ""
	@echo "    Created $@"
	@echo "    You should now execute the following line"
	@echo ""
	@echo "source `pwd`/$@"
	@echo ""

Makefile.system_dependent: FORCE
	@bin/insert_directory.csh $@.in $@

FORCE:
