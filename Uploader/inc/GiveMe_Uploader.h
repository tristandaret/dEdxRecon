#include "Misc.h"
#include "Uploader.h"

//
//      intUploader = 11
//                  =    3 ; // CERN 22
//                  =    4 ; // CERN 22 V4
//                  =    5 ; // David Henaf format, keep BeforeMerging=0
//                  =    6 ; // David Henaf format, keep BeforeMerging=1
//                  =   51 ; // David Henaf format, keep BeforeMerging=0 + Swap XY
//                  =   61 ; // David Henaf format, keep BeforeMerging=1 + Swap XY
//                  =  100 ; // Simulation Home made
//
//      SampleFile : input root file
//
Uploader* GiveMe_Uploader(
                          const int& intUploader,
                          const std::string& SampleFile 
);
