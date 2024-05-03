# Set include directory
include_directories(inc)

# Add source files
file(GLOB SOURCES "src/*.cxx")

# Add a library target
add_library(SampleToolsLibrary ${SOURCES})

# Generate ROOT dictionary
ROOT_GENERATE_DICTIONARY(SampleToolsDict inc/Uploader.h LINKDEF inc/LinkDef.h)

# Link the ROOT dictionary to the library
target_link_libraries(SampleToolsLibrary ${SampleToolsDict})
