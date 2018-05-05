# Setup the project
cmake_minimum_required(VERSION 2.6 FATAL_ERROR)
project(B1)

option(WITH_GEANT4_UIVIS "Build example with Geant4 UI and Vis drivers" ON)
if(WITH_GEANT4_UIVIS)
  find_package(Geant4 REQUIRED ui_all vis_all)
else()
  find_package(Geant4 REQUIRED)
endif()

include(${Geant4_USE_FILE})
include_directories(${PROJECT_SOURCE_DIR}/include)
#----------------------------------------------------------------------------
# Find ROOT variables if the variable G4ANALYSIS_USE_ROOT is set
#

find_package(ROOT REQUIRED)
if(ROOT_FOUND)
add_definitions(-DG4ANALYSIS_USE_ROOT)
include_directories(${ROOT_INCLUDE_DIR} ${Geant4_INCLUDE_DIR} ${PROJECT_SOURCE_DIR}/include)
message(STATUS "ROOT found. --> with ROOT enabled.")
else()
include_directories(${PROJECT_SOURCE_DIR}/include)
message(STATUS "ROOT not found. --> with ROOT disabled.")	
endif()



# Locate sources and headers for this project
# NB: headers are included so they will show up in IDEs
file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc)
file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.hh)

# Add the executable, and link it to the Geant4 libraries
add_executable(exampleB1 exampleB1.cc ${sources} ${headers})
if(ROOT_FOUND)
target_link_libraries(exampleB1 ${Geant4_LIBRARIES} ${ROOT_LIBRARIES})
else()
target_link_libraries(exampleB1 ${Geant4_LIBRARIES})
endif()

# Install the executable to 'bin' directory under CMAKE_INSTALL_PREFIX
install(TARGETS exampleB1 DESTINATION bin)


