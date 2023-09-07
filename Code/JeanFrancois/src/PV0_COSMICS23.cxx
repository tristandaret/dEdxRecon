#include "JeanFrancois/PV0_COSMICS23.h"
#include "Misc/Util.h"

#include "Procedures/PV0_BASE.h"

void PV0_COSMICS23()
{ 
  std::string OutDir      = "OUT_JeanFrancois/" ;
  
  int MaxNberOfEvents     = 100000 ;
  
  int iOptQuiet           = 0 ;
  
  int intUploader         = 5 ; // David Henaf format, keep BeforeMerging=0
  int StartFrom           = 0 ;
  int iOpt_Initial_YT_fit = 0 ;
  int iOpt_TrackType      = 3 ;
  int ModuleNber          = 4 ;
  
  std::string SelectionSet = "BN_COSMICS23_Sample" ;
  int Data_to_Use          = 0 ;
  
//
  std::string Tag = "BIDON" ;
  std::string EvtFil = "BIDON" ;

//
  int ISAMPLE_Min = 0 ;
  int ISAMPLE_Max = 1 ;
  for (int ISAMPLE = ISAMPLE_Min; ISAMPLE< ISAMPLE_Max; ISAMPLE++){

//--------------------------------------------------------//ISAMPLE 0
    if(ISAMPLE==0){
      Tag = "TEST" ;  

      ModuleNber = 1 ;
      
      Data_to_Use = 0 ;
      
      SelectionSet = "BN_COSMICS23_Sample" ;
      
      intUploader = 5 ; // David Henaf format, keep BeforeMerging=0
      
      int iFl_Min = 0 ;
      int iFl_Max = 1 ;
      for (int iFl = iFl_Min ; iFl< iFl_Max; iFl++){
//      if (iFl==0) { EvtFil = "../Real_Data/COSMICS23/TEST/R2022_10_29-15_59_15-000_iter0.root" ; }
        if (iFl==0)  EvtFil = "../Real_Data/COSMICS23/TEST/run00002_00_iter0.root" ;

        std::string PRFfile_Fixed_RC_DD =  "Data_SignalTime/SignalTime_True_400ns_RC100.00_DD0.00.csv"  ; 

        PV0_BASE(
                 MaxNberOfEvents     ,
                 iOptQuiet           ,
                 Tag                 ,
                 OutDir              ,
                 EvtFil              ,
                 PRFfile_Fixed_RC_DD ,
                 intUploader         ,
                 StartFrom           ,
                 iOpt_Initial_YT_fit ,
                 iOpt_TrackType      ,
                 ModuleNber          ,
                 SelectionSet        ,
                 Data_to_Use
                );
      }
      
    }
    
//--------------------------------------------------------//ISAMPLE 0

  }

}
