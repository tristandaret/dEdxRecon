#include "JeanFrancois/PV1_CERN22.h"
#include "Misc/Util.h"

#include "Procedures/PV1_BASE.h"

void PV1_CERN22()
{ 
  std::string OutDir      = "OUT_JeanFrancois/" ;
  
  int iOptQuiet           = 0 ;
  
  int intUploader         = 3 ; // CERN22 Data 
  int StartFrom           = 0 ;
  int iOpt_Initial_YT_fit = 0 ;
  int iOpt_TrackType      = 3 ;
  int ModuleNber          = 0 ;
  
  std::string SelectionSet = "BN_DESY21_Sample" ;
  int Data_to_Use          = 0 ;
  
  std::cout << std::endl ;
  std::cout << " Analysis: PV1 "  << std::endl ;
    
//
  std::string Tag = "BIDON" ;
  std::string EvtFil = "BIDON" ;

//
  int ISAMPLE_Min = 7 ;
  int ISAMPLE_Max = 8 ;
  for (int ISAMPLE = ISAMPLE_Min; ISAMPLE< ISAMPLE_Max; ISAMPLE++){
  
//--------------------------------------------------------//ISAMPLE 7
    if(ISAMPLE==7){
      Tag = "SIM_221006" ;  

      std::string SelectionSet = "AN_CERN22_Sample_Sim" ;

      ModuleNber = 0 ;
      
      int iFl_Min = 1 ;
      int iFl_Max = 9 ;
      for (int iFl = iFl_Min ; iFl< iFl_Max; iFl++){
        if (iFl==0) { EvtFil = "../Sim_Data/z_400_nomDrift_10cm_MD_RC100_v2_iter4.root" ; }
        if (iFl==1) { EvtFil = "../Sim_Data/z_400_nomDrift_20cm_MD_RC100_v2_iter4.root" ; }
        if (iFl==2) { EvtFil = "../Sim_Data/z_400_nomDrift_30cm_MD_RC100_v2_iter4.root" ; }
        if (iFl==3) { EvtFil = "../Sim_Data/z_400_nomDrift_40cm_MD_RC100_v2_iter4.root" ; }
        if (iFl==4) { EvtFil = "../Sim_Data/z_400_nomDrift_50cm_MD_RC100_v2_iter4.root" ; }
        if (iFl==5) { EvtFil = "../Sim_Data/z_400_nomDrift_60cm_MD_RC100_v2_iter4.root" ; }
        if (iFl==6) { EvtFil = "../Sim_Data/z_400_nomDrift_70cm_MD_RC100_v2_iter4.root" ; }
        if (iFl==7) { EvtFil = "../Sim_Data/z_400_nomDrift_80cm_MD_RC100_v2_iter4.root" ; }
        if (iFl==8) { EvtFil = "../Sim_Data/z_400_nomDrift_90cm_MD_RC100_v2_iter4.root" ; }

        std::string PRFfile_Fixed_RC_DD = "BIDON" ;
        PRFfile_Fixed_RC_DD  = "Data_SignalTime/SignalTime_True_400ns_RC100.00_DD0.00.csv"  ; 

        std::string PRFfile_Fixed_DD = "BIDON" ;
        PRFfile_Fixed_DD   = "Data_SignalTime/LoadRC_400ns_DD_00.00.csv"  ;

        PV1_BASE(
                 iOptQuiet           ,
                 Tag                 ,
                 OutDir              ,
                 EvtFil              ,
                 PRFfile_Fixed_RC_DD ,
                 PRFfile_Fixed_DD    ,
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
    
//--------------------------------------------------------//ISAMPLE 7
  
  }

  
}
