# Add source files
file(GLOB SOURCES "src/*.cxx")

# Add a library target
add_library(SignalModelLibrary ${SOURCES})

# Set include directories for FittersLibrary target
target_include_directories(SignalModelLibrary PUBLIC inc)

# Link against other required libraries
target_link_libraries(SignalModelLibrary PUBLIC MiscLibrary)