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
//    iOpt_TrackType            // Set the type of track to fit
//                              // 2: straight line
//                              // 3: parabola
//   
//    EventFile                 // Event file
//   
//    FILE_SC_VS_x_rc_dd        // 3D LUT  
//   
//    DD_Sample                 // DD value to fix DD in the 3D LUT to fit RC
//   
//    SamplingFrequency         // Sampling Frequency in MHz
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
void PV3_BASE(
  const int&         iOptQuiet          ,
  const std::string& Flavor             ,
  const std::string& OutDir             ,
  const std::string& EventFile          ,
  const std::string& FILE_SC_VS_x_rc_dd ,
  const double&      DD_Sample          ,
  const int&         Uploader           ,
  const int&         StartFrom          ,
  const int&         iOpt_TrackType     ,
  const double&      SamplingFrequency  ,
  const int&         ModuleNber         ,
  const std::string& SelectionSet       ,
  const int&         Data_to_Use  
);
