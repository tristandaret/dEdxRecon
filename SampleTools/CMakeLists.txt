# Add source files
file(GLOB SOURCES "src/*.cxx")

# Add a library target
add_library(SampleToolsLibrary ${SOURCES})

# Set include directories for FittersLibrary target
target_include_directories(SampleToolsLibrary PUBLIC inc)

# Generate ROOT dictionary
ROOT_GENERATE_DICTIONARY(SampleToolsDict inc/Uploader.h LINKDEF inc/LinkDef.h)

# Link the ROOT dictionary to the library
target_link_libraries(SampleToolsLibrary PUBLIC ${SampleToolsDict} MiscLibrary EvtModelLibrary)
