# Add source files
file(GLOB SOURCES "src/*.cxx")

# Add a library target
add_library(EvtModelToolsLibrary ${SOURCES})

# Set include directories for FittersLibrary target
target_include_directories(EvtModelToolsLibrary PUBLIC inc)

# Link against other required libraries
target_link_libraries(EvtModelToolsLibrary PUBLIC MiscLibrary SignalModelLibrary EvtModelLibrary FittersLibrary SampleToolsLibrary)