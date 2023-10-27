#include "JeanFrancois/PV3.h"
#include "Misc/Util.h"

#include "Procedures/PV3_BASE.h"

void PV3()
{ 
  std::string OutDir      = "OUT_JeanFrancois/" ;
  
  int iOptQuiet           = 1 ;
  
  int intUploader         = 2 ; // DESY21
  int StartFrom           = 0 ;
  int iOpt_TrackType      = 3 ;
  int ModuleNber          = 0 ;
  
  std::string SelectionSet = "AN_DESY21_Sample" ;
  int Data_to_Use          = 3 ;
  
//
  std::string Tag = "BIDON" ;
  std::string EvtFil = "BIDON" ;

//
  int ISAMPLE_Min = 0 ;
  int ISAMPLE_Max = 5 ;
  for (int ISAMPLE = ISAMPLE_Min; ISAMPLE< ISAMPLE_Max; ISAMPLE++){
    if(
       ISAMPLE!=0
     &&ISAMPLE!=3
    ) continue ;
  
//--------------------------------------------------------//ISAMPLE 0
    if(ISAMPLE==0){
      
      int iCTD_Min = 0 ;
      int iCTD_Max = 8 ;
      for (int iCTD = iCTD_Min ; iCTD< iCTD_Max; iCTD++){

        double CTD = 0. ;
        if (iCTD==0) CTD = 260. ;
        if (iCTD==1) CTD = 270. ;
        if (iCTD==2) CTD = 280. ;
        if (iCTD==3) CTD = 290. ;
        if (iCTD==4) CTD = 300. ;
        if (iCTD==5) CTD = 310. ;
        if (iCTD==6) CTD = 320. ;
        if (iCTD==7) CTD = 330. ;
        
        Tag = "D21_ZSCAN_TD_" ;  

        std::ostringstream aostringstream_CTD ;
        aostringstream_CTD << std::setiosflags(std::ios::fixed) ;
        aostringstream_CTD << Tag  ;
        aostringstream_CTD << std::setprecision(0) << CTD   ;
        aostringstream_CTD << "_00"   ;       
        Tag = aostringstream_CTD.str() ;  ;

        Data_to_Use = 3 ;
      
        SelectionSet = "AN_DESY21_Sample" ;
      
        int iFl_Min =  0 ;
        int iFl_Max = 10 ;
        for (int iFl = iFl_Min ; iFl< iFl_Max; iFl++){
          if (iFl==0) EvtFil = "../Real_Data/Tristan_DESY21/ZSCAN_412/z_360_275_412_02T_26_m40_iter0.root" ;
          if (iFl==1) EvtFil = "../Real_Data/Tristan_DESY21/ZSCAN_412/z_360_275_412_02T_26_60_iter0.root"  ;
          if (iFl==2) EvtFil = "../Real_Data/Tristan_DESY21/ZSCAN_412/z_360_275_412_02T_26_160_iter0.root" ;
          if (iFl==3) EvtFil = "../Real_Data/Tristan_DESY21/ZSCAN_412/z_360_275_412_02T_26_260_iter0.root" ;
          if (iFl==4) EvtFil = "../Real_Data/Tristan_DESY21/ZSCAN_412/z_360_275_412_02T_26_360_iter0.root" ;
          if (iFl==5) EvtFil = "../Real_Data/Tristan_DESY21/ZSCAN_412/z_360_275_412_02T_26_460_iter0.root" ;
          if (iFl==6) EvtFil = "../Real_Data/Tristan_DESY21/ZSCAN_412/z_360_275_412_02T_26_560_iter0.root" ;
          if (iFl==7) EvtFil = "../Real_Data/Tristan_DESY21/ZSCAN_412/z_360_275_412_02T_26_660_iter0.root" ;
          if (iFl==8) EvtFil = "../Real_Data/Tristan_DESY21/ZSCAN_412/z_360_275_412_02T_26_760_iter0.root" ;
          if (iFl==9) EvtFil = "../Real_Data/Tristan_DESY21/ZSCAN_412/z_360_275_412_02T_26_860_iter0.root" ;

          double ZZ = 0. ;
          if (iFl==0)  ZZ = -40.00  ;
          if (iFl==1)  ZZ =  60.00  ;
          if (iFl==2)  ZZ = 160.00  ;
          if (iFl==3)  ZZ = 260.00  ;
          if (iFl==4)  ZZ = 360.00  ;
          if (iFl==5)  ZZ = 460.00  ;
          if (iFl==6)  ZZ = 560.00  ;
          if (iFl==7)  ZZ = 660.00  ;
          if (iFl==8)  ZZ = 760.00  ;
          if (iFl==9)  ZZ = 860.00  ;
 
          double DD_Sample = ZZ + 137.46 ;
          DD_Sample *= std::pow((CTD/237.00),2) ;
          
          double SamplingFrequency = 20. ; // in MHz

          std::string FILE_SC_VS_x_rc_dd = "Data_SignalTime/Load_400ns_VS_x_rc_dd.csv" ;

          PV3_BASE(
                   iOptQuiet               ,
                   Tag                     ,
                   OutDir                  ,
                   EvtFil                  ,
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
      }
    } 
//--------------------------------------------------------//ISAMPLE 0


//--------------------------------------------------------//ISAMPLE 1
    if(ISAMPLE==1){
      Tag = "T3_w2" ;
      
      int iFl_Min = 0 ;
      int iFl_Max = 8 ;
      for (int iFl = iFl_Min ; iFl< iFl_Max; iFl++){
        if (iFl==0) EvtFil = "../Real_Data/Tristan_DESY21/B0/mag_0_200_y80_iter0.root"   ;
        if (iFl==1) EvtFil = "../Real_Data/Tristan_DESY21/B0/mag_0_200_y40_iter0.root"   ;
        if (iFl==2) EvtFil = "../Real_Data/Tristan_DESY21/B0/mag_0_200_ym20_iter0.root"  ;
        if (iFl==3) EvtFil = "../Real_Data/Tristan_DESY21/B0/mag_0_200_ym40_iter0.root"  ;
        if (iFl==4) EvtFil = "../Real_Data/Tristan_DESY21/B0/mag_0_200_ym60_iter0.root"  ;
        if (iFl==5) EvtFil = "../Real_Data/Tristan_DESY21/B0/mag_0_200_ym80_iter0.root"  ;
        if (iFl==6) EvtFil = "../Real_Data/Tristan_DESY21/B0/mag_0_200_ym120_iter0.root" ;
        if (iFl==7) EvtFil = "../Real_Data/Tristan_DESY21/B0/mag_0_200_ym160_iter0.root" ;

        double ZZ = 860.00  ;
 
        double DD_Sample = ZZ + 137.46 ;

        double SamplingFrequency = 20. ; // in MHz

        std::string FILE_SC_VS_x_rc_dd = "Data_SignalTime/Load_200ns_VS_x_rc_dd.csv" ;

        PV3_BASE(
                 iOptQuiet               ,
                 Tag                     ,
                 OutDir                  ,
                 EvtFil                  ,
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
    } 
//--------------------------------------------------------//ISAMPLE 1


//--------------------------------------------------------//ISAMPLE 2
    if(ISAMPLE==2){
      Tag = "T3_w2" ;
      
      int iFl_Min =  0 ;
      int iFl_Max = 10 ;
      for (int iFl = iFl_Min ; iFl< iFl_Max; iFl++){
        if (iFl==0) EvtFil = "../Real_Data/Tristan_DESY21/ZSCAN_200/z_360_275_200_02T_26_m40_iter0.root" ;
        if (iFl==1) EvtFil = "../Real_Data/Tristan_DESY21/ZSCAN_200/z_360_275_200_02T_26_60_iter0.root"  ;
        if (iFl==2) EvtFil = "../Real_Data/Tristan_DESY21/ZSCAN_200/z_360_275_200_02T_26_160_iter0.root" ;
        if (iFl==3) EvtFil = "../Real_Data/Tristan_DESY21/ZSCAN_200/z_360_275_200_02T_26_260_iter0.root" ;
        if (iFl==4) EvtFil = "../Real_Data/Tristan_DESY21/ZSCAN_200/z_360_275_200_02T_26_360_iter0.root" ;
        if (iFl==5) EvtFil = "../Real_Data/Tristan_DESY21/ZSCAN_200/z_360_275_200_02T_26_460_iter0.root" ;
        if (iFl==6) EvtFil = "../Real_Data/Tristan_DESY21/ZSCAN_200/z_360_275_200_02T_26_560_iter0.root" ;
        if (iFl==7) EvtFil = "../Real_Data/Tristan_DESY21/ZSCAN_200/z_360_275_200_02T_26_660_iter0.root" ;
        if (iFl==8) EvtFil = "../Real_Data/Tristan_DESY21/ZSCAN_200/z_360_275_200_02T_26_760_iter0.root" ;
        if (iFl==9) EvtFil = "../Real_Data/Tristan_DESY21/ZSCAN_200/z_360_275_200_02T_26_860_iter0.root" ;

        double ZZ = 0. ;
        if (iFl==0)  ZZ = -40.00  ;
        if (iFl==1)  ZZ =  60.00  ;
        if (iFl==2)  ZZ = 160.00  ;
        if (iFl==3)  ZZ = 260.00  ;
        if (iFl==4)  ZZ = 360.00  ;
        if (iFl==5)  ZZ = 460.00  ;
        if (iFl==6)  ZZ = 560.00  ;
        if (iFl==7)  ZZ = 660.00  ;
        if (iFl==8)  ZZ = 760.00  ;
        if (iFl==9)  ZZ = 860.00  ;
 
        double DD_Sample = ZZ + 137.46 ;

        double SamplingFrequency = 20. ; // in MHz

        std::string FILE_SC_VS_x_rc_dd = "Data_SignalTime/Load_200ns_VS_x_rc_dd.csv" ;

        PV3_BASE(
                 iOptQuiet               ,
                 Tag                     ,
                 OutDir                  ,
                 EvtFil                  ,
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
    } 
//--------------------------------------------------------//ISAMPLE 2

//--------------------------------------------------------//ISAMPLE 3
    if(ISAMPLE==3){

      Tag = "D21_ZSCAN" ;  

      Data_to_Use          = 0 ;
      
      SelectionSet = "AN_DESY21_Sample" ;
      
      int iFl_Min =  0 ;
      int iFl_Max = 10 ;
      for (int iFl = iFl_Min ; iFl< iFl_Max; iFl++){
        if (iFl==0) EvtFil = "../Real_Data/Tristan_DESY21/ZSCAN_412/z_360_275_412_02T_26_m40_iter0.root" ;
        if (iFl==1) EvtFil = "../Real_Data/Tristan_DESY21/ZSCAN_412/z_360_275_412_02T_26_60_iter0.root"  ;
        if (iFl==2) EvtFil = "../Real_Data/Tristan_DESY21/ZSCAN_412/z_360_275_412_02T_26_160_iter0.root" ;
        if (iFl==3) EvtFil = "../Real_Data/Tristan_DESY21/ZSCAN_412/z_360_275_412_02T_26_260_iter0.root" ;
        if (iFl==4) EvtFil = "../Real_Data/Tristan_DESY21/ZSCAN_412/z_360_275_412_02T_26_360_iter0.root" ;
        if (iFl==5) EvtFil = "../Real_Data/Tristan_DESY21/ZSCAN_412/z_360_275_412_02T_26_460_iter0.root" ;
        if (iFl==6) EvtFil = "../Real_Data/Tristan_DESY21/ZSCAN_412/z_360_275_412_02T_26_560_iter0.root" ;
        if (iFl==7) EvtFil = "../Real_Data/Tristan_DESY21/ZSCAN_412/z_360_275_412_02T_26_660_iter0.root" ;
        if (iFl==8) EvtFil = "../Real_Data/Tristan_DESY21/ZSCAN_412/z_360_275_412_02T_26_760_iter0.root" ;
        if (iFl==9) EvtFil = "../Real_Data/Tristan_DESY21/ZSCAN_412/z_360_275_412_02T_26_860_iter0.root" ;

        double ZZ = 0. ;
        if (iFl==0)  ZZ = -40.00  ;
        if (iFl==1)  ZZ =  60.00  ;
        if (iFl==2)  ZZ = 160.00  ;
        if (iFl==3)  ZZ = 260.00  ;
        if (iFl==4)  ZZ = 360.00  ;
        if (iFl==5)  ZZ = 460.00  ;
        if (iFl==6)  ZZ = 560.00  ;
        if (iFl==7)  ZZ = 660.00  ;
        if (iFl==8)  ZZ = 760.00  ;
        if (iFl==9)  ZZ = 860.00  ;
 
        double DD_Sample = ZZ + 137.46 ;
        
        double SamplingFrequency = 20. ; // in MHz

        std::string FILE_SC_VS_x_rc_dd = "Data_SignalTime/Load_400ns_VS_x_rc_dd.csv" ;

        PV3_BASE(
                 iOptQuiet               ,
                 Tag                     ,
                 OutDir                  ,
                 EvtFil                  ,
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
    } 
//--------------------------------------------------------//ISAMPLE 3

//--------------------------------------------------------//ISAMPLE 4
    if(ISAMPLE==4){

      Tag = "D21_ZSCAN_Improved" ;  

      Data_to_Use = 3 ;
      
      SelectionSet = "AN_DESY21_Sample" ;
      
      int iFl_Min =  0 ;
      int iFl_Max = 10 ;
      for (int iFl = iFl_Min ; iFl< iFl_Max; iFl++){
        if (iFl==0) EvtFil = "../Real_Data/Tristan_DESY21/ZSCAN_412/z_360_275_412_02T_26_m40_iter0.root" ;
        if (iFl==1) EvtFil = "../Real_Data/Tristan_DESY21/ZSCAN_412/z_360_275_412_02T_26_60_iter0.root"  ;
        if (iFl==2) EvtFil = "../Real_Data/Tristan_DESY21/ZSCAN_412/z_360_275_412_02T_26_160_iter0.root" ;
        if (iFl==3) EvtFil = "../Real_Data/Tristan_DESY21/ZSCAN_412/z_360_275_412_02T_26_260_iter0.root" ;
        if (iFl==4) EvtFil = "../Real_Data/Tristan_DESY21/ZSCAN_412/z_360_275_412_02T_26_360_iter0.root" ;
        if (iFl==5) EvtFil = "../Real_Data/Tristan_DESY21/ZSCAN_412/z_360_275_412_02T_26_460_iter0.root" ;
        if (iFl==6) EvtFil = "../Real_Data/Tristan_DESY21/ZSCAN_412/z_360_275_412_02T_26_560_iter0.root" ;
        if (iFl==7) EvtFil = "../Real_Data/Tristan_DESY21/ZSCAN_412/z_360_275_412_02T_26_660_iter0.root" ;
        if (iFl==8) EvtFil = "../Real_Data/Tristan_DESY21/ZSCAN_412/z_360_275_412_02T_26_760_iter0.root" ;
        if (iFl==9) EvtFil = "../Real_Data/Tristan_DESY21/ZSCAN_412/z_360_275_412_02T_26_860_iter0.root" ;

        double ZZ = 0. ;
        if (iFl==0)  ZZ = -40.00  ;
        if (iFl==1)  ZZ =  60.00  ;
        if (iFl==2)  ZZ = 160.00  ;
        if (iFl==3)  ZZ = 260.00  ;
        if (iFl==4)  ZZ = 360.00  ;
        if (iFl==5)  ZZ = 460.00  ;
        if (iFl==6)  ZZ = 560.00  ;
        if (iFl==7)  ZZ = 660.00  ;
        if (iFl==8)  ZZ = 760.00  ;
        if (iFl==9)  ZZ = 860.00  ;
 
        double DD_Sample = ZZ + 137.46 ;
        DD_Sample *= std::pow((310.00/237.00),2) ;
        
        double SamplingFrequency = 20. ; // in MHz

        std::string FILE_SC_VS_x_rc_dd = "Data_SignalTime/Load_400ns_VS_x_rc_dd.csv" ;

        PV3_BASE(
                 iOptQuiet               ,
                 Tag                     ,
                 OutDir                  ,
                 EvtFil                  ,
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
    } 
//--------------------------------------------------------//ISAMPLE 4

  }

}
