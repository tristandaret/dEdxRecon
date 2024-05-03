# Set include directory
include_directories(inc)

# Add source files
file(GLOB SOURCES "src/*.cxx")

# Add a library target
add_library(ReconstructionLibrary ${SOURCES})

# Generate ROOT dictionary
ROOT_GENERATE_DICTIONARY(OutPutDict inc/dEdx.h LINKDEF inc/dEdx_LinkDef.h)

# Link the ROOT dictionary to the library
target_link_libraries(ReconstructionLibrary ${OutPutDict})