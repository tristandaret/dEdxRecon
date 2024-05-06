# include reconstruction library
include_directories((${CMAKE_SOURCE_DIR}/Reconstruction/inc))

# Add executable target
add_executable(App App.cxx)

# Link libraries
target_link_libraries(App MiscLibrary SignalModelLibrary EvtModelLibrary FittersLibrary SampleToolsLibrary EvtModelToolsLibrary ReconstructionLibrary ${ROOT_LIBRARIES})