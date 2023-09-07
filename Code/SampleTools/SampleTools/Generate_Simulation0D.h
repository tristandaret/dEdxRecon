#include "Misc/Misc.h"

// 
//  Output_RootFile = Name of the output File
// 
//  iOptSmear       = 0 : no Smearing
//                  = 1 : cte sigma
//                  = 2 : sqrt(A) sigma
//
//  RC_ns_mmM2      = RC
//
//  Gain            = Gain
//
//  DD_mm           = Drift Distamce
//
//  NberOfEvents    = Nber of Events to be simulated
//
void Generate_Simulation0D( 
                         const int&    iOptQuiet            ,
                         const std::string& Output_RootFile ,
                         const int&    iOptSmear            ,
                         const double& RC_ns_mmM2           ,
                         const double& Gain                 ,
                         const double& DD_mm                ,
                         const int&    NberOfEvents
);
 
