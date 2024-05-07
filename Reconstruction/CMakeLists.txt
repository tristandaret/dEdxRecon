# Add source files
file(GLOB SOURCES "src/*.cxx")

# Add a library target
add_library(ReconstructionLibrary ${SOURCES})

# Set include directories for FittersLibrary target
target_include_directories(ReconstructionLibrary PUBLIC inc)

# Generate ROOT dictionary
root_generate_dictionary(Reconstruction_dict inc/dEdx.h MODULE ReconstructionLibrary LINKDEF inc/dEdx_LinkDef.h)
root_generate_dictionary(ERAMinfo inc/LUTs.h MODULE ReconstructionLibrary LINKDEF inc/LUTs_LinkDef.h)

# Link the ROOT dictionary to the library
target_link_libraries(ReconstructionLibrary PUBLIC MiscLibrary SignalModelLibrary EvtModelLibrary FittersLibrary SampleToolsLibrary EvtModelToolsLibrary)