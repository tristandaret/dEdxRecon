#ifndef Reconstruction_VARIABLES_H
#define Reconstruction_VARIABLES_H

#include "Misc.h"
#include "Uploader.h"

namespace Reconstruction{
    
    extern Uploader    *p_uploader;

    // Output file variables
    extern std::string outDir;
    extern std::string outFile_dEdx;
    extern std::string tag;
    extern std::string comment;
    extern std::string selectionSet;

    // Run variables
    extern std::string prtcle; 
    extern std::string dataFile;
    extern std::string driftMethod;
    extern Uploader*   pUploader;
    extern int         intUploader;
    extern int         moduleCase; // DESY21:0 | CERN22:-1
    extern int         PT;         // Peaking time (ns)
    extern int         Dt;         // Transverse diffusion coefficient (µm/sqrt(cm))
    extern int         driftDist;  // drift distance (mm)
    extern int         TB;         // timebin length (ns)
    extern bool        fcorrectGain;
    extern bool        fcorrectRC;
    extern bool        WFupdated;
    extern int         WFversion;
}

#endif