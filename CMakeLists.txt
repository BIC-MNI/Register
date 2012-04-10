# CMakeFiles.txt for the ray_trace
#
# Vladimir S. FONOV - vladimir.fonov@gmail.com

CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

SET(REGISTER_PACKAGE_VERSION_MAJOR 1)
SET(REGISTER_PACKAGE_VERSION_MINOR 4)
SET(REGISTER_PACKAGE_VERSION_PATCH 00)

IF(NOT MINC_TOOLKIT_BUILD)

  PROJECT(Register "C")

  # Packaging defines
  SET(CPACK_GENERATOR TGZ)
  SET(CPACK_PACKAGE_VERSION_MAJOR ${REGISTER_PACKAGE_VERSION_MAJOR})
  SET(CPACK_PACKAGE_VERSION_MINOR ${REGISTER_PACKAGE_VERSION_MINOR})
  SET(CPACK_PACKAGE_VERSION_PATCH ${REGISTER_PACKAGE_VERSION_PATCH})
  INCLUDE(CPack)

  FIND_PACKAGE( MINC2 REQUIRED )
  FIND_PACKAGE( BICPL REQUIRED )
#  FIND_PACKAGE( Perl REQUIRED )
  FIND_PACKAGE( Threads )
  FIND_PACKAGE( OpenGL REQUIRED )
  FIND_PACKAGE( GLUT REQUIRED )
ENDIF(NOT MINC_TOOLKIT_BUILD)

IF(APPLE)
  SET(HAVE_APPLE_OPENGL_FRAMEWORK ON)
ENDIF(APPLE)

SET(PACKAGE "Register")
SET(PACKAGE_BUGREPORT "a.janke@gmail.com")

SET(PACKAGE_NAME "Register")
SET(PACKAGE_VERSION "${REGISTER_PACKAGE_VERSION_MAJOR}.${REGISTER_PACKAGE_VERSION_MINOR}.${REGISTER_PACKAGE_VERSION_PATCH}")
SET(PACKAGE_STRING "${PACKAGE_NAME} ${PACKAGE_VERSION}")


INCLUDE( ${MINC2_USE_FILE} )
INCLUDE( ${BICPL_USE_FILE} )

SET(CMAKE_REQUIRED_INCLUDES ${CMAKE_REQUIRED_INCLUDES} ${MINC2_INCLUDE_DIRS} ${BICPL_INCLUDE_DIRS})

ADD_DEFINITIONS(-DHAVE_CONFIG_H)
ADD_DEFINITIONS()

# aliases
SET(VERSION "${PACKAGE_VERSION}")

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


ADD_EXECUTABLE(register 
  ${FUNCTIONALITY_SRC}
  ${UI_CALLS_F_SRC}
  ${USER_INTERFACE_SRC}
)

TARGET_LINK_LIBRARIES(register bicgl ${BICPL_LIBRARIES} ${VOLUME_IO_LIBRARIES} ${MINC2_LIBRARIES} ${GLUT_LIBRARIES})

INSTALL(TARGETS register DESTINATION bin)
