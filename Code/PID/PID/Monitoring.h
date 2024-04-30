#ifndef MONITORING_H
#define MONITORING_H

#include "Misc/Misc.h"
#include "SampleTools/Uploader.h"
#include "PID/LUTs.hxx"
#include "PID/dEdx.h"

namespace PID{

    void Monitoring();
    extern PID::dEdx   *p_dEdx;
    extern Uploader    *p_uploader;
    extern PID::LUT    *p_lut;

    // Output file variables
    extern std::string outDir;
    extern std::string dataFile;
    extern std::string tag;
    extern std::string comment;
    extern std::string selectionSet;

    // Run variables
    extern std::string prtcle; 
    extern std::string EvtFile;
    extern std::string driftMethod;
    extern Uploader*   pUploader;
    extern int         intUploader;
    extern int         moduleCase; // DESY21:0 | CERN22:-1
    extern int         PT;         // Peaking time (ns)
    extern int         Dt;         // Transverse diffusion coefficient (µm/sqrt(cm))
    extern int         driftDist;  // drift distance (mm)
    extern int         TB;         // timebin length (ns)
    extern bool        WFupdated;
    extern int         WFversion;
}

#endif