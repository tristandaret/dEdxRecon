# Add source files
file(GLOB SOURCES "src/*.cxx")

# Add a library target
add_library(MiscLibrary ${SOURCES})

# Set include directories for FittersLibrary target
target_include_directories(MiscLibrary PUBLIC inc)