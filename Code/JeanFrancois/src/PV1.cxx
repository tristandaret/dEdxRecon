#include "JeanFrancois/PV1.h"
#include "Misc/Util.h"

#include "Procedures/PV1_BASE.h"

void PV1()
{ 
  std::string OutDir      = "OUT_JeanFrancois/" ;
  
  int iOptQuiet           = 0 ;
  
  int intUploader         = 2 ; // DESY21
  int StartFrom           = 0 ;
  int iOpt_Initial_YT_fit = 0 ;
  int iOpt_TrackType      = 3 ;
  int ModuleNber          = 0 ;
  
  std::string SelectionSet = "BN_DESY21_Sample" ;
  int Data_to_Use          = 3 ;
  
  std::cout << std::endl ;
  std::cout << " Analysis: PV1 "  << std::endl ;
    
//
  for (int iOptFlavor = 0; iOptFlavor< 2; iOptFlavor++){

//--------------------------------------------------------//
  if (iOptFlavor==0){
    std::string Flavor = "T3_w0" ;
    
    int iFile_Max = 9 ;
//      iFile_Max = 1 ;
    for (int iFile = 0 ; iFile< iFile_Max; iFile++){
      std::string EventFile = "BIDON" ;
      if (iFile==0) EventFile = "../Real_Data/DESY21/ZSCAN_412/z_360_275_412_02T_26_60_iter0.root"  ;
      if (iFile==1) EventFile = "../Real_Data/DESY21/ZSCAN_412/z_360_275_412_02T_26_160_iter0.root" ;
      if (iFile==2) EventFile = "../Real_Data/DESY21/ZSCAN_412/z_360_275_412_02T_26_260_iter0.root" ;
      if (iFile==3) EventFile = "../Real_Data/DESY21/ZSCAN_412/z_360_275_412_02T_26_360_iter0.root" ;
      if (iFile==4) EventFile = "../Real_Data/DESY21/ZSCAN_412/z_360_275_412_02T_26_460_iter0.root" ;
      if (iFile==5) EventFile = "../Real_Data/DESY21/ZSCAN_412/z_360_275_412_02T_26_560_iter0.root" ;
      if (iFile==6) EventFile = "../Real_Data/DESY21/ZSCAN_412/z_360_275_412_02T_26_660_iter0.root" ;
      if (iFile==7) EventFile = "../Real_Data/DESY21/ZSCAN_412/z_360_275_412_02T_26_760_iter0.root" ;
      if (iFile==8) EventFile = "../Real_Data/DESY21/ZSCAN_412/z_360_275_412_02T_26_860_iter0.root" ;
      
      std::string PRFfile_Fixed_RC_DD = "BIDON" ;
      PRFfile_Fixed_RC_DD  = "Data_SignalTime/SignalTime_True_400ns_RC100.00_DD0.00.csv"  ; 

      std::string PRFfile_Fixed_DD = "BIDON" ;
      PRFfile_Fixed_DD   = "Data_SignalTime/LoadRC_400ns_DD_00.00.csv"  ;

      PV1_BASE(
               iOptQuiet           ,
               Flavor              ,
               OutDir              ,
               EventFile           ,
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
//--------------------------------------------------------//
    
//--------------------------------------------------------//
  if (iOptFlavor==1){
    std::string Flavor = "T3_w2" ;
    
    int iFile_Max = 9 ;
//      iFile_Max = 1 ;
    for (int iFile = 0 ; iFile< iFile_Max; iFile++){
      std::string EventFile = "BIDON" ;
      if (iFile==0) EventFile = "../Real_Data/DESY21/ZSCAN_412/z_360_275_412_02T_26_60_iter0.root"  ;
      if (iFile==1) EventFile = "../Real_Data/DESY21/ZSCAN_412/z_360_275_412_02T_26_160_iter0.root" ;
      if (iFile==2) EventFile = "../Real_Data/DESY21/ZSCAN_412/z_360_275_412_02T_26_260_iter0.root" ;
      if (iFile==3) EventFile = "../Real_Data/DESY21/ZSCAN_412/z_360_275_412_02T_26_360_iter0.root" ;
      if (iFile==4) EventFile = "../Real_Data/DESY21/ZSCAN_412/z_360_275_412_02T_26_460_iter0.root" ;
      if (iFile==5) EventFile = "../Real_Data/DESY21/ZSCAN_412/z_360_275_412_02T_26_560_iter0.root" ;
      if (iFile==6) EventFile = "../Real_Data/DESY21/ZSCAN_412/z_360_275_412_02T_26_660_iter0.root" ;
      if (iFile==7) EventFile = "../Real_Data/DESY21/ZSCAN_412/z_360_275_412_02T_26_760_iter0.root" ;
      if (iFile==8) EventFile = "../Real_Data/DESY21/ZSCAN_412/z_360_275_412_02T_26_860_iter0.root" ;
      
      std::string PRFfile_Fixed_RC_DD = "BIDON" ;
      if (iFile==0) PRFfile_Fixed_RC_DD  = "Data_SignalTime/SignalTime_True_400ns_RC100.00_DD197.46_Desy21_ZZ60.00.csv"  ;
      if (iFile==1) PRFfile_Fixed_RC_DD  = "Data_SignalTime/SignalTime_True_400ns_RC100.00_DD297.46_Desy21_ZZ160.00.csv" ;
      if (iFile==2) PRFfile_Fixed_RC_DD  = "Data_SignalTime/SignalTime_True_400ns_RC100.00_DD397.46_Desy21_ZZ260.00.csv" ;
      if (iFile==3) PRFfile_Fixed_RC_DD  = "Data_SignalTime/SignalTime_True_400ns_RC100.00_DD497.46_Desy21_ZZ360.00.csv" ;
      if (iFile==4) PRFfile_Fixed_RC_DD  = "Data_SignalTime/SignalTime_True_400ns_RC100.00_DD597.46_Desy21_ZZ460.00.csv" ;
      if (iFile==5) PRFfile_Fixed_RC_DD  = "Data_SignalTime/SignalTime_True_400ns_RC100.00_DD697.46_Desy21_ZZ560.00.csv" ;
      if (iFile==6) PRFfile_Fixed_RC_DD  = "Data_SignalTime/SignalTime_True_400ns_RC100.00_DD797.46_Desy21_ZZ660.00.csv" ;
      if (iFile==7) PRFfile_Fixed_RC_DD  = "Data_SignalTime/SignalTime_True_400ns_RC100.00_DD897.46_Desy21_ZZ760.00.csv" ;
      if (iFile==8) PRFfile_Fixed_RC_DD  = "Data_SignalTime/SignalTime_True_400ns_RC100.00_DD997.46_Desy21_ZZ860.00.csv" ;

      std::string PRFfile_Fixed_DD = "BIDON" ;
      if (iFile==0) PRFfile_Fixed_DD   = "Data_SignalTime/LoadRC_400ns_DD197.46_Desy21_ZZ60.00.csv"  ;
      if (iFile==1) PRFfile_Fixed_DD   = "Data_SignalTime/LoadRC_400ns_DD297.46_Desy21_ZZ160.00.csv" ;
      if (iFile==2) PRFfile_Fixed_DD   = "Data_SignalTime/LoadRC_400ns_DD397.46_Desy21_ZZ260.00.csv" ;
      if (iFile==3) PRFfile_Fixed_DD   = "Data_SignalTime/LoadRC_400ns_DD497.46_Desy21_ZZ360.00.csv" ;
      if (iFile==4) PRFfile_Fixed_DD   = "Data_SignalTime/LoadRC_400ns_DD597.46_Desy21_ZZ460.00.csv" ;
      if (iFile==5) PRFfile_Fixed_DD   = "Data_SignalTime/LoadRC_400ns_DD697.46_Desy21_ZZ560.00.csv" ;
      if (iFile==6) PRFfile_Fixed_DD   = "Data_SignalTime/LoadRC_400ns_DD797.46_Desy21_ZZ660.00.csv" ;
      if (iFile==7) PRFfile_Fixed_DD   = "Data_SignalTime/LoadRC_400ns_DD897.46_Desy21_ZZ760.00.csv" ;
      if (iFile==8) PRFfile_Fixed_DD   = "Data_SignalTime/LoadRC_400ns_DD997.46_Desy21_ZZ860.00.csv" ;

      PV1_BASE(
               iOptQuiet           ,
               Flavor              ,
               OutDir              ,
               EventFile           ,
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
//--------------------------------------------------------//
  
  }

  
}
