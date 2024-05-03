#  Minimum CMake version
cmake_minimum_required(VERSION 3.10)

#  Project name
project(dEdxRecon)

#  Find ROOT
find_package(ROOT REQUIRED)

# Add subdirectory of executable
add_subdirectory(App)

# Add subdirectories
add_subdirectory(EvtModel)
add_subdirectory(EvtModelTools)
add_subdirectory(Fitters)
add_subdirectory(Misc)
add_subdirectory(Reconstruction)
add_subdirectory(SampleTools)
add_subdirectory(SignalModel)
