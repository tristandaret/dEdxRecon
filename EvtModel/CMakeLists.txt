# Add source files
file(GLOB SOURCES "src/*.cxx")

# Add a library target
add_library(EvtModelLibrary ${SOURCES})

# Set include directories for FittersLibrary target
target_include_directories(EvtModelLibrary PUBLIC inc)

# Link against other required libraries
target_link_libraries(EvtModelLibrary PUBLIC MiscLibrary SignalModelLibrary)