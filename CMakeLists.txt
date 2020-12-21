# CMakeFiles.txt for the Register
#
# Vladimir S. FONOV - vladimir.fonov@gmail.com

CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

SET(REGISTER_PACKAGE_VERSION_MAJOR 1)
SET(REGISTER_PACKAGE_VERSION_MINOR 5)
SET(REGISTER_PACKAGE_VERSION_PATCH 0)

# Get the current branch
EXECUTE_PROCESS(
  COMMAND git rev-parse --abbrev-ref HEAD
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
  OUTPUT_VARIABLE GIT_BRANCH
  OUTPUT_STRIP_TRAILING_WHITESPACE
  ERROR_QUIET
)

IF(NOT GIT_BRANCH)
  SET(GIT_BRANCH "Develop")
ENDIF(NOT GIT_BRANCH) 


# Get the current commit hash.
EXECUTE_PROCESS(
  COMMAND git log -1 --format=%h
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
  OUTPUT_VARIABLE GIT_COMMIT
  OUTPUT_STRIP_TRAILING_WHITESPACE
  ERROR_QUIET
)

IF(NOT GIT_COMMIT)
  SET(GIT_COMMIT "unknown")
ENDIF(NOT GIT_COMMIT) 

IF(NOT MINC_TOOLKIT_BUILD)

  PROJECT(Register "C")
  SET(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH}" "${CMAKE_CURRENT_SOURCE_DIR}/cmake-modules")
  
  # Packaging defines
  SET(CPACK_GENERATOR TGZ)
  SET(CPACK_PACKAGE_VERSION_MAJOR ${REGISTER_PACKAGE_VERSION_MAJOR})
  SET(CPACK_PACKAGE_VERSION_MINOR ${REGISTER_PACKAGE_VERSION_MINOR})
  SET(CPACK_PACKAGE_VERSION_PATCH ${REGISTER_PACKAGE_VERSION_PATCH})
  INCLUDE(CPack)

  FIND_PACKAGE( LIBMINC REQUIRED )
  FIND_PACKAGE( BICPL REQUIRED )
#  FIND_PACKAGE( Perl REQUIRED )
  FIND_PACKAGE( Threads )
  FIND_PACKAGE( OpenGL REQUIRED )
  FIND_PACKAGE( GLUT REQUIRED )
  
  INCLUDE( InstallManPages )

ENDIF(NOT MINC_TOOLKIT_BUILD)

IF(APPLE)
  SET(HAVE_APPLE_OPENGL_FRAMEWORK ON)
ENDIF(APPLE)

SET(PACKAGE "Register")
SET(PACKAGE_BUGREPORT "a.janke@gmail.com")

SET(PACKAGE_NAME "Register")
SET(PACKAGE_VERSION "${REGISTER_PACKAGE_VERSION_MAJOR}.${REGISTER_PACKAGE_VERSION_MINOR}.${REGISTER_PACKAGE_VERSION_PATCH}")
SET(PACKAGE_STRING "${PACKAGE_NAME} ${PACKAGE_VERSION}")


INCLUDE( ${LIBMINC_USE_FILE} )
INCLUDE( ${BICPL_USE_FILE} )

SET(CMAKE_REQUIRED_INCLUDES ${CMAKE_REQUIRED_INCLUDES} ${LIBMINC_INCLUDE_DIRS} ${BICPL_INCLUDE_DIRS})

ADD_DEFINITIONS(-DHAVE_CONFIG_H)
IF(NOT APPLE)
  ADD_DEFINITIONS(-D_XOPEN_SOURCE=500)
ENDIF(NOT APPLE)
# aliases
SET(VERSION "${PACKAGE_VERSION}")

set(HARD_CODED_REGISTER_DIRECTORY1 "${CMAKE_INSTALL_PREFIX}/lib${LIB_SUFFIX}")
set(HARD_CODED_REGISTER_DIRECTORY2 "/usr/local/bic/lib")
set(HARD_CODED_REGISTER_DIRECTORY2 "/usr/local/lib")


# check for prereqs
INCLUDE(CheckIncludeFiles)

CHECK_INCLUDE_FILES(float.h     HAVE_FLOAT_H)
CHECK_INCLUDE_FILES(dlfcn.h     HAVE_FLFCN_H)
CHECK_INCLUDE_FILES(sys/stat.h  HAVE_SYS_STAT_H)
CHECK_INCLUDE_FILES(sys/types.h HAVE_SYS_TYPES_H)
CHECK_INCLUDE_FILES(values.h    HAVE_VALUES_H)
CHECK_INCLUDE_FILES(unistd.h    HAVE_UNISTD_H)
CHECK_INCLUDE_FILES(memory.h    HAVE_MEMORY_H)
CHECK_INCLUDE_FILES(malloc.h    HAVE_MALLOC_H)
CHECK_INCLUDE_FILES(stdlib.h    HAVE_STDLIB_H)
CHECK_INCLUDE_FILES(fcntl.h     HAVE_FCNTL_H)
CHECK_INCLUDE_FILES(vfork.h     HAVE_VFORK_H)
CHECK_INCLUDE_FILES(inttypes.h  HAVE_INTTYPES_H)
CHECK_INCLUDE_FILES(string.h    HAVE_STRING_H)
CHECK_INCLUDE_FILES(strings.h   HAVE_STRINGS_H)

IF(FREEGLUT_FOUND) 
 SET(HAVE_FREEGLUT_H ON)
ELSE()
 CHECK_INCLUDE_FILES(GL/freeglut.h HAVE_FREEGLUT_H)
ENDIF()

# CHECK_INCLUDE_FILES(bicpl/deform.h HAVE_DEFORM_H)

IF(CMAKE_USE_PTHREADS_INIT)
  SET(HAVE_PTHREAD ON)
ELSE(CMAKE_USE_PTHREADS_INIT)
  SET(HAVE_PTHREAD OFF)
ENDIF(CMAKE_USE_PTHREADS_INIT)

IF(WINDOWS)
  CHECK_INCLUDE_FILES(windows.h   HAVE_WINDOWS_H)
ELSE(WINDOWS)
  SET(HAVE_WINDOWS_H OFF)
ENDIF(WINDOWS)

# others
CONFIGURE_FILE(${CMAKE_CURRENT_SOURCE_DIR}/config.h.cmake ${CMAKE_CURRENT_BINARY_DIR}/config.h)
CONFIGURE_FILE(${CMAKE_CURRENT_SOURCE_DIR}/epm-header.in  ${CMAKE_CURRENT_BINARY_DIR}/epm-header)

INCLUDE_DIRECTORIES( ${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_CURRENT_SOURCE_DIR})
INCLUDE_DIRECTORIES( ${GLUT_INCLUDE_DIR} )
INCLUDE_DIRECTORIES( ${BICGL_INCLUDE_DIRS} )
INCLUDE_DIRECTORIES( User_interface/Include Functionality/Include UI_calls_F/Include  )
LINK_DIRECTORIES( ${BICGL_LIBRARY_DIRS} )


SET(FUNCTIONALITY_SRC
  Functionality/initialize/initialize.c 
  Functionality/slices/colour_map.c 
  Functionality/slices/cursor.c 
  Functionality/slices/create_slice.c 
  Functionality/slices/initialize_slice.c 
  Functionality/slices/resample.c 
  Functionality/slices/save_image.c 
  Functionality/slices/set_volume.c 
  Functionality/slices/slices.c 
  Functionality/slices/updates.c 
  Functionality/tags/objects.c 
  Functionality/tags/save_and_load.c 
  Functionality/tags/tag_points.c 
  Functionality/tags/tag_transform.c 
  Functionality/update/update_window.c 
)


SET(UI_CALLS_F_SRC
  UI_calls_F/UI_calls_F.c
)



SET(USER_INTERFACE_SRC
  User_interface/colour_popup/colour_selection.c 
  User_interface/delete_tags_popup/delete_tags.c 
  User_interface/event_callbacks/save_image.c 
  User_interface/event_callbacks/slice_events.c 
  User_interface/event_callbacks/tag_events.c 
  User_interface/event_callbacks/utilities.c 
  User_interface/event_callbacks/window_events.c 
  User_interface/event_handling/event_callbacks.c 
  User_interface/event_handling/event_loop.c 
  User_interface/event_handling/event_viewports.c 
  User_interface/event_handling/global_events.c 
  User_interface/event_handling/window_events.c 
  User_interface/filter_popup/filter_selection.c 
  User_interface/input/load.c 
  User_interface/input/load_popup.c 
  User_interface/layout/colours.c 
  User_interface/layout/layout.c 
  User_interface/print_popup/print.c 
  User_interface/quit_popup/quit.c 
  User_interface/resampling/resample.c 
  User_interface/slices/slices.c 
  User_interface/transform_popup/xform_selection.c 
  User_interface/value_readout/update.c 
  User_interface/widget_instances/colour_bar.c 
  User_interface/widget_instances/initialize.c 
  User_interface/widget_instances/main_menu_callbacks.c 
  User_interface/widget_instances/merged_interface.c 
  User_interface/widget_instances/meter.c 
  User_interface/widget_instances/position_widgets.c 
  User_interface/widget_instances/tag_points.c 
  User_interface/widget_instances/volume_interface.c 
  User_interface/widgets/buttons.c 
  User_interface/widgets/sliders.c 
  User_interface/widgets/text_entry.c 
  User_interface/widgets/utilities.c 
  User_interface/widgets/widgets.c 
  User_interface/windows/lookup.c 
  User_interface/windows/popup.c 
  User_interface/windows/update.c 
  User_interface/windows/xsect.c  
  User_interface/main/main.c 
  User_interface/main/initialize.c
)


#----------------------------------------------------------------------
# RPATH handling
# use, i.e. don't skip the full RPATH for the build tree
SET(CMAKE_SKIP_BUILD_RPATH  FALSE)

# when building, don't use the install RPATH already
# (but later on when installing)
SET(CMAKE_BUILD_WITH_INSTALL_RPATH FALSE) 

#SET(CMAKE_INSTALL_RPATH "${MINC_INCLUDE_DIR}/../lib")

# add the automatically determined parts of the RPATH
# which point to directories outside the build tree to the install RPATH
SET(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)


ADD_EXECUTABLE(register 
  ${FUNCTIONALITY_SRC}
  ${UI_CALLS_F_SRC}
  ${USER_INTERFACE_SRC}
)

ADD_EXECUTABLE(register_resample
  Register_resample/register_resample.c
)

IF( LIBMINC_STATIC_LIBRARIES AND BICPL_STATIC_LIBRARIES)
  TARGET_LINK_LIBRARIES(register ${BICGL_LIBRARIES} ${BICPL_STATIC_LIBRARIES} ${LIBMINC_STATIC_LIBRARIES} ${GLUT_LIBRARIES})
ELSE( LIBMINC_STATIC_LIBRARIES AND BICPL_STATIC_LIBRARIES)
  TARGET_LINK_LIBRARIES(register ${BICGL_LIBRARIES} ${BICPL_LIBRARIES} ${LIBMINC_LIBRARIES} ${GLUT_LIBRARIES})
ENDIF( LIBMINC_STATIC_LIBRARIES AND BICPL_STATIC_LIBRARIES)  

TARGET_LINK_LIBRARIES(register_resample ${BICPL_LIBRARIES} ${VOLUME_IO_LIBRARIES} ${LIBMINC_LIBRARIES})


INSTALL(TARGETS register register_resample DESTINATION bin)

INSTALL_MAN_PAGES( ${CMAKE_INSTALL_PREFIX}/man Documentation/register.man1 )
