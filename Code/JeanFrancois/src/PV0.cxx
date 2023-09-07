#include "JeanFrancois/PV0.h"
#include "Misc/Util.h"

#include "Procedures/PV0_BASE.h"

void PV0()
{ 
  std::string OutDir      = "OUT_JeanFrancois/" ;
  
  int iOptQuiet           = 1 ;
  
  int intUploader         = 2 ; // DESY21
  int StartFrom           = 0 ;
  int iOpt_Initial_YT_fit = 0 ;
  int iOpt_TrackType      = 3 ;
  int ModuleNber          = 0 ;
  
  std::string SelectionSet = "BN_DESY21_Sample" ;
  int Data_to_Use          = 3 ;
  
//
  std::string Tag = "BIDON" ;
  std::string EvtFil = "BIDON" ;

//
  int ISAMPLE_Min = 0 ;
  int ISAMPLE_Max = 3 ;
  for (int ISAMPLE = ISAMPLE_Min; ISAMPLE< ISAMPLE_Max; ISAMPLE++){
    if(
      ISAMPLE!=2
    ) continue ;
  
//--------------------------------------------------------//ISAMPLE 0
    if(ISAMPLE==0){
      Tag = "D21_ZSCAN" ;
      
      Data_to_Use = 0 ;
      
      SelectionSet = "BN_DESY21_Sample" ;
      
      int iFl_Min =  0 ;
      int iFl_Max = 10 ;
      for (int iFl = iFl_Min ; iFl< iFl_Max; iFl++){
        if (iFl==0) EvtFil = "../Real_Data/DESY21/ZSCAN_412/z_360_275_412_02T_26_m40_iter0.root" ;
        if (iFl==1) EvtFil = "../Real_Data/DESY21/ZSCAN_412/z_360_275_412_02T_26_60_iter0.root"  ;
        if (iFl==2) EvtFil = "../Real_Data/DESY21/ZSCAN_412/z_360_275_412_02T_26_160_iter0.root" ;
        if (iFl==3) EvtFil = "../Real_Data/DESY21/ZSCAN_412/z_360_275_412_02T_26_260_iter0.root" ;
        if (iFl==4) EvtFil = "../Real_Data/DESY21/ZSCAN_412/z_360_275_412_02T_26_360_iter0.root" ;
        if (iFl==5) EvtFil = "../Real_Data/DESY21/ZSCAN_412/z_360_275_412_02T_26_460_iter0.root" ;
        if (iFl==6) EvtFil = "../Real_Data/DESY21/ZSCAN_412/z_360_275_412_02T_26_560_iter0.root" ;
        if (iFl==7) EvtFil = "../Real_Data/DESY21/ZSCAN_412/z_360_275_412_02T_26_660_iter0.root" ;
        if (iFl==8) EvtFil = "../Real_Data/DESY21/ZSCAN_412/z_360_275_412_02T_26_760_iter0.root" ;
        if (iFl==9) EvtFil = "../Real_Data/DESY21/ZSCAN_412/z_360_275_412_02T_26_860_iter0.root" ;

        std::string PRFfile_Fixed_RC_DD = "Data_SignalTime/SignalTime_True_400ns_RC100.00_DD0.00.csv"  ; 

        PV0_BASE(
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

  
//--------------------------------------------------------//ISAMPLE 1
    if(ISAMPLE==1){
      Tag = "T3_Std" ;
      
      Data_to_Use = 3 ;
      
      SelectionSet = "BN_DESY21_Sample" ;
      
      int iFl_Min = 0 ;
      int iFl_Max = 8 ;
      for (int iFl = iFl_Min ; iFl< iFl_Max; iFl++){
        if (iFl==0) EvtFil = "../Real_Data/DESY21/B0/mag_0_200_y80_iter0.root"   ;
        if (iFl==1) EvtFil = "../Real_Data/DESY21/B0/mag_0_200_y40_iter0.root"   ;
        if (iFl==2) EvtFil = "../Real_Data/DESY21/B0/mag_0_200_ym20_iter0.root"  ;
        if (iFl==3) EvtFil = "../Real_Data/DESY21/B0/mag_0_200_ym40_iter0.root"  ;
        if (iFl==4) EvtFil = "../Real_Data/DESY21/B0/mag_0_200_ym60_iter0.root"  ;
        if (iFl==5) EvtFil = "../Real_Data/DESY21/B0/mag_0_200_ym80_iter0.root"  ;
        if (iFl==6) EvtFil = "../Real_Data/DESY21/B0/mag_0_200_ym120_iter0.root" ;
        if (iFl==7) EvtFil = "../Real_Data/DESY21/B0/mag_0_200_ym160_iter0.root" ;

        std::string PRFfile_Fixed_RC_DD = "Data_SignalTime/SignalTime_True_200ns_RC100.00_DD0.00.csv"  ; 

        PV0_BASE(
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
//--------------------------------------------------------//ISAMPLE 1

//--------------------------------------------------------//ISAMPLE 2
    if(ISAMPLE==2){
      Tag = "D21_ZSCAN_200" ;
      
      Data_to_Use = 0 ;
      
      SelectionSet = "BN_DESY21_200_Sample" ;
      
      int iFl_Min =  0 ;
      int iFl_Max = 10 ;
      for (int iFl = iFl_Min ; iFl< iFl_Max; iFl++){
        if (iFl==0) EvtFil = "../Real_Data/DESY21/ZSCAN_200/z_360_275_200_02T_26_m40_iter0.root" ;
        if (iFl==1) EvtFil = "../Real_Data/DESY21/ZSCAN_200/z_360_275_200_02T_26_60_iter0.root"  ;
        if (iFl==2) EvtFil = "../Real_Data/DESY21/ZSCAN_200/z_360_275_200_02T_26_160_iter0.root" ;
        if (iFl==3) EvtFil = "../Real_Data/DESY21/ZSCAN_200/z_360_275_200_02T_26_260_iter0.root" ;
        if (iFl==4) EvtFil = "../Real_Data/DESY21/ZSCAN_200/z_360_275_200_02T_26_360_iter0.root" ;
        if (iFl==5) EvtFil = "../Real_Data/DESY21/ZSCAN_200/z_360_275_200_02T_26_460_iter0.root" ;
        if (iFl==6) EvtFil = "../Real_Data/DESY21/ZSCAN_200/z_360_275_200_02T_26_560_iter0.root" ;
        if (iFl==7) EvtFil = "../Real_Data/DESY21/ZSCAN_200/z_360_275_200_02T_26_660_iter0.root" ;
        if (iFl==8) EvtFil = "../Real_Data/DESY21/ZSCAN_200/z_360_275_200_02T_26_760_iter0.root" ;
        if (iFl==9) EvtFil = "../Real_Data/DESY21/ZSCAN_200/z_360_275_200_02T_26_860_iter0.root" ;

        std::string PRFfile_Fixed_RC_DD = "Data_SignalTime/SignalTime_True_200ns_RC100.00_DD0.00.csv"  ; 

        PV0_BASE(
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
//--------------------------------------------------------//ISAMPLE 2

  }
}
