#
#   if the next line bombs, it is because you didn't do the following:
#
#      cd Makefile_multi_architecture ; make 
#      then execute the command: source ...  as it tells you to do when
#

include $(ARCH_MAKEFILE)

def: cshrc.csh
	@echo ""
	@echo "In order to make these directories you must do the following:"
	@echo ""
	@echo "   1) execute the "source" line above"
	@echo "   2) if not using OpenGL or IrisGL, build the Mesa library, "
	@echo "        (cd Mesa; make) "
	@echo "      or if it is already on system, edit Make/Makefile.general"
	@echo "   3) type one of:"
	@echo "        make irisgl"
	@echo "        make opengl"
	@echo "        make mesa"
	@echo ""


cshrc.csh: FORCE
	@$(ARCH_MAKEFILE_DIR)/bin/insert_directory.csh $@.in $@
	@echo ""
	@echo "    Created $@"
	@echo "    You should now execute the following line"
	@echo ""
	@echo "source `pwd`/$@"
	@echo ""

FORCE:

OPT = -O

NETCDF_DIR = netcdf-2.3.2
NETCDF_LIB = $(NETCDF_DIR)/libsrc/libnetcdf.a
MESA_LIB = Mesa/$(ARCH_DIR)/libMesaGL.a

MINC_TAR = minc-0.6.tar.gz

MINC_LIB = minc/libsrc/libminc.a

$(NETCDF_DIR): $(NETCDF_DIR).tar.gz
	gunzip -c $(NETCDF_DIR).tar.gz | tar xvf -
	cd $(NETCDF_DIR) ; ln -s libsrc lib ; ln -s libsrc include

$(NETCDF_LIB): $(NETCDF_DIR)
	cd $(NETCDF_DIR) ; ./configure --prefix=. ; make

minc: $(MINC_TAR)
	gunzip -c $(MINC_TAR) | tar xvf -

$(MINC_LIB): minc
	-cd minc ; env NETCDF_PREFIX=`pwd`/../$(NETCDF_DIR) ./configure --prefix=. ; \
                     make

glut:
	-(cd GLUT ; make)

glut_for_mesa:
	cd GLUT ; make MESA_INCLUDE=-I$(SRC_DIRECTORY)/Mesa/Mesa-distribution/include

$(MESA_LIB):
	cd Mesa ; make

basic: $(NETCDF_LIB) $(MINC_LIB)


irisgl: basic
	cd Volume_io ; make "OPT=$(OPT)"
	cd BIC_PL ; make "OPT=$(OPT)"
	cd Graphics ; make $@ "OPT=$(OPT)"
	cd Display ; make Display.$@ "OPT=$(OPT)"
	cd Register ; make make_objects register.$@ "OPT=$(OPT)"
	cd Register/Tagtoxfm ; make "OPT=$(OPT)" tagtoxfm

opengl:
#	cd Volume_io ; make "OPT=$(OPT)"
#	cd BIC_PL ; make "OPT=$(OPT)"
	cd ../Graphics ; make $@ "OPT=$(OPT)"
#	cd Display ; make Display.$@ "OPT=$(OPT)"
	cd .. ; make make_objects register.$@ "OPT=$(OPT)"
	cd ../Tagtoxfm ; make "OPT=$(OPT)" tagtoxfm

mesa: basic $(MESA_LIB) glut_for_mesa
	cd Volume_io ; make "OPT=$(OPT)"
	cd BIC_PL ; make "OPT=$(OPT)"
	cd Graphics ; make $@ "OPT=$(OPT)"
	cd Display ; make Display.$@ "OPT=$(OPT)"
	cd Register ; make make_objects register.$@ "OPT=$(OPT)"
	cd Register/Tagtoxfm ; make "OPT=$(OPT)" tagtoxfm

irisgl-O3: basic
	cd Volume_io ; make libvolume_io-O3.a "OPT=$(OPT)"
	cd BIC_PL ; make libbicpl-O3.a "OPT=$(OPT)"
	cd Graphics ; make $@ "OPT=$(OPT)"
	cd Display ; make Display.${@:-O3=} "OPT=$(OPT)"
	cd Register ; make make_objects register.${@:-O3=} "OPT=$(OPT)"
	cd Register/Tagtoxfm ; make "OPT=$(OPT)" tagtoxfm

opengl-O3: basic glut
	cd Volume_io ; make libvolume_io-O3.a "OPT=$(OPT)"
	cd BIC_PL ; make libbicpl-O3.a "OPT=$(OPT)"
	cd Graphics ; make $@ "OPT=$(OPT)"
	cd Display ; make Display.${@:-O3=} "OPT=$(OPT)"
	cd Register ; make make_objects register.${@:-O3=} "OPT=$(OPT)"
	cd Register/Tagtoxfm ; make "OPT=$(OPT)" tagtoxfm

mesa-O3: basic $(MESA_LIB) glut_for_mesa
	cd Volume_io ; make libvolume_io-O3.a "OPT=$(OPT)"
	cd BIC_PL ; make libbicpl-O3.a "OPT=$(OPT)"
	cd Graphics ; make $@ "OPT=$(OPT)"
	cd Display ; make Display.${@:-O3=} "OPT=$(OPT)"
	cd Register ; make make_objects register.${@:-O3=} "OPT=$(OPT)"
	cd Register/Tagtoxfm ; make "OPT=$(OPT)" tagtoxfm

clean:
#	cd $(NETCDF_DIR) ; make clean
#	cd minc ; make clean
#	cd Volume_io ; make clean
#	cd BIC_PL ; make clean
	cd Graphics ; make clean
#	cd Display ; make clean_all
	cd .. ; make clean_all
	cd ../Tagtoxfm ; make clean
