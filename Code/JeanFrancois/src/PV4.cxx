#include "JeanFrancois/PV4.h"
#include "Misc/Util.h"

#include "Procedures/PV4_BASE.h"

void PV4()
{ 
  std::string OutDir      = "OUT_JeanFrancois/" ;
  
  int iOptQuiet           = 0 ;
  
  int intUploader         = 2 ; // DESY21
  int StartFrom           = 0 ;
  int iOpt_TrackType      = 3 ;
  int ModuleNber          = 0 ;
  
  std::string SelectionSet = "AN_DESY21_Sample" ;
  int Data_to_Use          = 3 ;
  
  std::cout << std::endl ;
  std::cout << " Analysis: PV4 "  << std::endl ;
    
//
  for (int ISAMPLE = 0; ISAMPLE< 3; ISAMPLE++){
//    if(ISAMPLE!=0) continue ;
  
    if(ISAMPLE==0){
//--------------------------------------------------------//ISAMPLE 0
      std::string Flavor = "T3_w2" ;
      
      int iFile_Max = 10 ;
//        iFile_Max = 1 ;
      for (int iFile = 0 ; iFile< iFile_Max; iFile++){
//        if (iFile!=3) continue ;
        std::string EventFile = "BIDON" ;
        if (iFile==0) EventFile = "../Real_Data/DESY21/ZSCAN_412/z_360_275_412_02T_26_m40_iter0.root" ;
        if (iFile==1) EventFile = "../Real_Data/DESY21/ZSCAN_412/z_360_275_412_02T_26_60_iter0.root"  ;
        if (iFile==2) EventFile = "../Real_Data/DESY21/ZSCAN_412/z_360_275_412_02T_26_160_iter0.root" ;
        if (iFile==3) EventFile = "../Real_Data/DESY21/ZSCAN_412/z_360_275_412_02T_26_260_iter0.root" ;
        if (iFile==4) EventFile = "../Real_Data/DESY21/ZSCAN_412/z_360_275_412_02T_26_360_iter0.root" ;
        if (iFile==5) EventFile = "../Real_Data/DESY21/ZSCAN_412/z_360_275_412_02T_26_460_iter0.root" ;
        if (iFile==6) EventFile = "../Real_Data/DESY21/ZSCAN_412/z_360_275_412_02T_26_560_iter0.root" ;
        if (iFile==7) EventFile = "../Real_Data/DESY21/ZSCAN_412/z_360_275_412_02T_26_660_iter0.root" ;
        if (iFile==8) EventFile = "../Real_Data/DESY21/ZSCAN_412/z_360_275_412_02T_26_760_iter0.root" ;
        if (iFile==9) EventFile = "../Real_Data/DESY21/ZSCAN_412/z_360_275_412_02T_26_860_iter0.root" ;

        double ZZ = 0. ;
        if (iFile==0)  ZZ = -40.00  ;
        if (iFile==1)  ZZ =  60.00  ;
        if (iFile==2)  ZZ = 160.00  ;
        if (iFile==3)  ZZ = 260.00  ;
        if (iFile==4)  ZZ = 360.00  ;
        if (iFile==5)  ZZ = 460.00  ;
        if (iFile==6)  ZZ = 560.00  ;
        if (iFile==7)  ZZ = 660.00  ;
        if (iFile==8)  ZZ = 760.00  ;
        if (iFile==9)  ZZ = 860.00  ;
 
        double DD_Sample = ZZ + 137.46 ;
        
        double SamplingFrequency = 20. ; // in MHz

        std::string FILE_SC_VS_x_rc_dd = "Data_SignalTime/Load_400ns_VS_x_rc_dd.csv" ;

        PV4_BASE(
                 iOptQuiet               ,
                 Flavor                  ,
                 OutDir                  ,
                 EventFile               ,
                 FILE_SC_VS_x_rc_dd      ,
                 DD_Sample               ,
                 intUploader             ,
                 StartFrom               ,
                 iOpt_TrackType          ,
                 SamplingFrequency       ,
                 ModuleNber              ,
                 SelectionSet            ,
                 Data_to_Use
                );
      
      }
//--------------------------------------------------------//
    }// ISAMPLE 0  


    if(ISAMPLE==1){
//--------------------------------------------------------//ISAMPLE 1
      std::string Flavor = "T3_w2" ;
      
      int iFile_Max = 8 ;
//        iFile_Max = 1 ;
      for (int iFile = 0 ; iFile< iFile_Max; iFile++){
        std::string EventFile = "BIDON" ;
        if (iFile==0) EventFile = "../Real_Data/DESY21/B0/mag_0_200_y80_iter0.root"   ;
        if (iFile==1) EventFile = "../Real_Data/DESY21/B0/mag_0_200_y40_iter0.root"   ;
        if (iFile==2) EventFile = "../Real_Data/DESY21/B0/mag_0_200_ym20_iter0.root"  ;
        if (iFile==3) EventFile = "../Real_Data/DESY21/B0/mag_0_200_ym40_iter0.root"  ;
        if (iFile==4) EventFile = "../Real_Data/DESY21/B0/mag_0_200_ym60_iter0.root"  ;
        if (iFile==5) EventFile = "../Real_Data/DESY21/B0/mag_0_200_ym80_iter0.root"  ;
        if (iFile==6) EventFile = "../Real_Data/DESY21/B0/mag_0_200_ym120_iter0.root" ;
        if (iFile==7) EventFile = "../Real_Data/DESY21/B0/mag_0_200_ym160_iter0.root" ;

        double ZZ = 860.00  ;
 
        double DD_Sample = ZZ + 137.46 ;

        double SamplingFrequency = 20. ; // in MHz

        std::string FILE_SC_VS_x_rc_dd = "Data_SignalTime/Load_200ns_VS_x_rc_dd.csv" ;

        PV4_BASE(
                 iOptQuiet               ,
                 Flavor                  ,
                 OutDir                  ,
                 EventFile               ,
                 FILE_SC_VS_x_rc_dd      ,
                 DD_Sample               ,
                 intUploader             ,
                 StartFrom               ,
                 iOpt_TrackType          ,
                 SamplingFrequency       ,
                 ModuleNber              ,
                 SelectionSet            ,
                 Data_to_Use
                );
      
      }
//--------------------------------------------------------//
    }// ISAMPLE 1  


    if(ISAMPLE==2){
//--------------------------------------------------------//ISAMPLE 2
      std::string Flavor = "T3_w2" ;
      
      int iFile_Max = 10 ;
//        iFile_Max = 1 ;
      for (int iFile = 0 ; iFile< iFile_Max; iFile++){
        std::string EventFile = "BIDON" ;
        if (iFile==0) EventFile = "../Real_Data/DESY21/ZSCAN_200/z_360_275_200_02T_26_m40_iter0.root" ;
        if (iFile==1) EventFile = "../Real_Data/DESY21/ZSCAN_200/z_360_275_200_02T_26_60_iter0.root"  ;
        if (iFile==2) EventFile = "../Real_Data/DESY21/ZSCAN_200/z_360_275_200_02T_26_160_iter0.root" ;
        if (iFile==3) EventFile = "../Real_Data/DESY21/ZSCAN_200/z_360_275_200_02T_26_260_iter0.root" ;
        if (iFile==4) EventFile = "../Real_Data/DESY21/ZSCAN_200/z_360_275_200_02T_26_360_iter0.root" ;
        if (iFile==5) EventFile = "../Real_Data/DESY21/ZSCAN_200/z_360_275_200_02T_26_460_iter0.root" ;
        if (iFile==6) EventFile = "../Real_Data/DESY21/ZSCAN_200/z_360_275_200_02T_26_560_iter0.root" ;
        if (iFile==7) EventFile = "../Real_Data/DESY21/ZSCAN_200/z_360_275_200_02T_26_660_iter0.root" ;
        if (iFile==8) EventFile = "../Real_Data/DESY21/ZSCAN_200/z_360_275_200_02T_26_760_iter0.root" ;
        if (iFile==9) EventFile = "../Real_Data/DESY21/ZSCAN_200/z_360_275_200_02T_26_860_iter0.root" ;

        double ZZ = 0. ;
        if (iFile==0)  ZZ = -40.00  ;
        if (iFile==1)  ZZ =  60.00  ;
        if (iFile==2)  ZZ = 160.00  ;
        if (iFile==3)  ZZ = 260.00  ;
        if (iFile==4)  ZZ = 360.00  ;
        if (iFile==5)  ZZ = 460.00  ;
        if (iFile==6)  ZZ = 560.00  ;
        if (iFile==7)  ZZ = 660.00  ;
        if (iFile==8)  ZZ = 760.00  ;
        if (iFile==9)  ZZ = 860.00  ;
 
        double DD_Sample = ZZ + 137.46 ;

        double SamplingFrequency = 20. ; // in MHz

        std::string FILE_SC_VS_x_rc_dd = "Data_SignalTime/Load_200ns_VS_x_rc_dd.csv" ;

        PV4_BASE(
                 iOptQuiet               ,
                 Flavor                  ,
                 OutDir                  ,
                 EventFile               ,
                 FILE_SC_VS_x_rc_dd      ,
                 DD_Sample               ,
                 intUploader             ,
                 StartFrom               ,
                 iOpt_TrackType          ,
                 SamplingFrequency       ,
                 ModuleNber              ,
                 SelectionSet            ,
                 Data_to_Use
                );
      
      }
//--------------------------------------------------------//
    }// ISAMPLE 2  

  }

}
