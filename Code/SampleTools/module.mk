# Source files to compile (without suffix)

FILES := src/SampleUploader_Data
FILES += src/GiveMe_Uploader
FILES += src/Uploader
FILES += src/Uploader_DESY21
FILES += src/Uploader_CERN22
FILES += src/Uploader_CERN22_V4
FILES += src/Uploader_Simulation
FILES += src/Generate_Simulation1D
FILES += src/Generate_Simulation0D
FILES += src/Uploader_David
FILES += src/Uploader_David
FILES += src/ReadRCmap
FILES += src/ReadGainmap
FILES += src/Uploader_ReadRCmap
FILES += src/Uploader_ReadGainmap

# Header files to use for dictionary generation
DICTFILES :=  SampleTools/LinkDef

# Executable files
PROGRAMS :=

NEEDS_ROOT := yes


