cmake_minimum_required(VERSION 2.6 FATAL_ERROR)
project(estandar)

option(WITH_GEANT4_UIVIS "Build example with Geant4 UI and Vis drivers" ON)
if(WITH_GEANT4_UIVIS)
  find_package(Geant4 REQUIRED ui_all vis_all)
else()
  find_package(Geant4 REQUIRED)
endif()

include(${Geant4_USE_FILE})
include_directories(${PROJECT_SOURCE_DIR}/include)

find_package(ROOT REQUIRED)
if(ROOT_FOUND)
add_definitions(-DG4ANALYSIS_USE_ROOT)
include_directories(${ROOT_INCLUDE_DIR} ${Geant4_INCLUDE_DIR} ${PROJECT_SOURCE_DIR}/include)
message(STATUS "ROOT found. --> with ROOT enabled.")
else()
include_directories(${PROJECT_SOURCE_DIR}/include)
message(STATUS "ROOT not found. --> with ROOT disabled.")	
endif()

file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc)
file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.hh)

add_executable(crossection crossection.cc ${sources} ${headers})
if(ROOT_FOUND)
target_link_libraries(crossection ${Geant4_LIBRARIES} ${ROOT_LIBRARIES})
else()
target_link_libraries(crossection ${Geant4_LIBRARIES})
endif()

install(TARGETS crossection DESTINATION bin)


