#----------------------------------------------------------------------------
# Setup the project
cmake_minimum_required(VERSION 2.6 FATAL_ERROR)
project(ComptonCameraFCF)

#----------------------------------------------------------------------------
# Find Geant4 package, activating all available UI and Vis drivers by default
# You can set WITH_GEANT4_UIVIS to OFF via the command line or ccmake/cmake-gui
# to build a batch mode only executable
#
option(WITH_GEANT4_UIVIS "Build example with Geant4 UI and Vis drivers" ON)
if(WITH_GEANT4_UIVIS)
	find_package(Geant4 REQUIRED ui_all vis_all)
else()
	find_package(Geant4 REQUIRED)
endif()

include(${Geant4_USE_FILE})
include_directories(${PROJECT_SOURCE_DIR}/include ${PROJECT_SOURCE_DIR}/include/Geometry)



find_package(ROOT REQUIRED)

if(ROOT_FOUND)
	add_definitions(-DG4ANALYSIS_USE_ROOT)
	include_directories(${ROOT_INCLUDE_DIR} ${Geant4_INCLUDE_DIR} ${PROJECT_SOURCE_DIR}/include)
	message(STATUS "The Compton Camera FCF: ROOT found. --> ComptonCamera with ROOT enabled.")
else()
	include_directories(${PROJECT_SOURCE_DIR}/include)
	message(STATUS "The Compton Camera FCF: ROOT not found. --> ComptonCamera with ROOT disabled.")	
endif()

#----------------------------------------------------------------------------
# Locate sources and headers for this project
# NB: headers are included so they will show up in IDEs
#
file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc ${PROJECT_SOURCE_DIR}/src/Geometry/*.cc )
file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.hh ${PROJECT_SOURCE_DIR}/include/Geometry/*.hh )

#----------------------------------------------------------------------------
# Add the executable, and link it to the Geant4 libraries
#
add_executable(ComptonCameraFCF TheComptonCameraFCF.cc ${sources} ${headers})

if(ROOT_FOUND)
target_link_libraries(ComptonCameraFCF ${Geant4_LIBRARIES} ${ROOT_LIBRARIES})
else()
target_link_libraries(ComptonCameraFCF ${Geant4_LIBRARIES})
endif()

#----------------------------------------------------------------------------
# Copy all scripts to the build directory, i.e. the directory in which we
# build ComptonCameraFCF. This is so that we can run the executable directly because
# it relies on these scripts being in the current working directory.
#
set(ComptonCameraFCF_SCRIPTS
THEmacro.mac
MAINmacro.mac
vis.mac
visQt.mac
init_vis.mac
)

foreach(_script ${ComptonCameraFCF_SCRIPTS})
message("Copying macro file ${_script}")
configure_file(
${PROJECT_SOURCE_DIR}/macros/${_script}
${PROJECT_BINARY_DIR}/macros/${_script}
COPYONLY
)
endforeach()

#----------------------------------------------------------------------------
# Install the executable to 'bin' directory under CMAKE_INSTALL_PREFIX
#
install(TARGETS ComptonCameraFCF DESTINATION bin)
