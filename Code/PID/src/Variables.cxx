#include "PID/Variables.h"

namespace PID{

    // Output file variables
    std::string outDir = "OUT_PID/";
    std::string tag;
    std::string comment;
    std::string selectionSet;

    // Run variables
    std::string prtcle; 
    std::string dataFile;
    std::string driftMethod = "zcalc";
    Uploader*   pUploader;
    int         intUploader;
    int         moduleCase; // DESY21:0 | CERN22:-1
    int         PT;         // Peaking time (ns)
    int         Dt;         // Transverse diffusion coefficient (µm/sqrt(cm))
    int         driftDist;  // drift distance (mm)
    int         TB;         // timebin length (ns)
    bool        WFupdated = true;
    int         WFversion = 0;
    bool        fgainCorrection = true;

}