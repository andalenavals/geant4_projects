#----------------------------------------------------------------------------
# Setup the project
cmake_minimum_required(VERSION 2.6 FATAL_ERROR)
project(GFNUNdetectors)

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

#----------------------------------------------------------------------------
# Setup Geant4 include directories and compile definitions
#----------------------------------------------------------------------------
include(${Geant4_USE_FILE})
include_directories(${PROJECT_SOURCE_DIR}/include 
                    ${Geant4_INCLUDE_DIR})

#include_directories(${PROJECT_SOURCE_DIR}/include)

#----------------------------------------------------------------------------
# Locate sources and headers for this project
# NB: headers are included so they will show up in IDEs
#
file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc)
file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.hh)

#----------------------------------------------------------------------------
# Add ROOT include and libraries in an awful way. Is there a better way?
# perhaps people can use find_package(ROOT REQUIRED) if you distro allows you to.
# ArchLinux does not provide such files so I have this AWFUL workaround to avoid
# the following error:
# 
#  Could not find module FindROOT.cmake or a configuration file for package
#  ROOT.
#
#  Adjust CMAKE_MODULE_PATH to find FindROOT.cmake or set ROOT_DIR to the
#  directory containing a CMake configuration file for ROOT.  The file will
#  have one of the following names:
#
#    ROOTConfig.cmake
#    root-config.cmake
#
# 
set(CMAKE_EXE_LINKER_FLAGS 
"-L/usr/lib/root -lCore -lCint -lRIO -lNet -lHist -lGraf -lGraf3d -lGpad -lTree -lRint -lPostscript -lMatrix -lPhysics -lMathCore -lThread -pthread -lm -ldl -rdynamic")
include_directories(/usr/include/root/)

#find_package(ROOT REQUIRED)

#----------------------------------------------------------------------------
# Add the executable, and link it to the Geant4 libraries
#

#set(ROOT_LIBRARY_PROPERTIES ${ROOT_LIBRARY_PROPERTIES}) ## test DFlechas
add_executable(GFNUNdetectors GFNUNdetectors.cc ${sources} ${headers})
#target_link_libraries(GFNUNdetectors ${Geant4_LIBRARIES}) 
target_link_libraries(GFNUNdetectors ${Geant4_LIBRARIES} ${ROOT_LIBRARIES}) ## test DFlechas

#----------------------------------------------------------------------------
# Copy all scripts to the build directory, i.e. the directory in which we
# build GFNUNdetectors. This is so that we can run the executable directly because
# it relies on these scripts being in the current working directory.
#
set(GFNUNdetectors_SCRIPTS
THEmacro.mac
GPS.mac
MAINmacro.mac
William.mac
Cf252NeutronVertex.txt
Neutrons.mac
vis.mac
visQt.mac
Andres.mac
)

foreach(_script ${GFNUNdetectors_SCRIPTS})
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
install(TARGETS GFNUNdetectors DESTINATION bin)
