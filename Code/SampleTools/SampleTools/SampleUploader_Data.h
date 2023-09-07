#include "Misc/Misc.h"
#include "EvtModel/Sample.h"

//Upload data 
//
//      intUploader =    2 ; // DESY 21
//                  =    3 ; // CERN 22
//                  =    4 ; // CERN 22 V4
//                  =    5 ; // David Henaf format, keep BeforeMerging=0
//                  =    6 ; // David Henaf format, keep BeforeMerging=1
//                  =   51 ; // David Henaf format, keep BeforeMerging=0 + Swap XY
//                  =   61 ; // David Henaf format, keep BeforeMerging=1 + Swap XY
//                  =  100 ; // Simulation Home made
//
//      aSample : output object
//
//      ModuleNber_Input : Nber of the Module to load 
//
//      SampleFile : int root file
//
//      Data_to_Use = 0 ; Amax and Tmax from input ntuple
//                  = 1 ; Amax and Tmax from 1st max of WF
//                  = 2 ; Amax and Tmax from 1st clean max of WF
//                  = 3 ; Amax and Tmax from fit of WF peak  
//
void SampleUploader_Data(
                         const int&         intUploader      , 
                         Sample&            aSample          , 
                         const int&         ModuleNber_Input , 
                         const std::string& SampleFile       , 
                         const int&         Data_to_Use 
);
void SampleUploader_Data(
                         const int&         MaxNberOfEvents  , 
                         const int&         intUploader      , 
                         Sample&            aSample          , 
                         const int&         ModuleNber_Input , 
                         const std::string& SampleFile       , 
                         const int&         Data_to_Use 
);
