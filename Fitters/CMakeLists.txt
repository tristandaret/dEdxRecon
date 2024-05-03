# Set include directory
include_directories(inc)

# Add source files
file(GLOB SOURCES "src/*.cxx")

# Add a library target
add_library(FittersLibrary ${SOURCES})