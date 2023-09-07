#include "Misc/Misc.h"

//    iOptQuiet                  // Quiet mode = 1
//   
//    Flavor                     // Arbitrary string (no space)
//   
//    OutDir                     // Output Dir Name
//   
//    Uploader                    
//                               // 2 DESY 21 root
//   
//    StartFrom                 // Set starting YT as
//                              // 0: weighthed
//                              // 1: Leading pad
//   
//    iOpt_Initial_YT_fit       // Do an initial fit on YT values  
//                              // 0: no initial fit
//                              // 1: the PRF from LUT at fixed RC and DD
//                              // 2: the PRF from TFile
//   
//    iOpt_TrackType            // Set the type of track to fit
//                              // 2: straight line
//                              // 3: parabola
//   
//    EventFile                 // Event file
//   
//    PRFfile_Fixed_RC_DD       // LUT at fixed RC and DD (possibly used in initial fit)
//   
//    PRFfile_Fixed_DD          // LUT at fixed DD (used to fit RC)
//   
//    ModuleNber                // Number of the Module Studied
//   
//    SelectionSet              // Selection Choice
//   
//    Data_to_Use = 0 ; Amax and Tmax from input ntuple
//                = 1 ; Amax and Tmax from 1st max of WF
//                = 2 ; Amax and Tmax from 1st clean max of WF
//                = 3 ; Amax and Tmax from fit of WF peak (Default)
//
void PV1_BASE(
  const int&         iOptQuiet           ,
  const std::string& Flavor              ,
  const std::string& OutDir              ,
  const std::string& EventFile           ,
  const std::string& PRFfile_Fixed_RC_DD ,
  const std::string& PRFfile_Fixed_DD    ,
  const int&         Uploader            ,
  const int&         StartFrom           ,
  const int&         iOpt_Initial_YT_fit ,
  const int&         iOpt_TrackType      ,
  const int&         ModuleNber              ,
  const std::string& SelectionSet        ,
  const int&         Data_to_Use  
);
