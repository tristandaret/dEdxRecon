#  Minimum CMake version
cmake_minimum_required(VERSION 3.10)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#  Project name
project(dEdxRecon)

#  Find ROOT
find_package(ROOT REQUIRED COMPONENTS RIO Tree)
include_directories(${ROOT_INCLUDE_DIRS})

# Add source files
file(GLOB SOURCES "*/src/*.cxx")

# Add a library target
add_library(dEdxReconLib ${SOURCES})

# Include directories
target_include_directories(dEdxReconLib PUBLIC Fitters/inc)
target_include_directories(dEdxReconLib PUBLIC ModelEvent/inc)
target_include_directories(dEdxReconLib PUBLIC ModelSignal/inc)
target_include_directories(dEdxReconLib PUBLIC Reconstruction/inc)
target_include_directories(dEdxReconLib PUBLIC Uploader/inc)
target_include_directories(dEdxReconLib PUBLIC Utilities/inc)

# Generate dictionaries
root_generate_dictionary(Event_dict     Reconstruction/inc/dEdx.h LINKDEF Reconstruction/inc/Recon_LinkDef.h MODULE dEdxReconLib)
root_generate_dictionary(ERAMinfo_dict  Reconstruction/inc/LUTs.h LINKDEF Reconstruction/inc/Recon_LinkDef.h MODULE dEdxReconLib)

# Add executable target
add_executable(Analysis Analysis.cxx) 

# Link library to executable
target_link_libraries(Analysis dEdxReconLib ${ROOT_LIBRARIES})