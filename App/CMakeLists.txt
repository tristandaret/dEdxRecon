# Add executable target
add_executable(App App.cxx)

# Link libraries
target_link_libraries(App EvtModelLibrary EvtModelToolsLibrary FittersLibrary MiscLibrary ReconstructionLibrary SampleToolsLibrary SignalModelLibrary)