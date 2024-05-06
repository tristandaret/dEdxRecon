# Add source files
file(GLOB SOURCES "src/*.cxx")

# Add a library target
add_library(FittersLibrary ${SOURCES})

# Set include directories for FittersLibrary target
target_include_directories(FittersLibrary PUBLIC inc)

# Link against other required libraries
target_link_libraries(FittersLibrary PUBLIC MiscLibrary EvtModelLibrary)