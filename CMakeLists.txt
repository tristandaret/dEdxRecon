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

# Add subdirectory of executable
add_subdirectory(App)

# Add subdirectories
add_subdirectory(Misc)
add_subdirectory(SignalModel)
add_subdirectory(EvtModel)
add_subdirectory(Fitters)
add_subdirectory(SampleTools)
add_subdirectory(EvtModelTools)
add_subdirectory(Reconstruction)
