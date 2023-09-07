#include "JeanFrancois/EVT_DESY21_01.h"
#include "Misc/Util.h"

#include "Procedures/EVT_01_BASE.h"

void EVT_DESY21_01()
{ 
//
  std::string OutDir       = "OUT_JeanFrancois/" ;

  int iOptQuiet            = 0 ;

  std::string SelectionSet = "BN_DESY21_Event" ;

  int intUploader          =  2 ; // DESY21 Data 
  int ModuleNber           = -1 ; // Work on all modules nbers
  int Data_to_Use          = 0  ; // Use ntuple Maxs

//
  std::string Tag = "BIDON" ;

  std::string EvtFil = "BIDON" ;

  double Stg120_L  = -1. ;
  double Stg120_H = 1000. ;
  
//
  int ISAMPLE_Min = 0 ;
  int ISAMPLE_Max = 1 ;
  for (int ISAMPLE = ISAMPLE_Min; ISAMPLE< ISAMPLE_Max; ISAMPLE++){
  
//--------------------------------------------------------//ISAMPLE 0
    if(ISAMPLE==0){
      Tag = "EVT_DESY21_01" ;  
      
      Stg120_L = -1. ; Stg120_H = 1000. ; EvtFil = "../Real_Data/DESY21/ZSCAN_412/z_360_275_412_02T_26_60_iter0.root";
      
      EVT_01_BASE(
                         iOptQuiet ,
                         Tag ,
                         OutDir ,
                         EvtFil ,
                         SelectionSet ,
                         intUploader ,
                         ModuleNber  ,
                         Data_to_Use ,
                         Stg120_L ,
                         Stg120_H 
                        ) ;
    }
//--------------------------------------------------------//ISAMPLE 0

  }



}
