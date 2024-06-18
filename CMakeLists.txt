# Minimum CMake version
cmake_minimum_required(VERSION 3.10)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Project name
project(dEdxRecon)

# Find ROOT
find_package(ROOT REQUIRED COMPONENTS RIO Tree)

# Include ROOT directories
include(${ROOT_USE_FILE})

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
ROOT_GENERATE_DICTIONARY(G__dEdxReconLib
  Reconstruction/inc/dEdx.h Reconstruction/inc/LUTs.h
  LINKDEF Reconstruction/inc/Recon_LinkDef.h
  MODULE dEdxReconLib)

# Link the dictionary to the library
target_link_libraries(dEdxReconLib PUBLIC ${ROOT_LIBRARIES})

# Add executable target
add_executable(Analysis Apps/Analysis.cxx)

# Link library to executable
target_link_libraries(Analysis dEdxReconLib ${ROOT_LIBRARIES})

# Add more debug information, including line number in case of segfault
set(CMAKE_BUILD_TYPE "Debug")
target_compile_options(Analysis PRIVATE -fno-omit-frame-pointer)
target_link_options(Analysis PRIVATE -fno-omit-frame-pointer)